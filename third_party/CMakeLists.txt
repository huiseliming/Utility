cmake_minimum_required (VERSION 3.8)

message("build imgui")



include_sub_directories_recursively(${CMAKE_CURRENT_SOURCE_DIR}/inc/imgui false) # 获取当前目录文件
FILE(GLOB_RECURSE SOURCE_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/inc//imgui*.cpp) # 获取当前目录源文件
FILE(GLOB_RECURSE HEADER_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/inc/imgui/*.h)   # 获取当前目录头文件
message(${SOURCE_FILES})
message(${HEADER_FILES})


add_library(imgui STATIC ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories (imgui
    PUBLIC ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(imgui PRIVATE glfw)
target_link_libraries(imgui PRIVATE ${Vulkan_LIBRARIES})