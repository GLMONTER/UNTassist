cmake_minimum_required(VERSION 3.23)
project(linuxVER)

set(CMAKE_CXX_STANDARD 14)

find_package(X11 REQUIRED)

include_directories(${X11_INCLUDE_DIR})
add_executable(linuxVER main.cpp)

link_directories(${X11_LIBRARIES})
target_link_libraries(linuxVER ${X11_LIBRARIES} ${X11_Xtst_LIB})


