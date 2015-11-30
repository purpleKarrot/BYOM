#include <byom/dynamic_view.hpp>
#include <byom/adapted/builtin.hpp>
#include <byom/adapted/ostream.hpp>

#include <boost/core/lightweight_test.hpp>
#include <boost/lexical_cast.hpp>

int builtin(int argc, char* argv[])
{
  auto const model = 42;
  auto const view = byom::dynamic_view{ model };

  BOOST_TEST(!view.empty());

  BOOST_TEST_EQ(boost::lexical_cast<std::string>(view), "42");

  BOOST_TEST_THROWS(view.at("member"), std::invalid_argument);

  auto const visit = [](auto const&, auto const&) {};
  BOOST_TEST_THROWS(view.for_each(visit), std::invalid_argument);

  return boost::report_errors();
}
