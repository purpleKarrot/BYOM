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
#include <boost/type_index.hpp>

namespace byom {

class dynamic_view;

using visit_function =
  std::function<void(dynamic_view const&, dynamic_view const&)>;

template <typename T, typename Enable = void>
struct ext;

namespace detail {

struct fallback
{
  template <typename T>
  static bool empty_impl(T const&)
  {
    using boost::typeindex::type_id_with_cvr;
    throw std::invalid_argument{ "'empty(" +
                                 type_id_with_cvr<T const&>().pretty_name() +
                                 ")' is not implemented" };
  }

  template <typename T, typename K>
  static std::nullptr_t const& at_impl(T const&, K const&)
  {
    using boost::typeindex::type_id_with_cvr;
    throw std::invalid_argument{
      "'at(" + type_id_with_cvr<T const&>().pretty_name() + ", " +
      type_id_with_cvr<K const&>().pretty_name() + ")' is not implemented"
    };
  }

  template <typename T>
  static void for_each_impl(T const&, visit_function const&)
  {
    using boost::typeindex::type_id_with_cvr;
    throw std::invalid_argument{
      "'for_each(" + type_id_with_cvr<T const&>().pretty_name() +
      ", byom::visit_function const&)' is not implemented"
    };
  }

  template <typename T>
  static void print_impl(std::ostream& os, T const&)
  {
    using boost::typeindex::type_id_with_cvr;
    throw std::invalid_argument{ "'print(std::ostream&, " +
                                 type_id_with_cvr<T const&>().pretty_name() +
                                 ")' is not implemented" };
  }
};

} // namespace detail

class dynamic_view
{
public:
  template <typename T>
  dynamic_view(T&& t)
    : dynamic_view(std::forward<T>(t), std::is_lvalue_reference<T>())
  {
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

  dynamic_view at(std::string const& n) const&& = delete;

  void for_each(visit_function const& v) const
  {
    object().for_each(v);
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
    virtual bool empty() const = 0;
    virtual dynamic_view at(std::string const& n) const = 0;
    virtual void for_each(visit_function const& v) const = 0;
    virtual void print(std::ostream& os) const = 0;
  };

  template <template <typename> class Derived, typename T>
  struct model_base_t : concept_t
  {
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
      new (storage) remote_model_t(object);
    }

    void move_clone(void* storage) override
    {
      new (storage) remote_model_t(std::move(*this));
    }

    T const& get() const
    {
      return *object;
    }

    std::unique_ptr<T const> object;
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
    using local_type = local_model_t<T>;
    using remote_type = remote_model_t<T>;
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
