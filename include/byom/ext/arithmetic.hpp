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

#ifndef BYOM_EXT_ARITHMETIC_HPP
#define BYOM_EXT_ARITHMETIC_HPP

#include <byom/dynamic_view.hpp>
#include <type_traits>
#include <ostream>

namespace byom {

template <typename T>
struct ext<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
  : detail::fallback
{
  static bool empty_impl(T const& model)
  {
    return model == T{};
  }

  static void print_impl(std::ostream& os, T const& model)
  {
    os << model;
  }
};

} // namespace byom

#endif /* BYOM_EXT_ARITHMETIC_HPP */
