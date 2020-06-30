cmake_minimum_required (VERSION 3.8)
message("test Label BEGIN")

include_directories(../utility/inc/)

file(GLOB ALL_SRC_TEST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp) # 获得源目录下的所有测试文件，放入ALL_SRC_TEST列表中

#message("---->[1] ./src BEGIN")
foreach(SRC_TEST ${ALL_SRC_TEST})
    STRING (REGEX REPLACE "^src\\/(.*)\\.cpp$" "\\1" TEST_NAME ${SRC_TEST})
    #message("------>[2] ./src/" ${TEST_NAME} " BEGIN")
    add_executable(src_${TEST_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_TEST})

    target_include_directories (src_${TEST_NAME} 
        PUBLIC ${Vulkan_INCLUDE_DIRS}
    )
    target_link_libraries(src_${TEST_NAME} PRIVATE utility)
    target_link_libraries(src_${TEST_NAME} PRIVATE imgui)
    target_link_libraries(src_${TEST_NAME} PRIVATE glfw imgui)
    target_link_libraries(src_${TEST_NAME} PRIVATE ${Vulkan_LIBRARIES})
    #target_link_libraries(src_${TEST_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
    #target_link_libraries(src_${TEST_NAME} utility)
    #message("------>[2] ./src/" ${TEST_NAME} " END")
endforeach()
#message("---->[1] ./src END")


#[[
file(GLOB ALL_TEST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*) # 获得当前目录下的所有文件，放入ALL_TEST列表中
foreach(SUB_TEST ${ALL_TEST})
    if ((IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${SUB_TEST}) # 当前路径是一个目录吗，是的话就加入到包含目录
    AND( NOT(${SUB_TEST} MATCHES ".*\\/\\..*") ) # 忽略.开头的隐藏文件
    AND( NOT(${SUB_TEST} MATCHES ".*\\/out\\/*.*") ) # 忽略输出目录的文件
    AND( NOT(${SUB_TEST} EQUAL ${CMAKE_CURRENT_SOURCE_DIR})) # 不包含根目录
    AND( NOT(${SUB_TEST} STREQUAL "src")) # 不包含src
    )
        #message("---->[1] ./" ${SUB_TEST} " BEGIN")
        include_sub_directories_recursively(${SUB_TEST} false) # 添加包含目录
        FILE(GLOB_RECURSE SOURCE_FILES  ${SUB_TEST}/src/*.cpp) # 获取当前目录源文件
        FILE(GLOB_RECURSE HEADER_FILES  ${SUB_TEST}/inc/*.h)   # 获取当前目录头文件
        add_executable(${SUB_TEST} ${SOURCE_FILES} ${HEADER_FILES})
        #target_link_libraries(${SUB_TEST} utility)
        #message("---->[1] ./" ${SUB_TEST} " END")
    endif()
endforeach()
]]

file(GLOB ALL_TEST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*) # 获得当前目录下的所有文件，放入ALL_TEST列表中
foreach(SUB_TEST ${ALL_TEST})
    if ((IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${SUB_TEST}) # 当前路径是一个目录吗，是的话就加入到包含目录
    AND( NOT(${SUB_TEST} MATCHES ".*\\/\\..*") ) # 忽略.开头的隐藏文件
    AND( NOT(${SUB_TEST} MATCHES ".*\\/out\\/*.*") ) # 忽略输出目录的文件
    AND( NOT(${SUB_TEST} EQUAL ${CMAKE_CURRENT_SOURCE_DIR})) # 不包含根目录
    AND( NOT(${SUB_TEST} STREQUAL "src")) # 不包含src
    )
    add_subdirectory(${SUB_TEST})
    endif()
endforeach()



message("test Label END")

