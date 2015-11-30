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

#ifndef BYOM_EXT_QVARIANT_HPP
#define BYOM_EXT_QVARIANT_HPP

#include <byom/dynamic_view.hpp>
#include <QVariant>

namespace byom {

template <>
struct ext<QVariant>
{
  static bool empty_impl(QVariant const& model)
  {
    return model.isNull();
  }

  static dynamic_view at_impl(QVariant const& model, std::string const& name)
  {
    throw std::invalid_argument{ "'at' needs rvalue support" };
    //if (model.canConvert<QVariantHash>() ||
    //model.canConvert<QVariantMap>()) {
    //  auto iterable = model.value<QAssociativeIterable>();
    //  for (auto i = iterable.begin(); i != iterable.end(); ++i) {
    //    if (i.key() == QString::fromStdString(name)) {
    //      return i.value();
    //    }
    //  }
    //  throw std::out_of_range{ "out of range" };
    //} else {
    //}
  }

  static void for_each_impl(QVariant const& model, visit_function const& visit)
  {
    if (model.canConvert<QVariantHash>() || model.canConvert<QVariantMap>()) {
      auto iterable = model.value<QAssociativeIterable>();
      for (auto i = iterable.begin(); i != iterable.end(); ++i) {
        auto const key = i.key();
        auto const value = i.value();
        visit(key, value);
      }
    } else if (model.canConvert<QVariantList>()) {
      std::size_t index = 0;
      for (auto const& elem : model.value<QSequentialIterable>()) {
        visit(index, elem);
        index += 1;
      }
    } else {
      throw std::invalid_argument{ "for_each not fully implemented" };
    }
  }

  static void print_impl(std::ostream& os, QVariant const& model)
  {
    if (model.canConvert<QString>()) {
      os << qPrintable(model.toString());
    } else {
      throw std::invalid_argument{ "print not fully implemented" };
    }
  }
};

} // namespace byom

#endif /* BYOM_EXT_QVARIANT_HPP */
