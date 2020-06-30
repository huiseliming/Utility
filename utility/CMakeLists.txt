# CMakeList.txt : CMake project for Utility, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
message("build Utility Begin")

include_sub_directories_recursively(${CMAKE_CURRENT_SOURCE_DIR} false) # 获取当前目录文件
FILE(GLOB_RECURSE SOURCE_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp) # 获取当前目录源文件
FILE(GLOB_RECURSE HEADER_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h)   # 获取当前目录头文件

message(${SOURCE_FILES})
message(${HEADER_FILES})

# Add source to this pro"src/Utility.cpp"able.
add_library(utility STATIC ${SOURCE_FILES} ${HEADER_FILES} "src/ThreadPool.cpp")
# TODO: Add tests and install targets if needed.


message("build Utility End")