cmake_minimum_required(VERSION 3.10)
project(RedisLite)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_library(CustomLibrary
    src/customLibrery/dynamicArray.cpp
    src/customLibrery/doublyLinkedList.cpp
    src/customLibrery/linkedList.cpp
    src/customLibrery/hashFunction.cpp
    src/customLibrery/hashMap.cpp
    src/customLibrery/redisLite.cpp
    src/customLibrery/commandParser.cpp
)
target_include_directories(CustomLibrary PUBLIC include)

add_executable(RedisLite main.cpp)
target_link_libraries(RedisLite CustomLibrary)

# 1. Turn on testing in CMake
enable_testing()

# 2. Tell CMake to download Google Test directly from GitHub
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# (Windows specific fix to prevent weird compiler conflicts)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# 3. Download and make Google Test available to your project
FetchContent_MakeAvailable(googletest)

# 4. Create separate test executables for each data structure
add_executable(DynamicArrayTests
    test/DynamicArrayTests.cpp
)
add_executable(LinkedListTests
    test/LinkedListTests.cpp
)
add_executable(HashMapTests
    test/HashMapTests.cpp
)

# 5. Link Google Test and CustomLibrary to each test executable
target_link_libraries(DynamicArrayTests GTest::gtest_main CustomLibrary)
target_link_libraries(LinkedListTests   GTest::gtest_main CustomLibrary)
target_link_libraries(HashMapTests      GTest::gtest_main CustomLibrary)

# 6. Allow CMake/CTest to discover all tests
include(GoogleTest)
gtest_discover_tests(DynamicArrayTests)
gtest_discover_tests(LinkedListTests)
gtest_discover_tests(HashMapTests)
