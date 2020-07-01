# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Utility")

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

message(STATUS "compiler: " ${CMAKE_CXX_COMPILER_ID})

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # MSVC链接静态库(/MT),替换/MD为/MT来进行链接静态库,使用RUNTIME_STATIC_LINK控制链接静态库还是动态库
    if(MSVC AND RUNTIME_STATIC_LINK)
        foreach(flag
            CMAKE_C_FLAGS
            CMAKE_CXX_FLAGS
            CMAKE_C_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_C_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_C_FLAGS_MINSIZEREL
            CMAKE_CXX_FLAGS_MINSIZEREL
            CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS_RELWITHDEBINFO
        )
            if(${flag} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
            endif()
        endforeach()
    endif()
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-Wformat=0)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message( FATAL_ERROR "未配置Clang")
endif()

find_package(GTest CONFIG REQUIRED)

add_subdirectory("utility")