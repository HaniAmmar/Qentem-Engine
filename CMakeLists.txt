cmake_minimum_required(VERSION 3.0)
project(Qentem_Engine CXX)
set(CMAKE_CXX_STANDARD 11)
include_directories("Include")
enable_testing()

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -march=native)
    option(ENABLE_COVERAGE "Enable Coverage for gcc/clang" FALSE)

    if (ENABLE_COVERAGE)
        add_compile_options(--coverage -g -O0)
    endif()
endif()

# TestHelper
add_executable(TestHelperTest Tests/TestHelper.cpp)
add_test(NAME TestHelperTest COMMAND TestHelperTest)

if (ENABLE_COVERAGE)
    target_link_libraries(TestHelperTest --coverage)
endif()

# Memory
add_executable(MemoryTest Tests/MemoryTest.cpp)
add_test(NAME MemoryTest COMMAND MemoryTest)

if (ENABLE_COVERAGE)
    target_link_libraries(MemoryTest --coverage)
endif()

# StringUtils
add_executable(StringUtilsTest Tests/StringUtilsTest.cpp)
add_test(NAME StringUtilsTest COMMAND StringUtilsTest)

if (ENABLE_COVERAGE)
    target_link_libraries(StringUtilsTest --coverage)
endif()

# String
add_executable(StringTest Tests/StringTest.cpp)
add_test(NAME StringTest COMMAND StringTest)

if (ENABLE_COVERAGE)
    target_link_libraries(StringTest --coverage)
endif()

# StringStream
add_executable(StringStreamTest Tests/StringStreamTest.cpp)
add_test(NAME StringStreamTest COMMAND StringStreamTest)

if (ENABLE_COVERAGE)
    target_link_libraries(StringStreamTest --coverage)
endif()

# FixedArray
add_executable(FixedArrayTest Tests/FixedArrayTest.cpp)
add_test(NAME FixedArrayTest COMMAND FixedArrayTest)

if (ENABLE_COVERAGE)
    target_link_libraries(FixedArrayTest --coverage)
endif()

# Array
add_executable(ArrayTest Tests/ArrayTest.cpp)
add_test(NAME ArrayTest COMMAND ArrayTest)

if (ENABLE_COVERAGE)
    target_link_libraries(ArrayTest --coverage)
endif()

# Digit
add_executable(DigitTest Tests/DigitTest.cpp)
add_test(NAME DigitTest COMMAND DigitTest)

if (ENABLE_COVERAGE)
    target_link_libraries(DigitTest --coverage)
endif()

# HArray
add_executable(HArrayTest Tests/HArrayTest.cpp)
add_test(NAME HArrayTest COMMAND HArrayTest)

if (ENABLE_COVERAGE)
    target_link_libraries(HArrayTest --coverage)
endif()

# Engine
add_executable(EngineTest Tests/EngineTest.cpp)
add_test(NAME EngineTest COMMAND EngineTest)

if (ENABLE_COVERAGE)
    target_link_libraries(EngineTest --coverage)
endif()

# ALE
add_executable(ALETest Tests/ALETest.cpp)
add_test(NAME ALETest COMMAND ALETest)

if (ENABLE_COVERAGE)
    target_link_libraries(ALETest --coverage)
endif()

# Unicode
add_executable(UnicodeTest Tests/UnicodeTest.cpp)
add_test(NAME UnicodeTest COMMAND UnicodeTest)

if (ENABLE_COVERAGE)
    target_link_libraries(UnicodeTest --coverage)
endif()

# JSONUtils
add_executable(JSONUtilsTest Tests/JSONUtilsTest.cpp)
add_test(NAME JSONUtilsTest COMMAND JSONUtilsTest)

if (ENABLE_COVERAGE)
    target_link_libraries(JSONUtilsTest --coverage)
endif()

# Value
add_executable(ValueTest Tests/ValueTest.cpp)
add_test(NAME ValueTest COMMAND ValueTest)

if (ENABLE_COVERAGE)
    target_link_libraries(ValueTest --coverage)
endif()

# JSON
add_executable(JSONTest Tests/JSONTest.cpp)
add_test(NAME JSONTest COMMAND JSONTest)

if (ENABLE_COVERAGE)
    target_link_libraries(JSONTest --coverage)
endif()

# Template
add_executable(TemplateTest Tests/TemplateTest.cpp)
add_test(NAME TemplateTest COMMAND TemplateTest)

if (ENABLE_COVERAGE)
    target_link_libraries(TemplateTest --coverage)
endif()
