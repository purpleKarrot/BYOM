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

#ifndef BYOM_EXT_TUPLE_HPP
#define BYOM_EXT_TUPLE_HPP

#include <byom/ext/arithmetic.hpp>
#include <byom/ext/fallback.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

namespace byom {

template <typename... T>
struct ext<std::tuple<T...>> : detail::fallback
{
  using model_t = std::tuple<T...>;

  static bool empty_impl(model_t const& model)
  {
    return std::tuple_size<model_t>::value == 0;
  }

  static void for_each_impl(model_t const& model, visit_function const& visit)
  {
    std::size_t index = 0;
    boost::fusion::for_each(model, [&](auto const& elem) {
      visit(index, elem);
      index += 1;
    });
  }
};

} // namespace byom

#endif /* BYOM_EXT_TUPLE_HPP */
