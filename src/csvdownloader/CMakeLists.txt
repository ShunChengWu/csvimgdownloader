SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

find_package(CURL REQUIRED)
find_package(OpenCV)
 
set(CSVD_LIBS)
list(APPEND CSVD_LIBS ${CURL_LIBRARIES})

if(OpenCV_LIBS)
    list(APPEND CSVD_LIBS ${OpenCV_LIBS})
    add_definitions(-DWITH_OPENCV)
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()

#file(GLOB SRC kdtree/*.c kdtree/*.cpp kdtree/*.h kdtree/*.hpp)
file(GLOB HEADER_SOURCES *.h *.hpp)
file(GLOB BIN_SOURCES *.cpp *.cc)
foreach(filename ${BIN_SOURCES})
  get_filename_component(name ${filename} NAME_WE)
  add_executable(${name} ${filename} ${HEADER_SOURCES} ${SRC})
  target_link_libraries(${name}
    ${CSVD_LIBS}
)

  if(APPLE)
    set_property(TARGET ${name} PROPERTY BUILD_RPATH ${CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES})
  endif()
endforeach()
