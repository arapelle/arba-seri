add_cpp_library_tests(SHARED ${PROJECT_NAME}
                      STATIC ${PROJECT_NAME}-static
                      SOURCES 
                          binary_io_tests.cpp
                          polymorphism_tests.cpp
                      )
