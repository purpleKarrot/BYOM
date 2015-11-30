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

#ifndef BYOM_EXT_FUSION_HPP
#define BYOM_EXT_FUSION_HPP

#include <boost/fusion/adapted/struct.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>

namespace byom {
namespace detail {

template <int N>
struct at_impl
{
  template <typename T>
  static dynamic_view call(T const& object, std::string const& name)
  {
    using boost::fusion::extension::struct_member_name;
    if (struct_member_name<T, N - 1>::call() == name) {
      return boost::fusion::at_c<N - 1>(object);
    }
    return at_impl<N - 1>::call(object, name);
  }
};

template <>
struct at_impl<0>
{
  template <typename T>
  static dynamic_view call(T const&, std::string const& name)
  {
    throw std::out_of_range{ "No member " + name };
  }
};

} // namespace detail

template <typename Seq>
struct ext<Seq, typename std::enable_if<
                  boost::fusion::traits::is_sequence<Seq>::value>::type>
  : detail::fallback
{
  static bool empty_impl(Seq const& model)
  {
    return boost::fusion::result_of::size<Seq>::value == 0;
  }

  static dynamic_view at_impl(Seq const& model, std::string const& name)
  {
    using namespace boost::fusion;
    return detail::at_impl<result_of::size<Seq>::value>::call(model, name);
  }

  static void for_each_impl(Seq const& model, visit_function const& visit)
  {
    static auto constexpr size = boost::fusion::result_of::size<Seq>::value;
    boost::mpl::for_each<boost::mpl::range_c<int, 0, size>>([&](auto i) {
      using boost::fusion::extension::struct_member_name;
      auto const name = struct_member_name<Seq, i>::call();
      visit(name, boost::fusion::at_c<i>(model));
    });
  }
};

} // namespace byom

#endif /* BYOM_EXT_FUSION_HPP */
