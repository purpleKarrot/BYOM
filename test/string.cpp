#include <byom/dynamic_view.hpp>
#include <byom/adapted/string.hpp>
#include <byom/adapted/ostream.hpp>

#include <boost/core/lightweight_test.hpp>
#include <boost/lexical_cast.hpp>
#include <string>

int string(int argc, char* argv[])
{
  auto const model = std::string{ "John Doe" };
  auto const view = byom::dynamic_view{ model };

  BOOST_TEST(!view.empty());

  BOOST_TEST_EQ(boost::lexical_cast<std::string>(view), "John Doe");

  BOOST_TEST_THROWS(view.at("member"), std::invalid_argument);

  auto const visit = [](auto const&, auto const&) {};
  BOOST_TEST_THROWS(view.for_each(visit), std::invalid_argument);

  return boost::report_errors();
}
