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

#include <byom/dynamic_view.hpp>
#include <byom/ext/arithmetic.hpp>
#include <byom/ext/nullptr.hpp>
#include <byom/ext/string.hpp>
#include <byom/ext/qlist.hpp>

#include <boost/core/lightweight_test.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

int qlist(int argc, char* argv[])
{
  auto const model = QList<std::string>{ "foo", "bar" };
  auto const view = byom::dynamic_view{ model };

  BOOST_TEST(!view.empty());

  BOOST_TEST_THROWS(boost::lexical_cast<std::string>(view),
                    std::invalid_argument);

  BOOST_TEST_THROWS(view.at("member"), std::invalid_argument);

  int counter = 0;
  view.for_each([&](auto const& index, auto const& elem) {
    switch (++counter) {
      case 1:
        BOOST_TEST_EQ(boost::lexical_cast<std::string>(index), "0");
        BOOST_TEST_EQ(boost::lexical_cast<std::string>(elem), "foo");
        break;
      case 2:
        BOOST_TEST_EQ(boost::lexical_cast<std::string>(index), "1");
        BOOST_TEST_EQ(boost::lexical_cast<std::string>(elem), "bar");
        break;
      default:
        BOOST_ERROR("Should not be reached.");
        break;
    }
  });
  BOOST_TEST_EQ(counter, 2);

  return boost::report_errors();
}
