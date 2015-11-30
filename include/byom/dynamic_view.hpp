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
#include <functional>
#include <type_traits>
#include <boost/type_index.hpp>

namespace byom {

class dynamic_view;

using visit_function =
  std::function<void(dynamic_view const&, dynamic_view const&)>;

namespace ext {

struct tag
{
};

} // namespace ext

namespace detail {

template <typename, typename>
struct is_different : std::true_type
{
};

template <typename T>
struct is_different<T, T> : std::false_type
{
};

struct fallback;

fallback empty(...);
fallback at(...);
fallback for_each(...);
fallback print(...);

#define REQUIRE(...) typename std::enable_if<(__VA_ARGS__), int>::type = 0

template <typename T>
using has_empty =
  is_different<fallback,
               decltype(empty(std::declval<T>(), std::declval<ext::tag>()))>;

template <typename T, REQUIRE(has_empty<T>::value)>
bool empty_impl(T const& object)
{
  return empty(object, ext::tag{});
}

template <typename T, REQUIRE(!has_empty<T>::value)>
bool empty_impl(T const&)
{
  using boost::typeindex::type_id_with_cvr;
  throw std::invalid_argument{ "'empty(" +
                               type_id_with_cvr<T const&>().pretty_name() +
                               ")' is not implemented" };
}

template <typename T, typename K>
using has_at =
  is_different<fallback, decltype(at(std::declval<T>(), std::declval<K>(),
                                     std::declval<ext::tag>()))>;

template <typename T, typename K, REQUIRE(has_at<T, K>::value)>
decltype(auto) at_impl(T const& object, K const& name)
{
  return at(object, name, ext::tag{});
}

template <typename T, typename K, REQUIRE(!has_at<T, K>::value)>
std::nullptr_t const& at_impl(T const&, K const&)
{
  using boost::typeindex::type_id_with_cvr;
  throw std::invalid_argument{
    "'at(" + type_id_with_cvr<T const&>().pretty_name() + ", " +
    type_id_with_cvr<K const&>().pretty_name() + ")' is not implemented"
  };
}

template <typename T, typename F>
using has_for_each =
  is_different<fallback, decltype(for_each(std::declval<T>(), std::declval<F>(),
                                           std::declval<ext::tag>()))>;

template <typename T, typename F, REQUIRE(has_for_each<T, F>::value)>
void for_each_impl(T const& object, F const& visit)
{
  for_each(object, visit, ext::tag{});
}

template <typename T, typename F, REQUIRE(!has_for_each<T, F>::value)>
void for_each_impl(T const&, F const&)
{
  throw std::invalid_argument{ "for_each is not implemented" };
}

template <typename T>
using has_print =
  is_different<fallback,
               decltype(print(std::declval<std::ostream&>(), std::declval<T>(),
                              std::declval<ext::tag>()))>;

template <typename T, REQUIRE(has_print<T>::value)>
void print_impl(std::ostream& os, T const& object)
{
  print(os, object, ext::tag{});
}

template <typename T, REQUIRE(!has_print<T>::value)>
void print_impl(std::ostream& os, T const&)
{
  using boost::typeindex::type_id_with_cvr;
  throw std::invalid_argument{ "'print(std::ostream&, " +
                               type_id_with_cvr<T const&>().pretty_name() +
                               ")' is not implemented" };
}

#undef REQUIRE

template <typename... T>
struct wrong : std::false_type
{
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
    x.object().clone(storage());
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

  dynamic_view at(std::string const& n) const
  {
    return object().at(n);
  }

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
    virtual bool empty() const = 0;
    virtual dynamic_view at(std::string const& n) const = 0;
    virtual void for_each(visit_function const& v) const = 0;
    virtual void print(std::ostream& os) const = 0;
  };

  template <typename T>
  struct model_t : concept_t
  {
    model_t(T const& x)
      : object(&x)
    {
    }

    void clone(void* storage) const override
    {
      new (storage) model_t(get());
    }

    bool empty() const override
    {
      return byom::detail::empty_impl(get());
    }

    dynamic_view at(std::string const& n) const override
    {
      return byom::detail::at_impl(get(), n);
    }

    void for_each(visit_function const& v) const override
    {
      byom::detail::for_each_impl(get(), v);
    }

    void print(std::ostream& os) const override
    {
      byom::detail::print_impl(os, get());
    }

    T const& get() const
    {
      return *object;
    }

    T const* object;
  };

private:
  template <typename T>
  dynamic_view(T&& t, std::true_type)
  {
    using model = model_t<typename std::remove_reference<T>::type>;
    static_assert(sizeof(model) <= sizeof(data), "size mismatch");
    new (storage()) model(t);
  }

  template <typename T>
  dynamic_view(T&& t, std::false_type)
  {
    static_assert(detail::wrong<T>::value, "rvalues are not yet supported");
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
