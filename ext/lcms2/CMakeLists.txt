cmake_minimum_required(VERSION 3.14.5)
project(lcms2_builder C CXX)
include(ExternalProject)

#
# get lcms2
#
if(MAKE_SHARED_SIPI)
    set(COMPOPT "--enable-shared=yes")
    set(COMPOPT "----enable-static=no")
else()
    set(COMPOPT "--enable-shared=no")
    set(COMPOPT "----enable-static=yes")
endif()

ExternalProject_Add(project_lcms2
        DEPENDS jpeg
        DEPENDS tiff
        DEPENDS zlib
        INSTALL_DIR ${COMMON_LOCAL}
        GIT_REPOSITORY https://github.com/mm2/Little-CMS.git
        SOURCE_DIR ${COMMON_SRCS}/lcms2-latest
        CONFIGURE_COMMAND   ${COMMON_SRCS}/lcms2-latest/configure
                            --prefix=${COMMON_LOCAL}
                            --with-jpeg=${COMMON_LOCAL}
                            --with-tiff=${COMMON_LOCAL}
                            --libdir=${CONFIGURE_LIBDIR}
        BUILD_COMMAND make
        INSTALL_COMMAND make PREFIX=${COMMON_LOCAL} install
        BUILD_IN_SOURCE 1
        )

ExternalProject_Get_Property(project_lcms2 install_dir)

set_property(TARGET lcms2 PROPERTY IMPORTED_LOCATION ${CONFIGURE_LIBDIR}/liblcms2${CMAKE_STATIC_LIBRARY_SUFFIX})
if(MAKE_SHARED_SIPI)
    add_library(lcms2 SHARED IMPORTED GLOBAL)
    set_property(TARGET lcms2 PROPERTY IMPORTED_LOCATION ${CONFIGURE_LIBDIR}/liblcms2${CMAKE_SHARED_LIBRARY_SUFFIX})
else()
    add_library(lcms2 STATIC IMPORTED GLOBAL)
    set_property(TARGET lcms2 PROPERTY IMPORTED_LOCATION ${CONFIGURE_LIBDIR}/liblcms2${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()

add_dependencies(lcms2 project_lcms2)
