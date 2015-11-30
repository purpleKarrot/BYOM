#include <byom/dynamic_view.hpp>
#include <byom/adapted/fusion.hpp>
#include <byom/adapted/empty.hpp>
#include <byom/adapted/ostream.hpp>

#include <boost/fusion/adapted/struct.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/lexical_cast.hpp>
#include <map>

struct Person
{
  std::string name;
  int age;
};

BOOST_FUSION_ADAPT_STRUCT(Person, name, age)

int fusion(int argc, char* argv[])
{
  auto const model = Person{ "John Doe", 42 };
  auto const view = byom::dynamic_view{ model };

  BOOST_TEST(!view.empty());

  BOOST_TEST_THROWS(boost::lexical_cast<std::string>(view),
                    std::invalid_argument);

  auto const name = view.at("name");
  BOOST_TEST_EQ(boost::lexical_cast<std::string>(name), "John Doe");

  auto const age = view.at("age");
  BOOST_TEST_EQ(boost::lexical_cast<std::string>(age), "42");

  BOOST_TEST_THROWS(view.at("bar"), std::out_of_range);

  int counter = 0;
  view.for_each([&](auto const& key, auto const& val) {
    switch (++counter) {
      case 1:
        BOOST_TEST_EQ(boost::lexical_cast<std::string>(key), "name");
        BOOST_TEST_EQ(boost::lexical_cast<std::string>(val), "John Doe");
        break;
      case 2:
        BOOST_TEST_EQ(boost::lexical_cast<std::string>(key), "age");
        BOOST_TEST_EQ(boost::lexical_cast<std::string>(val), "42");
        break;
      default:
        BOOST_ERROR("Should not be reached.");
        break;
    }
  });
  BOOST_TEST_EQ(counter, 2);

  return boost::report_errors();
}
