cmake_minimum_required (VERSION 3.8)

message("DrogonWeb")

#bmessage(${DROGON_INCLUDE_DIRS})
#include_directories(${DROGON_INCLUDE_DIRS})
#target_include_directories();

FILE(GLOB_RECURSE SOURCE_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cc
    )
	 
message(${SOURCE_FILES})

add_executable(main ${SOURCE_FILES})

target_link_libraries(main PRIVATE
	Drogon::Drogon 
	pg_lib 
	#UUID_lib 
	#coz::coz 
	MySQL_lib
	jsoncpp_lib
	OpenSSL::SSL
	OpenSSL::Crypto
	spdlog::spdlog 
	spdlog::spdlog_header_only
	)