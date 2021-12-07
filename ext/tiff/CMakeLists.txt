cmake_minimum_required(VERSION 3.14.5)
project(tiff_builder C CXX)
include(ExternalProject)

#
# get libtiff-4.0.6
#

ExternalProject_Add(project_tiff
        DEEPNDS lzma
        DEPENDS jpeg
        DEPENDS jbigkit
        DEPENDS webp
        DEPENDS lerc
        INSTALL_DIR ${COMMON_LOCAL}
        TEST_BEFORE_INSTALL 1
        GIT_REPOSITORY https://gitlab.com/libtiff/libtiff.git
        SOURCE_DIR ${COMMON_SRCS}/libtiff-latest
        CMAKE_ARGS  ${CMAKE_CREATE_SHARED}
                    -DCMAKE_C_STANDARD_LIBRARIES="-lpthread"
                    -DCMAKE_PREFIX_PATH=${COMMON_LOCAL}
                    -DCMAKE_INSTALL_PREFIX=${COMMON_LOCAL}
                    -DCMAKE_INSTALL_LIBDIR=${CONFIGURE_LIBDIR}
                    -DWEBP_LIBRARY=${CONFIGURE_LIBDIR}/libwebp${CMAKE_STATIC_LIBRARY_SUFFIX}
                    -DWEBP_INCLUDE_DIR=${CONFIGURE_INCDIR}
        INSTALL_COMMAND make test
        COMMAND make install
        COMMAND ${CMAKE_COMMAND} -E copy ${COMMON_SRCS}/libtiff-4.1.0/libtiff/tif_dir.h ${COMMON_LOCAL}/include/
        BUILD_IN_SOURCE 1
)
ExternalProject_Get_Property(project_tiff install_dir)
if(MAKE_SHARED_SIPI)
    add_library(tiff SHARED IMPORTED GLOBAL)
    set_property(TARGET tiff PROPERTY IMPORTED_LOCATION ${CONFIGURE_LIBDIR}/libtiff${CMAKE_SHARED_LIBRARY_SUFFIX})
else()
    add_library(tiff STATIC IMPORTED GLOBAL)
    set_property(TARGET tiff PROPERTY IMPORTED_LOCATION ${CONFIGURE_LIBDIR}/libtiff${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()
add_dependencies(tiff project_tiff)
