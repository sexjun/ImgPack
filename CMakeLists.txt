cmake_minimum_required(VERSION 3.0.0)
project(ImaPack VERSION 0.1.0)

include(CTest)
enable_testing()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")  #设置c++的编译选项
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")  #设置c的编译选项

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)   # 库文件
conan_basic_setup()

include_directories(./include)  # 头文件


aux_source_directory(./src dir_src)

add_executable(ImaPack  ${dir_src})  # 代码文件

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
