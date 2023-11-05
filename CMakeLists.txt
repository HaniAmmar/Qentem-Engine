cmake_minimum_required(VERSION 3.5)
project(Qentem_Engine CXX)
set(CMAKE_CXX_STANDARD 11)
include_directories("Include")
enable_testing()

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
    option(ENABLE_COVERAGE "Enable Coverage for gcc/clang" FALSE)
    option(ENABLE_AVX2 "Enable AVX2" FALSE)
    option(ENABLE_SSE2 "Enable SSE2" FALSE)

    if (ENABLE_COVERAGE)
        add_compile_options(--coverage -g -O0)
    endif()

    if (ENABLE_AVX2)
        add_compile_options(-D QENTEM_AVX2 -march=native)
    elseif(ENABLE_SSE2)
        add_compile_options(-D QENTEM_SSE2 -march=native)
    endif()
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

# Evaluate
add_executable(EvaluateTest Tests/EvaluateTest.cpp)
add_test(NAME EvaluateTest COMMAND EvaluateTest)

if (ENABLE_COVERAGE)
    target_link_libraries(EvaluateTest --coverage)
endif()

# Template
add_executable(TemplateTest Tests/TemplateTest.cpp)
add_test(NAME TemplateTest COMMAND TemplateTest)

if (ENABLE_COVERAGE)
    target_link_libraries(TemplateTest --coverage)
endif()

# Template (Wide character)
add_executable(TemplateLTest Tests/TemplateLTest.cpp)
add_test(NAME TemplateLTest COMMAND TemplateLTest)

if (ENABLE_COVERAGE)
    target_link_libraries(TemplateLTest --coverage)
 endif()

# Template (16bit character)
add_executable(TemplateUTest Tests/TemplateUTest.cpp)
add_test(NAME TemplateUTest COMMAND TemplateUTest)

if (ENABLE_COVERAGE)
    target_link_libraries(TemplateUTest --coverage)
 endif()
