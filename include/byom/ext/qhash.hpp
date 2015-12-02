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

#ifndef BYOM_EXT_QHASH_HPP
#define BYOM_EXT_QHASH_HPP

#include <byom/ext/fallback.hpp>
#include <byom/ext/qstring.hpp>
#include <QHash>

namespace byom {

template <typename Value>
struct ext<QHash<QString, Value>> : detail::fallback
{
  using model_t = QHash<QString, Value>;

  static bool empty_impl(model_t const& model)
  {
    return model.isEmpty();
  }

  static Value const& at_impl(model_t const& model, std::string const& name)
  {
    auto const it = model.find(QString::fromStdString(name));
    if (it == model.end()) {
      throw std::out_of_range{ "out of range" };
    }
    return it.value();
  }

  static void for_each_impl(model_t const& model, visit_function const& visit)
  {
    for (auto i = model.begin(); i != model.end(); ++i) {
      visit(i.key(), i.value());
    }
  }
};

} // namespace byom

#endif /* BYOM_EXT_QHASH_HPP */
