# Copyright (c) 2014, Daniel Pfeifer <daniel@pfeifer-mail.de>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

set(test_list
  builtin
  fusion
  map
  string
  vector
  )

create_test_sourcelist(test_sources
  main.cpp
  ${test_list}
  )

add_executable(byom_test_driver ${test_sources})
target_link_libraries(byom_test_driver PRIVATE byom)
target_compile_features(byom_test_driver PRIVATE
  cxx_generic_lambdas
  )

foreach(test IN LISTS test_list)
  add_test(NAME byom.${test}
    COMMAND $<TARGET_FILE:byom_test_driver> ${test}
    )
endforeach()
