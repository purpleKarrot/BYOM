// Copyright (c) 2015, Daniel Pfeifer <daniel@pfeifer-mail.de>
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#ifndef BYOM_DYNAMIC_VIEW_HPP
#define BYOM_DYNAMIC_VIEW_HPP

#include <string>
#include <memory>
#include <functional>
#include <type_traits>
#include <stdexcept>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

namespace byom {

class dynamic_view;

using visit_function =
  std::function<void(dynamic_view const&, dynamic_view const&)>;

template <typename T, typename Enable = void>
struct ext;

class dynamic_view
{
  template <typename T>
  using no_copy_ctor = typename std::enable_if<!std::is_same<
    typename std::remove_reference<T>::type, dynamic_view>::value>::type;

public:
  template <typename T, typename Enable = no_copy_ctor<T>>
  dynamic_view(T&& t)
    : dynamic_view(std::forward<T>(t), std::is_lvalue_reference<T>())
  {
  }

  template <typename T>
  dynamic_view(T* t)
  {
    new (storage()) local_model_t<T*>(t);
  }

  template <typename T>
  dynamic_view(T const* t)
  {
    new (storage()) local_model_t<T const*>(t);
  }

  dynamic_view(dynamic_view const& x)
  {
    x.object().clone(storage());
  }

  dynamic_view(dynamic_view&& x)
  {
    x.object().move_clone(storage());
  }

  ~dynamic_view()
  {
    object().~concept_t();
  }

  dynamic_view& operator=(dynamic_view&& x) = delete;
  dynamic_view& operator=(dynamic_view const& x) = delete;

public:
  bool empty() const
  {
    return object().empty();
  }

  dynamic_view at(std::string const& n) const &
  {
    return object().at(n);
  }

  dynamic_view at(std::string const& n) const &&
  {
    auto member = object().at(n);
    if (object().owned() && !member.object().owned()) {
      throw std::invalid_argument{ "dangling reference" };
    }
    return member;
  }

  void for_each(visit_function const& v) const
  {
    object().for_each(v);
  }

  template <typename T>
  T get() const
  {
    return cast_helper<T>::cast(object());
  }

  friend std::ostream& operator<<(std::ostream& os, dynamic_view const& self)
  {
    self.object().print(os);
    return os;
  }

private:
  struct concept_t
  {
    virtual ~concept_t() = default;
    virtual void clone(void* storage) const = 0;
    virtual void move_clone(void* storage) = 0;
    virtual bool owned() const = 0;
    virtual const std::type_info& type() const = 0;
    virtual void const* data() const = 0;
    virtual bool empty() const = 0;
    virtual dynamic_view at(std::string const& n) const = 0;
    virtual void for_each(visit_function const& v) const = 0;
    virtual unsigned long long int to_integral() const = 0;
    virtual long double to_floating_point() const = 0;
    virtual std::string to_string() const = 0;
    virtual void print(std::ostream& os) const = 0;
  };

  template <template <typename> class Derived, typename T>
  struct model_base_t : concept_t
  {
    const std::type_info& type() const override
    {
      return typeid(T);
    }

    void const* data() const override
    {
      return &get();
    }

    bool empty() const override
    {
      return ext<T>::empty_impl(get());
    }

    dynamic_view at(std::string const& n) const override
    {
      return ext<T>::at_impl(get(), n);
    }

    void for_each(visit_function const& v) const override
    {
      ext<T>::for_each_impl(get(), v);
    }

    unsigned long long int to_integral() const override
    {
      return ext<T>::to_integral(get());
    }

    long double to_floating_point() const override
    {
      return ext<T>::to_floating_point(get());
    }

    std::string to_string() const override
    {
      return ext<T>::to_string(get());
    }

    void print(std::ostream& os) const override
    {
      ext<T>::print_impl(os, get());
    }

    T const& get() const
    {
      return static_cast<Derived<T> const*>(this)->get();
    }
  };

  template <typename T>
  struct cref_model_t : model_base_t<cref_model_t, T>
  {
    cref_model_t(T const& x)
      : object(x)
    {
    }

    void clone(void* storage) const override
    {
      new (storage) cref_model_t(object);
    }

    void move_clone(void* storage) override
    {
      clone(storage);
    }

    bool owned() const override
    {
      return false;
    }

    T const& get() const
    {
      return object;
    }

    T const& object;
  };

  template <typename T>
  struct local_model_t : model_base_t<local_model_t, T>
  {
    local_model_t(T x)
      : object(std::move(x))
    {
    }

    void clone(void* storage) const override
    {
      new (storage) local_model_t(object);
    }

    bool owned() const override
    {
      return true;
    }

    void move_clone(void* storage) override
    {
      new (storage) local_model_t(std::move(object));
    }

    T const& get() const
    {
      return object;
    }

    T object;
  };

  template <typename T>
  struct remote_model_t : model_base_t<remote_model_t, T>
  {
    remote_model_t(T x)
      : object(std::make_unique<T const>(std::move(x)))
    {
    }

    void clone(void* storage) const override
    {
      new (storage) remote_model_t(get());
    }

    void move_clone(void* storage) override
    {
      new (storage) remote_model_t(std::move(*this));
    }

    bool owned() const override
    {
      return true;
    }

    T const& get() const
    {
      return *object;
    }

    std::unique_ptr<T const> object;
  };

private:
  template <typename T, typename Enable = void>
  struct cast_helper
  {
    static T cast(concept_t const& object)
    {
      if (object.type() != typeid(T)) {
        throw std::bad_cast{ "bad cast" };
      }
      return *reinterpret_cast<T const*>(object.data());
    }
  };

  template <typename T>
  struct cast_helper<T,
                     typename std::enable_if<std::is_integral<T>::value>::type>
  {
    static T cast(concept_t const& object)
    {
      return static_cast<T>(object.to_integral());
    }
  };

  template <typename T>
  struct cast_helper<
    T, typename std::enable_if<std::is_floating_point<T>::value>::type>
  {
    static T cast(concept_t const& object)
    {
      return static_cast<T>(object.to_floating_point());
    }
  };

  template <typename T>
  struct cast_helper<
    T, typename std::enable_if<std::is_same<T, std::string>::value>::type>
  {
    static T cast(concept_t const& object)
    {
      return object.to_string();
    }
  };

private:
  template <typename T>
  dynamic_view(T&& t, std::true_type)
  {
    using model = cref_model_t<typename std::decay<T>::type>;
    static_assert(sizeof(model) <= sizeof(data), "size mismatch");
    new (storage()) model(t);
  }

  template <typename T>
  dynamic_view(T&& t, std::false_type)
  {
    using local_type = local_model_t<typename std::decay<T>::type>;
    using remote_type = remote_model_t<typename std::decay<T>::type>;
    using use_local_type =
      boost::mpl::bool_<(sizeof(local_type) <= sizeof(data)) &&
                        (std::is_nothrow_copy_constructible<T>::value ||
                         std::is_nothrow_move_constructible<T>::value)>;
    using model =
      typename boost::mpl::if_<use_local_type, local_type, remote_type>::type;
    static_assert(sizeof(model) <= sizeof(data), "size mismatch");
    new (storage()) model(std::move(t));
  }

private:
  concept_t& object()
  {
    return *static_cast<concept_t*>(storage());
  }

  concept_t const& object() const
  {
    return *static_cast<concept_t const*>(storage());
  }

  void* storage()
  {
    return &data;
  }

  void const* storage() const
  {
    return &data;
  }

  double data[2];
};

} // namespace byom

#endif /* BYOM_DYNAMIC_VIEW_HPP */
