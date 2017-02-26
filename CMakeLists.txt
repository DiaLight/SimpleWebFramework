
#find_program(CLANG_PATH "clang")
#find_program(CLANG_CXX_PATH "clang++")
#IF(CLANG_PATH AND CLANG_CXX_PATH)
#    message(STATUS "Clang comppiler found")
#        message(STATUS "CLANG_PATH = " ${CLANG_PATH})
#        message(STATUS "CLANG_CXX_PATH = " ${CLANG_CXX_PATH})
#    set(CMAKE_C_COMPILER ${CLANG_PATH})
#    set(CMAKE_CXX_COMPILER ${CLANG_CXX_PATH})
#endif()

cmake_minimum_required(VERSION 3.6)
project(SimpleWebFramework)

set(CMAKE_CXX_STANDARD 14)

set(PROJECT_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/main/c++/headers)
set(PROJECT_NATIVE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/main/c++/native)
file(GLOB_RECURSE PROJECT_HEADERS_FILES ${PROJECT_HEADERS_DIR}/*.hpp)
file(GLOB_RECURSE PROJECT_NATIVE_FILES ${PROJECT_NATIVE_DIR}/*.cpp)

# build executable
include_directories(${PROJECT_HEADERS_DIR})
add_executable(${PROJECT_NAME} ${PROJECT_HEADERS_FILES} ${PROJECT_NATIVE_FILES})

# build and link subprojects
add_subdirectory(Network)
include_directories(${Network_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Network)
