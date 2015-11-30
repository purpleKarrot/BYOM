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

#ifndef BYOM_EXT_MAP_HPP
#define BYOM_EXT_MAP_HPP

#include <byom/dynamic_view.hpp>
#include <map>

namespace byom {

template <typename K, typename T, typename C, typename A>
struct ext<std::map<K, T, C, A>> : detail::fallback
{
  using model_t = std::map<K, T, C, A>;

  static bool empty_impl(model_t const& model)
  {
    return model.empty();
  }

  static T const& at_impl(model_t const& model, std::string const& name)
  {
    return model.at(name);
  }

  static void for_each_impl(model_t const& model, visit_function const& visit)
  {
    for (auto const& elem : model) {
      visit(elem.first, elem.second);
    }
  }
};

} // namespace byom

#endif /* BYOM_EXT_MAP_HPP */
