cmake_minimum_required(VERSION 2.6)

### use a compiler supporting constexpr math
# set(CMAKE_C_COMPILER "/usr/local/lib/gcc-9.1.0/bin/gcc" CACHE STRING "gcc compiler" FORCE)
# set(CMAKE_CXX_COMPILER "/usr/local/lib/gcc-9.1.0/bin/g++" CACHE STRING "g++ compiler" FORCE)
enable_language(CXX)

# set(CMAKE_C_COMPILER "gcc" CACHE STRING "gcc compiler" FORCE)

# set(CMAKE_CXX_COMPILER "g++" CACHE STRING "g++ compiler" FORCE)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS "7.3.0")
    message(FATAL_ERROR "Insufficient gcc version ${CMAKE_CXX_COMPILER_VERSION} (Minimum required 7.4.0)")
  endif()
endif()



### Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

project(ShapedSlp)

include_directories(${PROJECT_SOURCE_DIR})
set(FOLCA_SOURCE_DIR ${PROJECT_SOURCE_DIR}/folca)
include_directories(${FOLCA_SOURCE_DIR})
set(SUX_SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/sux/sux)
include_directories(${SUX_SOURCE_DIR}/function)
include_directories(${SUX_SOURCE_DIR}/support)

### check for SDSL
find_library(SDSL_LIB libsdsl.a
        PATH ${CMAKE_SDSL_LIB_DIR}/lib
        HINTS ${CMAKE_SDSL_LIB_DIR}/lib)
if (NOT SDSL_LIB)
  message("${CMAKE_SDSL_LIB_DIR}/lib")
  message("sdsl-lite not found, building as external content")
#  set(CMAKE_INSTALL_INCLUDEDIR "include") # This is an hack because include(GUIInstallDirs) doesn't work
#  include(FetchContent)
#  ## Add sdsl
#  FetchContent_Declare(
#    sdsl
#    GIT_REPOSITORY https://github.com/simongog/sdsl-lite
#  )
#
#  FetchContent_GetProperties(sdsl)
#  if(NOT sdsl_POPULATED)
#    FetchContent_Populate(sdsl)
#
#    set(GENERATE_DOC OFF CACHE BOOL "Do not generate doxygen for sdsl-lite")
#
#    add_subdirectory(${sdsl_SOURCE_DIR} ${sdsl_BINARY_DIR} EXCLUDE_FROM_ALL)
#  endif()
#
#  ## Add divsuffsort
#  FetchContent_Declare(
#    divsufsort
#    GIT_REPOSITORY https://github.com/simongog/libdivsufsort.git
#    GIT_TAG        2.0.1
#  )
#
#  FetchContent_GetProperties(divsufsort)
#  if(NOT divsufsort_POPULATED)
#    FetchContent_Populate(divsufsort)
#
#    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Do not build a shared library for libdivsufsort")
#    set(BUILD_EXAMPLES OFF CACHE BOOL "Do not build libdivsufsort example")
#    set(BUILD_DIVSUFSORT64 ON CACHE BOOL "Build libdivsufsort in 64-bits mode")
#
#    add_subdirectory(${divsufsort_SOURCE_DIR} ${divsufsort_BINARY_DIR} EXCLUDE_FROM_ALL)
#
#    target_include_directories(divsufsort PUBLIC "${divsufsort_BINARY_DIR}/include")
#    target_include_directories(divsufsort64 PUBLIC "${divsufsort_BINARY_DIR}/include")
#  endif()
else()
  find_library(DSS divsufsort)
  find_library(DSS64 divsufsort64)
  message("sdsl: " ${SDSL_LIB})
  message("divsufsort: " ${DSS})
  message("divsufsort64: " ${DSS64})
endif()

message("Building in ${CMAKE_BUILD_TYPE} mode")

# set(CMAKE_CXX_FLAGS "--std=c++2a /opt/local/lib/libstdc++.6.dylib")
# set(CMAKE_CXX_FLAGS "--std=c++2a")

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -std=c++17 -ggdb -g -pthread") # -march=native
set(CMAKE_CXX_FLAGS_RELEASE "-g -std=c++17 -Ofast -fstrict-aliasing -pthread -DNDEBUG") #-march=native
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -std=c++17 -ggdb -Ofast -fstrict-aliasing -pthread") #-march=native


add_executable(SlpEncBuild SlpEncBuild.cpp ${FOLCA_SOURCE_DIR}/fbtree.cpp ${FOLCA_SOURCE_DIR}/onlinebp.c)
TARGET_LINK_LIBRARIES(SlpEncBuild sdsl)

add_executable(SubstrBenchmark SubstrBenchmark.cpp ${FOLCA_SOURCE_DIR}/fbtree.cpp ${FOLCA_SOURCE_DIR}/onlinebp.c)
TARGET_LINK_LIBRARIES(SubstrBenchmark sdsl)

add_executable(Decompress Decompress.cpp ${FOLCA_SOURCE_DIR}/fbtree.cpp ${FOLCA_SOURCE_DIR}/onlinebp.c)
TARGET_LINK_LIBRARIES(Decompress sdsl)


add_executable(LceBenchmark LceBenchmark.cpp ${FOLCA_SOURCE_DIR}/fbtree.cpp ${FOLCA_SOURCE_DIR}/onlinebp.c)
TARGET_LINK_LIBRARIES(LceBenchmark sdsl)
