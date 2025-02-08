include(cmtk/CppLibraryTests)
include(GoogleTest)

find_package(GTest 1.14 CONFIG REQUIRED)

add_cpp_library_basic_tests(${PROJECT_TARGET_NAME} GTest::gtest_main
    SOURCES
        project_version_tests.cpp
        binary_polymorphic_tests.cpp
        binary_signed_integral_tests.cpp
        binary_unsigned_integral_tests.cpp
        binary_real_tests.cpp
        binary_string_tests.cpp
        binary_range_tests.cpp
        binary_uuid_tests.cpp
        polymorphism_tests.cpp
)
