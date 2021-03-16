cmake_minimum_required(VERSION 3.18)
include(FetchContent)

SET(CMAKE_SDSL_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sdsl)

# SDSL
add_library(sdsl STATIC IMPORTED GLOBAL)
set_target_properties(
        sdsl
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SDSL_LIB_DIR}/lib/libsdsl.a)

# divsufsort
add_library(divsufsort STATIC IMPORTED GLOBAL)

set_target_properties(
        divsufsort
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SDSL_LIB_DIR}/lib/libdivsufsort.a)

# divsufsort64
add_library(divsufsort64 STATIC IMPORTED GLOBAL)

set_target_properties(
        divsufsort64
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SDSL_LIB_DIR}/lib/libdivsufsort64.a)

include_directories(${CMAKE_SDSL_LIB_DIR}/include/)



## benchmark
add_library(benchmark STATIC IMPORTED GLOBAL)

set_target_properties(
        benchmark
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/benchmark/build/src/libbenchmark.a)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/benchmark/include/)


## Add malloc_count
FetchContent_Declare(
        malloc_count
        GIT_REPOSITORY https://github.com/bingmann/malloc_count
)
FetchContent_GetProperties(malloc_count)
if(NOT malloc_count_POPULATED)
    FetchContent_Populate(malloc_count)

    add_library(malloc_count OBJECT ${malloc_count_SOURCE_DIR}/malloc_count.c ${malloc_count_SOURCE_DIR}/malloc_count.h)
    target_link_libraries(malloc_count dl)
    target_include_directories(malloc_count PUBLIC "${malloc_count_SOURCE_DIR}")

    add_library(memprofile OBJECT ${malloc_count_SOURCE_DIR}/memprofile.h)
    set_target_properties(memprofile PROPERTIES LINKER_LANGUAGE C)
    target_include_directories(memprofile PUBLIC "${malloc_count_SOURCE_DIR}")
endif()



### Add r-index
#FetchContent_Declare(
#    r-index
#    GIT_REPOSITORY https://github.com/maxrossi91/r-index.git
#)
#
#FetchContent_GetProperties(r-index)
#if(NOT r-index_POPULATED)
#    FetchContent_Populate(r-index)
#
#    add_subdirectory(${r-index_SOURCE_DIR} ${r-index_BINARY_DIR} )#EXCLUDE_FROM_ALL)
#    add_library(ri INTERFACE)
#    target_link_libraries(ri INTERFACE klib z sdsl divsufsort divsufsort64)
#    target_include_directories(ri INTERFACE ${r-index_SOURCE_DIR}/internal ${CMAKE_CURRENT_SOURCE_DIR}/include/)
#endif()

add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/r-index-master/ )#EXCLUDE_FROM_ALL)

#include_directories(../include)
#set (CMAKE_EXE_LINKER_FLAGS "-I ${CMAKE_CURRENT_SOURCE_DIR}/include -L ${CMAKE_CURRENT_SOURCE_DIR}/lib")
