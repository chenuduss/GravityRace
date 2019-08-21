project(GravityRaceServer)
cmake_minimum_required(VERSION 2.8)
add_definitions(-std=c++11)


aux_source_directory(. SRC_LIST)
aux_source_directory(./json SRC_LIST)
aux_source_directory(./zlib SRC_LIST)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(BOOST_COMPONENTS filesystem system date_time chrono thread regex coroutine context)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

find_package(Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(. ${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} )

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} "/usr/lib/x86_64-linux-gnu/libev.so")


