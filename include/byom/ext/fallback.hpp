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

#ifndef BYOM_EXT_FALLBACK_HPP
#define BYOM_EXT_FALLBACK_HPP

#include <boost/type_index.hpp>
#include <stdexcept>
#include <ostream>

namespace byom {
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
  static unsigned long long int to_integral(T const&)
  {
    using boost::typeindex::type_id_with_cvr;
    throw std::invalid_argument{ "'to_integral(" +
                                 type_id_with_cvr<T const&>().pretty_name() +
                                 ")' is not implemented" };
  }

  template <typename T>
  static long double to_floating_point(T const&)
  {
    using boost::typeindex::type_id_with_cvr;
    throw std::invalid_argument{ "'to_floating_point(" +
                                 type_id_with_cvr<T const&>().pretty_name() +
                                 ")' is not implemented" };
  }

  template <typename T>
  static std::string to_string(T const&)
  {
    using boost::typeindex::type_id_with_cvr;
    throw std::invalid_argument{ "'to_string(" +
                                 type_id_with_cvr<T const&>().pretty_name() +
                                 ")' is not implemented" };
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

template <>
struct ext<std::nullptr_t> : detail::fallback
{
  static bool empty_impl(std::nullptr_t)
  {
    return true;
  }

  static void print_impl(std::ostream& os, std::nullptr_t)
  {
    os << "null";
  }
};

} // namespace byom

#endif /* BYOM_EXT_FALLBACK_HPP */
