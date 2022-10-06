cmake_minimum_required(VERSION 3.24)
project(lru_cache)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-std=c++20")

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG 08935483cb22749b3c3774e9bcd5ef35b5a1a7b7
)
FetchContent_MakeAvailable(googletest)

include_directories(${PROJECT_SOURCE_DIR}/include)

enable_testing()

add_executable(
        cache_test
        test.cpp
)
target_link_libraries(
        cache_test
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(cache_test)
