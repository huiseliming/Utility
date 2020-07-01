cmake_minimum_required(VERSION 3.12)
set(This UtilityTests)

FILE(GLOB_RECURSE SourceFiles  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp) #获取当前目录源文件

add_executable(${This} ${SourceFiles})

set_target_properties(${This} PROPERTIES
    FOLDER Tests
)

target_include_directories(${This} PRIVATE ..)

target_link_libraries(${This} PUBLIC
    utility 
    GTest::gtest_main 
)

add_test(
    NAME ${This}
    COMMAND ${This}
)
