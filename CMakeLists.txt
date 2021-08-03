cmake_minimum_required(VERSION 3.19)

include(ExternalProject)
include(CheckStructHasMember)
include(CheckIncludeFiles)

project(sipi VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(COMMON_LOCAL ${CMAKE_BINARY_DIR}/local)
set(COMMON_SRCS ${CMAKE_BINARY_DIR}/extsrcs)
set(CONFIGURE_LIBDIR ${COMMON_LOCAL}/lib)
set(CONFIGURE_INCDIR ${COMMON_LOCAL}/include)

set(SIPI_EXE ${CMAKE_CURRENT_BINARY_DIR}/cserver)

