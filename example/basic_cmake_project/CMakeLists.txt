cmake_minimum_required(VERSION 3.26)

project(basic_cmake_project)

add_executable(${PROJECT_NAME} main.cpp)
add_executable(${PROJECT_NAME}-static main.cpp)
# Find package arba-seri:
find_package(arba-seri 0.4.0 CONFIG REQUIRED)
# Use arba-seri release shared target:
target_link_libraries(${PROJECT_NAME} PRIVATE arba-seri)
# Use arba-seri release static target:
target_link_libraries(${PROJECT_NAME}-static PRIVATE arba-seri-static)
