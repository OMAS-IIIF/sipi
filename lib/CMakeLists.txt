cmake_minimum_required(VERSION 3.19)

project(sipilib VERSION 1.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(DARWIN "Darwin")
set(LINUX "Linux")
if(CMAKE_SYSTEM_NAME STREQUAL DARWIN)
    message(STATUS "Compiling for Darwin...")
    message(STATUS ${CMAKE_SYSTEM_NAME})
    SET(CMAKE_MACOSX_RPATH 1)
elseif(CMAKE_SYSTEM_NAME STREQUAL LINUX)
    message(STATUS "Compiling for Linux64...")
    message(STATUS ${CMAKE_SYSTEM_NAME})
    execute_process(COMMAND /usr/bin/lsb_release -si COMMAND tr -d '\n' OUTPUT_VARIABLE CMAKE_DISTRO_NAME)
    MESSAGE(STATUS "Distro: " ${CMAKE_DISTRO_NAME})
endif()

#
# set path to special find_package() code
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_ROOT}/Modules/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#
# Process the ICC headers provided in ICC-Profiles
#
add_custom_command(
        OUTPUT ${GENERATED_ICC_HEADERS}
        COMMENT "Generating ICC profile includes"
        WORKING_DIRECTORY ${COMMON_INCLUDE_FILES_DIR}
        COMMAND cp ${COMMON_INCLUDE_FILES_DIR}/ICC-Profiles/AdobeRGB1998.icc ${COMMON_INCLUDE_FILES_DIR}/AdobeRGB1998.icc
        COMMAND xxd -i AdobeRGB1998.icc AdobeRGB1998_icc.h
        COMMAND rm ${COMMON_INCLUDE_FILES_DIR}/AdobeRGB1998.icc
        COMMAND cp ${COMMON_INCLUDE_FILES_DIR}/ICC-Profiles/USWebCoatedSWOP.icc ${COMMON_INCLUDE_FILES_DIR}/USWebCoatedSWOP.icc
        COMMAND xxd -i USWebCoatedSWOP.icc USWebCoatedSWOP_icc.h
        COMMAND rm ${COMMON_INCLUDE_FILES_DIR}/USWebCoatedSWOP.icc
        COMMAND cp ${COMMON_INCLUDE_FILES_DIR}/ICC-Profiles/YCC709.icm ${COMMON_INCLUDE_FILES_DIR}/YCC709.icm
        COMMAND xxd -i YCC709.icm YCC709_icc.h
        COMMAND rm ${COMMON_INCLUDE_FILES_DIR}/YCC709.icm
        COMMAND cp ${COMMON_INCLUDE_FILES_DIR}/ICC-Profiles/Rec709-Rec1886.icc ${COMMON_INCLUDE_FILES_DIR}/Rec709-Rec1886.icc
        COMMAND xxd -i Rec709-Rec1886.icc Rec709-Rec1886_icc.h
        COMMAND rm ${COMMON_INCLUDE_FILES_DIR}/Rec709-Rec1886.icc
)

#
# add the dependency
#
add_custom_target(icc_profiles
        DEPENDS ${GENERATED_ICC_HEADERS}
        )


include_directories(
        ${COMMON_LOCAL}/include
        /usr/local/include
        #${LIBMAGIC_INCLUDES}
)

configure_file(SipiLibVersion.h.in SipiLibVersion.h)
configure_file(HasAtimeSpec.h.in HasAtimeSpec.h)

add_library(sipilib
        SipiVersion.h
        SipiCommon.cpp SipiCommon.h
        SipiError.cpp SipiError.h
        SipiHttpServer.cpp SipiHttpServer.h
        SipiImage.cpp SipiImage.h
        SipiCache.cpp SipiCache.h
        SipiFilenameHash.cpp SipiFilenameHash.h
        SipiLua.cpp SipiLua.cpp
        SipiIO.h
        formats/SipiIOJ2k.cpp formats/SipiIOJ2k.h
        formats/SipiIOJpeg.cpp formats/SipiIOJpeg.h
        formats/SipiIOPng.cpp formats/SipiIOPng.h
        formats/SipiIOTiff.cpp formats/SipiIOTiff.h
        iiifparser/SipiIdentifier.cpp iiifparser/SipiIdentifier.h
        iiifparser/SipiQualityFormat.cpp iiifparser/SipiQualityFormat.h
        iiifparser/SipiRegion.cpp iiifparser/SipiRegion.h
        iiifparser/SipiRotation.cpp iiifparser/SipiRotation.h
        iiifparser/SipiSize.cpp iiifparser/SipiSize.h
        metadata/SipiEssentials.cpp metadata/SipiEssentials.h
        metadata/SipiExif.cpp metadata/SipiExif.h
        metadata/SipiIcc.cpp metadata/SipiIcc.h
        metadata/SipiIptc.cpp metadata/SipiIptc.h
        metadata/SipiXmp.cpp metadata/SipiXmp.h
        )

add_dependencies(sipilib
        cserve
        exiv2
        lcms2
        expat
        gettext
        jbigkit
        lerc
        webp
        jpeg
        tiff
        png
        icc_profiles
        kdu
        )

target_include_directories(sipilib
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )

install(TARGETS sipilib
        DESTINATION lib/sipilib)
install(FILES Global.h Error.h Cserve.h Connection.h CserveVersion.h
        DESTINATION include/cserve)