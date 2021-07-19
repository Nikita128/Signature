cmake_minimum_required(VERSION 3.18)

project(Signature)

##---------- Compiller options-------------##
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra")

set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -fno-inline -D_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_C_FLAGS "-Wall -Wextra -std=gnu99")

set(CMAKE_C_FLAGS_DEBUG "-g3 -O0 -fno-inline -D_DEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3")

##----------------Threads------------------##
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

##-----------------Boost-------------------##
set(BOOST_COMPONENTS system iostreams thread)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.75 COMPONENTS ${BOOST_COMPONENTS} REQUIRED)

##-----------------------------------------##
set(SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/thread_pool.cpp
    ${CMAKE_SOURCE_DIR}/signature_maker.cpp
    ${CMAKE_SOURCE_DIR}/task_queue.cpp
    ${CMAKE_SOURCE_DIR}/signature_thread.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    stdc++fs
)
