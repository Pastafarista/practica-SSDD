cmake_minimum_required (VERSION 2.8.11)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "-static")

project(clientMatrix LANGUAGES CXX)
add_executable(clientMatrix ./main_multMatrix.cpp ./clientMatrix.h ./clientMatrix.cpp ./utils.h ./utils.cpp ./operacionesMatrix.h)
target_link_libraries(clientMatrix pthread)

project(serverMatrix LANGUAGES CXX)
add_executable(serverMatrix ./serverMatrix.cpp ./multmatrix.h ./multmatrix.cpp ./utils.h ./utils.cpp ./operacionesMatrix.h)
target_link_libraries(serverMatrix pthread)