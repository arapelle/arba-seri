
find_or_fetch_GTest()

add_cpp_library_basic_tests(GTest::gtest_main
    SHARED ${PROJECT_NAME}
    STATIC ${PROJECT_NAME}-static
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
