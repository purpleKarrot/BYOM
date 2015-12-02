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

#ifndef BYOM_EXT_STRING_HPP
#define BYOM_EXT_STRING_HPP

#include <byom/dynamic_view.hpp>
#include <byom/ext/fallback.hpp>
#include <string>

namespace byom {

template <typename C, typename T, typename A>
struct ext<std::basic_string<C, T, A>> : detail::fallback
{
  using model_t = std::basic_string<C, T, A>;

  static bool empty_impl(model_t const& model)
  {
    return model.empty();
  }

  static unsigned long long int to_integral(model_t const& model)
  {
    return std::stoull(model);
  }

  static long double to_floating_point(model_t const& model)
  {
    return std::stold(model);
  }

  static std::string to_string(model_t const& model)
  {
    return model;
  }

  static void print_impl(std::ostream& os, model_t const& model)
  {
    os << model;
  }
};

} // namespace byom

#endif /* BYOM_EXT_STRING_HPP */
