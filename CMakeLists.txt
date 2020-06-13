# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Utility")

#glfw
find_package(glfw3 CONFIG REQUIRED)
#Vulkan
find_package(Vulkan REQUIRED FATAL_ERROR)
#spdlog
find_package(spdlog CONFIG REQUIRED)

#添加包含目录
function(include_sub_directories_recursively root_dir inc_self)
    if ((IS_DIRECTORY ${root_dir}) # 当前路径是一个目录吗，是的话就加入到包含目录
    AND( NOT(${root_dir} MATCHES ".*\\/\\..*") ) # 忽略.开头的隐藏文件
    AND( NOT(${root_dir} MATCHES ".*\\/out\\/*.*") ) # 忽略输出目录的文件
    AND( inc_self AND NOT (${root_dir} EQUAL ${CMAKE_CURRENT_SOURCE_DIR})) # 是否包含根遍历目录
    )
        message("include dir: " ${root_dir})
        include_directories(${root_dir})
    endif()

    file(GLOB ALL_SUB RELATIVE ${root_dir} ${root_dir}/*) # 获得当前目录下的所有文件，放入ALL_SUB列表中
    foreach(sub ${ALL_SUB})
        if (IS_DIRECTORY ${root_dir}/${sub})
            include_sub_directories_recursively(${root_dir}/${sub} inc_self) # 递归子目录
        endif()
    endforeach()
endfunction()

function(include_sub_directories root_dir inc_self)
    file(GLOB ALL_SUB RELATIVE ${root_dir} ${root_dir}/*) # 获得当前目录下的所有文件，放入ALL_SUB列表中
    foreach(SUB_DIR ${ALL_SUB})
        if ((IS_DIRECTORY ${SUB_DIR}) # 当前路径是一个目录吗，是的话就加入到包含目录
        AND( NOT(${SUB_DIR} MATCHES ".*\\/\\..*") ) # 忽略.开头的隐藏文件
        AND( NOT(${SUB_DIR} MATCHES ".*\\/out\\/*.*") ) # 忽略输出目录的文件
        AND( inc_self AND NOT (${SUB_DIR} EQUAL ${CMAKE_CURRENT_SOURCE_DIR})) # 是否包含根目录
        )
            message("include dir: " ${SUB_DIR})
            include_directories(${SUB_DIR})
        endif()
    endforeach()
endfunction()

# Include sub-projects.

include_directories(third_party/inc/)

add_subdirectory("third_party")
add_subdirectory("utility")
add_subdirectory("test")