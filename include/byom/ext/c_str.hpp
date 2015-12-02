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

#ifndef BYOM_EXT_C_STR_HPP
#define BYOM_EXT_C_STR_HPP

#include <byom/dynamic_view.hpp>
#include <byom/ext/fallback.hpp>
#include <ostream>

namespace byom {

template <>
struct ext<char const*> : detail::fallback
{
  static bool empty_impl(char const* model)
  {
    return !model || !model[0];
  }

  static unsigned long long int to_integral(char const* model)
  {
    return std::stoull(model);
  }

  static long double to_floating_point(char const* model)
  {
    return std::stold(model);
  }

  static std::string to_string(char const* model)
  {
    return model;
  }

  static void print_impl(std::ostream& os, char const* model)
  {
    os << model;
  }
};

} // namespace byom

#endif /* BYOM_EXT_C_STR_HPP */
