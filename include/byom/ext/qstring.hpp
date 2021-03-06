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

#ifndef BYOM_EXT_QSTRING_HPP
#define BYOM_EXT_QSTRING_HPP

#include <byom/ext/fallback.hpp>
#include <QString>
#include <ostream>

namespace byom {

template <>
struct ext<QString> : detail::fallback
{
  static bool empty_impl(QString const& model)
  {
    return model.isEmpty();
  }

  static unsigned long long int to_integral(QString const& model)
  {
    return model.toULongLong();
  }

  static long double to_floating_point(QString const& model)
  {
    return model.toDouble();
  }

  static std::string to_string(QString const& model)
  {
    return model.toStdString();
  }

  static void print_impl(std::ostream& os, QString const& model)
  {
    os << qPrintable(model);
  }
};

} // namespace byom

#endif /* BYOM_EXT_QSTRING_HPP */
