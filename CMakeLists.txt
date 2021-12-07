cmake_minimum_required(VERSION 3.19)

include(ExternalProject)
include(CheckStructHasMember)
include(CheckIncludeFiles)

project(sipi VERSION 1.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(COMMON_LIBSIPI_FILES_DIR ${PROJECT_SOURCE_DIR}/lib)

set(COMMON_LOCAL ${CMAKE_BINARY_DIR}/local)
set(COMMON_SRCS ${CMAKE_BINARY_DIR}/extsrcs)
set(CONFIGURE_LIBDIR ${COMMON_LOCAL}/lib)
set(CONFIGURE_INCDIR ${COMMON_LOCAL}/include)
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)
set(COMMON_PATCHES ${PROJECT_SOURCE_DIR}/patches)
set(COMMON_VENDOR ${PROJECT_SOURCE_DIR}/vendor)
set(SIPI_EXE ${CMAKE_CURRENT_BINARY_DIR}/sipi)

option(MAKE_SHARED_SIPI "Create SIPI using all shared libraries" OFF)

set(DARWIN "Darwin")
set(LINUX "Linux")
math(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")
if(CMAKE_SYSTEM_NAME STREQUAL DARWIN)
    message(STATUS "Compiling for Darwin...")
    message(STATUS ${CMAKE_SYSTEM_NAME})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    SET(CMAKE_MACOSX_RPATH 1)
    if (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "x86_64")
        SET(KDU_MAKE "Makefile-MAC-x86-${BITS}-gcc")
        SET(KDU_ARCH "Mac-x86-${BITS}-gcc")
    else()
        SET(KDU_MAKE "Makefile-MAC-arm-${BITS}-gcc")
        SET(KDU_ARCH "Mac-arm-${BITS}-gcc")
    endif()
    SET(LUA_ARCH "macosx")
    # ToDo: shouldn't this be also set on the mac through FindOpenSSL?
    #SET(OPENSSL_ROOT_DIR "/usr/local/opt/openssl" CACHE FILEPATH "openssl")
    #SET(OPENSSL_USE_STATIC_LIBS ON)
elseif(CMAKE_SYSTEM_NAME STREQUAL LINUX)
    message(STATUS "Compiling for Linux64...")
    message(STATUS ${CMAKE_SYSTEM_NAME})
    execute_process(COMMAND /usr/bin/lsb_release -si COMMAND tr -d '\n' OUTPUT_VARIABLE CMAKE_DISTRO_NAME)
    MESSAGE(STATUS "Distro: " ${CMAKE_DISTRO_NAME})
    if (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "x86_64")
        SET(KDU_MAKE "Makefile-Linux-x86-${BITS}-gcc")
        SET(KDU_ARCH "Linux-x86-${BITS}-gcc")
    else()
        SET(KDU_MAKE "Makefile-Linux-arm-${BITS}-gcc")
        SET(KDU_ARCH "Linux-arm-${BITS}-gcc")
    endif()
    SET(LUA_ARCH "linux")
endif()
message(STATUS "Compiling for ${KDU_ARCH}")


set(GENERATED_ICC_HEADERS
        ${COMMON_INCLUDE_FILES_DIR}/AdobeRGB1998_icc.h
        ${COMMON_INCLUDE_FILES_DIR}/USWebCoatedSWOP_icc.h
        ${COMMON_INCLUDE_FILES_DIR}/YCC709_icc.h
        )

add_custom_command(
        OUTPUT ${GENERATED_ICC_HEADERS}
        COMMENT "Generating ICC profile includes"
        WORKING_DIRECTORY ${COMMON_INCLUDE_FILES_DIR}
        COMMAND cp ${PROJECT_SOURCE_DIR}/ICC-Profiles/AdobeRGB1998.icc ${COMMON_INCLUDE_FILES_DIR}/AdobeRGB1998.icc
        COMMAND xxd -i AdobeRGB1998.icc AdobeRGB1998_icc.h
        COMMAND rm ${COMMON_INCLUDE_FILES_DIR}/AdobeRGB1998.icc
        COMMAND cp ${PROJECT_SOURCE_DIR}/ICC-Profiles/USWebCoatedSWOP.icc ${COMMON_INCLUDE_FILES_DIR}/USWebCoatedSWOP.icc
        COMMAND xxd -i USWebCoatedSWOP.icc USWebCoatedSWOP_icc.h
        COMMAND rm ${COMMON_INCLUDE_FILES_DIR}/USWebCoatedSWOP.icc
        COMMAND cp ${PROJECT_SOURCE_DIR}/ICC-Profiles/YCC709.icm ${COMMON_INCLUDE_FILES_DIR}/YCC709.icm
        COMMAND xxd -i YCC709.icm YCC709_icc.h
        COMMAND rm ${COMMON_INCLUDE_FILES_DIR}/YCC709.icm
        COMMAND cp ${PROJECT_SOURCE_DIR}/ICC-Profiles/Rec709-Rec1886.icc ${COMMON_INCLUDE_FILES_DIR}/Rec709-Rec1886.icc
        COMMAND xxd -i Rec709-Rec1886.icc Rec709-Rec1886_icc.h
        COMMAND rm ${COMMON_INCLUDE_FILES_DIR}/Rec709-Rec1886.icc
)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_subdirectory(${EXT_PROJECTS_DIR}/cserve)
add_subdirectory(${EXT_PROJECTS_DIR}/jpeg)
add_subdirectory(${EXT_PROJECTS_DIR}/jbigkit)
add_subdirectory(${EXT_PROJECTS_DIR}/webp)
add_subdirectory(${EXT_PROJECTS_DIR}/tiff)
add_subdirectory(${EXT_PROJECTS_DIR}/expat)
add_subdirectory(${EXT_PROJECTS_DIR}/png)
add_subdirectory(${EXT_PROJECTS_DIR}/bzip2)
add_subdirectory(${EXT_PROJECTS_DIR}/gettext)
add_subdirectory(${EXT_PROJECTS_DIR}/lcms2)
add_subdirectory(${EXT_PROJECTS_DIR}/exiv2)
add_subdirectory(${EXT_PROJECTS_DIR}/kakadu)
add_subdirectory(lib)

add_executable(sipi
        sipi.cpp
        SipiConf.cpp SipiConf.h)
