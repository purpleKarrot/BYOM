#include <byom/dynamic_view.hpp>
#include <byom/adapted/map.hpp>
#include <byom/adapted/empty.hpp>
#include <byom/adapted/ostream.hpp>

#include <boost/core/lightweight_test.hpp>
#include <boost/lexical_cast.hpp>
#include <map>

int map(int argc, char* argv[])
{
  auto const model = std::map<std::string, std::string>{ { "name", "John Doe" },
                                                         { "age", "42" } };
  auto const view = byom::dynamic_view{ model };

  BOOST_TEST(!view.empty());

  BOOST_TEST_THROWS(boost::lexical_cast<std::string>(view),
                    std::invalid_argument);

  auto const name = view.at("name");
  BOOST_TEST_EQ(boost::lexical_cast<std::string>(name), "John Doe");

  auto const age = view.at("age");
  BOOST_TEST_EQ(boost::lexical_cast<std::string>(age), "42");

  BOOST_TEST_THROWS(view.at("bar"), std::out_of_range);

  int called = 0;
  view.for_each([&](auto const& key, auto const& val) {
    called += 1;
    BOOST_TEST((boost::lexical_cast<std::string>(key) == "name" &&
                boost::lexical_cast<std::string>(val) == "John Doe") ||
               (boost::lexical_cast<std::string>(key) == "age" &&
                boost::lexical_cast<std::string>(val) == "42"));
  });
  BOOST_TEST_EQ(called, 2);

  return boost::report_errors();
}
