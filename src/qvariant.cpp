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

#include <byom/ext/qvariant.hpp>
#include <byom/ext/qhash.hpp>
#include <byom/ext/qmap.hpp>
#include <byom/ext/arithmetic.hpp>
#include <byom/ext/nullptr.hpp>

namespace byom {

dynamic_view ext<QVariant>::at_impl(QVariant const& model,
                                    std::string const& name)
{
  if (model.canConvert<QVariantHash>()) {
    auto const iterable = model.value<QAssociativeIterable>();
    auto const it = iterable.find(QString::fromStdString(name));
    if (it == iterable.end()) {
      throw std::out_of_range{ "out of range" };
    }
    return it.value();
  } else {
    throw std::invalid_argument{ "at not fully implemented" };
  }
}

void ext<QVariant>::for_each_impl(QVariant const& model,
                                  visit_function const& visit)
{
  if (model.canConvert<QVariantHash>()) {
    auto iterable = model.value<QAssociativeIterable>();
    for (auto i = iterable.begin(); i != iterable.end(); ++i) {
      visit(i.key(), i.value());
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

void ext<QVariant>::print_impl(std::ostream& os, QVariant const& model)
{
  if (model.canConvert<QString>()) {
    os << qPrintable(model.toString());
  } else {
    throw std::invalid_argument{ "print not fully implemented" };
  }
}

} // namespace byom
