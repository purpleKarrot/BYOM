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
namespace ext {

template <typename K, typename T, typename C, typename A>
bool empty(std::map<K, T, C, A> const& map, tag)
{
  return map.empty();
}

template <typename K, typename T, typename C, typename A>
T const& at(std::map<K, T, C, A> const& map, std::string const& name, tag)
{
  return map.at(name);
}

template <typename K, typename V, typename C, typename A>
void for_each(std::map<K, V, C, A> const& map, visit_function const& visit, tag)
{
  for (auto const& elem : map) {
    visit(elem.first, elem.second);
  }
}

} // namespace ext
} // namespace byom

#endif /* BYOM_EXT_MAP_HPP */
