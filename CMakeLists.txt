cmake_minimum_required(VERSION 3.16)

# CMAKE INCLUDES

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(CMakePrintHelpers)
include(cmtk/CppLibraryProject)

# CONFIGURATION

disable_in_source_build()
set_build_type_ifndef()

# C++ PROJECT

set(PROJECT_NAME "arba-seri")
set(project_namespace "arba")
set(project_feature_name "seri")
set(PROJECT_VERSION 0.4.0)
project(${PROJECT_NAME}
        VERSION ${PROJECT_VERSION}
        DESCRIPTION "Portable serialization C++ library."
        HOMEPAGE_URL "https://github.com/arapelle/seri"
        LANGUAGES CXX)
message(STATUS "PROJECT : ${PROJECT_NAME} ${PROJECT_VERSION} ${CMAKE_BUILD_TYPE}")
include(CTest)

# C++ LIBRARY

## Generated/Configured headers:
generate_version_macro_header(generated_version_header
                              ${project_namespace}_${project_feature_name}
                              ${project_namespace}/${project_feature_name}/version.hpp)

## Headers:
set(headers
include/seri/serializable_object.hpp
include/seri/polymorphism.hpp
include/seri/binary/io.hpp
include/seri/binary/read_binary.hpp
include/seri/binary/write_binary.hpp
)

## Sources:
set(sources
src/binary/read_binary.cpp
src/binary/write_binary.cpp
)

## Add C++ library:
add_cpp_library(
  SHARED ${PROJECT_NAME}
  STATIC ${PROJECT_NAME}-static
  BUILD_SHARED OPTION
  BUILD_STATIC OPTION
  HEADERS ${headers} ${generated_version_header}
  SOURCES ${sources}
  CXX_STANDARD 20
)

## Link C++ targets:
if(NOT TARGET arba-core)
    find_package(arba-core 0.14 REQUIRED CONFIG)
endif()
cpp_library_link_libraries(
  SHARED ${PROJECT_NAME}
  STATIC ${PROJECT_NAME}-static
  PUBLIC arba-core
)

## Install C++ library:
install_cpp_library(
  SHARED ${PROJECT_NAME}
  STATIC ${PROJECT_NAME}-static
  EXPORT ${PROJECT_NAME}-targets
)

## Install project package
install_library_package(${PROJECT_NAME}
                        INPUT_PACKAGE_CONFIG_FILE CMake-package-config.cmake.in
)
install_uninstall_script(${PROJECT_NAME})

## Add examples:
option_or_set_ifdef(BUILD_${PROJECT_NAME}_EXAMPLES "Build ${PROJECT_NAME} examples or not" OFF BUILD_arba-arba_EXAMPLES)
if(BUILD_${PROJECT_NAME}_EXAMPLES)
    add_subdirectory(example)
endif()

## Add tests:
option_or_set_ifdef(BUILD_${PROJECT_NAME}_TESTS "Build ${PROJECT_NAME} tests or not" OFF BUILD_arba-arba_TESTS)
option(BUILD_${PROJECT_NAME}_TESTS "Build ${PROJECT_NAME} tests or not" OFF)
if(BUILD_${PROJECT_NAME}_TESTS AND BUILD_TESTING)
    add_subdirectory(test)
endif()
