cmake_minimum_required(VERSION 3.26)

# EXTERNAL DEPENDENCIES

add_subdirectory(external)

# CMAKE INCLUDES

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/external/cmake/)
include(CMakePrintHelpers)
include(cmtk/CppLibraryProject)

# CONFIGURATION

disable_in_source_build()
set_build_type_ifndef()

# C++ PROJECT

set_project_name(NAMESPACE "arba" FEATURE_NAME "seri")
string(TIMESTAMP configure_datetime "%Y%m%d-%H%M%S")
set_project_semantic_version("0.7.0" BUILD_METADATA "${configure_datetime}")

project(${PROJECT_NAME}
        VERSION ${PROJECT_VERSION}
        DESCRIPTION "A C++ library providing cross-platform serialization features."
        HOMEPAGE_URL "https://github.com/arapelle/arba-seri"
        LANGUAGES CXX)
message(STATUS "PROJECT : ${PROJECT_NAME} ${PROJECT_SEMANTIC_VERSION} ${CMAKE_BUILD_TYPE}")

# C++ LIBRARY

## Generated/Configured headers:
configure_headers(configured_headers
    FILES
        include/${PROJECT_NAMESPACE}/${PROJECT_FEATURE_NAME}/version.hpp.in
)

## Headers:
set(headers
    include/arba/seri/binary/binary_serializable.hpp
    include/arba/seri/binary/io.hpp
    include/arba/seri/binary/read_binary.hpp
    include/arba/seri/binary/write_binary.hpp
    include/arba/seri/polymorphism.hpp
)

## Sources:
set(sources
    src/arba/seri/binary/read_binary.cpp
    src/arba/seri/binary/write_binary.cpp
)

## Add C++ library:
cxx_standard_option(${PROJECT_UPPER_VAR_NAME}_CXX_STANDARD MIN 20 MAX 26)

shared_or_static_option(${PROJECT_UPPER_VAR_NAME}_LIBRARY_TYPE "SHARED")
if("${${PROJECT_UPPER_VAR_NAME}_LIBRARY_TYPE}" STREQUAL "SHARED")
  set(LIBRARY_TYPE_POSTFIX "")
  set(PROJECT_TARGET_NAME "${PROJECT_NAME}")
else()
  set(LIBRARY_TYPE_POSTFIX "-static")
  set(PROJECT_TARGET_NAME "${PROJECT_NAME}${LIBRARY_TYPE_POSTFIX}")
endif()

add_cpp_library(${PROJECT_TARGET_NAME} ${${PROJECT_UPPER_VAR_NAME}_LIBRARY_TYPE}
  HEADERS ${headers} ${configured_headers}
  SOURCES ${sources}
  CXX_STANDARD ${${PROJECT_UPPER_VAR_NAME}_CXX_STANDARD}
  DEFAULT_WARNING_OPTIONS
  DEFAULT_ERROR_OPTIONS
)
add_library("${PROJECT_NAMESPACE}::${PROJECT_FEATURE_NAME}${LIBRARY_TYPE_POSTFIX}" ALIAS ${PROJECT_TARGET_NAME})

## Link C++ targets:
find_package(arba-core 0.30.0 REQUIRED CONFIG)
find_package(arba-uuid 0.3.0 REQUIRED CONFIG)
target_link_libraries(${PROJECT_TARGET_NAME}
    PUBLIC
        arba::core
        arba::uuid
)

## Add tests:
add_test_subdirectory_if_build(test)

## Add examples:
add_example_subdirectory_if_build(example)

# C++ INSTALL

## Install C++ library:
install_cpp_libraries(TARGETS ${PROJECT_TARGET_NAME} EXPORT ${PROJECT_NAME}-targets)

## Install project package
install_library_package(${PROJECT_NAME} INPUT_PACKAGE_CONFIG_FILE cmake/config/package-config.cmake.in)
install_uninstall_script(${PROJECT_NAME})
