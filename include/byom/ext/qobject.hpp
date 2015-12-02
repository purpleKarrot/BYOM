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

#ifndef BYOM_EXT_QOBJECT_HPP
#define BYOM_EXT_QOBJECT_HPP

#include <byom/dynamic_view.hpp>
#include <byom/ext/fallback.hpp>
#include <QObject>

namespace byom {

template <>
struct ext<QObject const*> : detail::fallback
{
  static bool empty_impl(QObject const* model);

  static dynamic_view at_impl(QObject const* model, std::string const& name);

  static void for_each_impl(QObject const* model, visit_function const& visit);
};

template <>
struct ext<QObject*> : ext<QObject const*>
{
};

template <typename T>
struct ext<T const*,
           typename std::enable_if<std::is_base_of<QObject, T>::value>::type>
  : ext<QObject const*>
{
};

template <typename T>
struct ext<T*,
           typename std::enable_if<std::is_base_of<QObject, T>::value>::type>
  : ext<QObject const*>
{
};

} // namespace byom

#endif /* BYOM_EXT_QOBJECT_HPP */
