cmake_minimum_required(VERSION 3.18)

project(Signature)

set(CMAKE_CXX_STANDARD 17)

##----------------Threads------------------##
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

##-----------------Boost-------------------##
set(BOOST_COMPONENTS system filesystem iostreams thread)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.75 COMPONENTS ${BOOST_COMPONENTS} REQUIRED)

##-----------------------------------------##
set(SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/main.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
)
