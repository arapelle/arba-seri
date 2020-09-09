cmake_minimum_required(VERSION 3.16)

project(basic_cmake_project)

add_executable(${PROJECT_NAME} main.cpp)
# Find package seri:
find_package(seri 0.1.1 CONFIG REQUIRED)
# Use seri release shared target:
target_link_libraries(${PROJECT_NAME} PRIVATE arba::seri)
# Use seri release static target:
#target_link_libraries(${PROJECT_NAME} PRIVATE arba::seri-static)
