cmake_minimum_required(VERSION 2.8)
project (csvdownloader)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#prevent generate extra debug/release folder when using Xcode
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
add_compile_options(-std=c++11)

#---------------------------- add_subdirectory ----------------------------
add_subdirectory(${PROJECT_SOURCE_DIR}/src/csvdownloader)


