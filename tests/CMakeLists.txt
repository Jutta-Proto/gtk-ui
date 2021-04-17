cmake_minimum_required(VERSION 3.16)

include(Catch)

add_executable(tests Tests.cpp
                     TestFixedSizeStack.cpp)

set_target_properties(tests PROPERTIES UNITY_BUILD OFF)
target_link_libraries(tests PRIVATE Catch2::Catch2 backend)

catch_discover_tests(tests)
