cmake_minimum_required(VERSION 3.16)

project(gtest)

add_compile_options(-pedantic -Werror -Wextra -fsanitize=address -fsanitize=undefined -std=c++20)
add_link_options(-pedantic -Werror -Wextra -fsanitize=address -fsanitize=undefined -std=c++20)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
#set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)
add_executable(gtest tests.cpp)
enable_testing()

add_test(NAME gtest COMMAND ${Testing_SOURCE_DIR}/bin/testing)

target_link_libraries(gtest Threads::Threads ${GTEST_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})

project(loctest)
add_executable(loctest main.cpp)
