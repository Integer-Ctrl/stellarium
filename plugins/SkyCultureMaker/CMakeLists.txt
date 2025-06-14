# This is the cmake config file for the Sky Culture Maker plugin
SET(SCM_VERSION "0.1.0")

# download CPM.cmake
file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.8/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=78ba32abdf798bc616bab7c73aac32a17bbd7b06ad9e26a6add69de8f3ae4791
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)


# Option to manually control importer, defaults to TRUE, but overridden by Qt version
option(SCM_SHOULD_ENABLE_IMPORTER "Attempt to enable Sky Culture Importer" TRUE)
set(SCM_IMPORTER_ENABLED FALSE) # Default to disabled

if(SCM_SHOULD_ENABLE_IMPORTER AND NOT (QT_VERSION_MAJOR EQUAL "5"))
  set(SCM_IMPORTER_ENABLED TRUE)
  message(STATUS "Sky Culture Importer will be enabled.")
  # download https://github.com/Stellarium/stellarium-skyculture-converter
  CPMAddPackage(
    NAME              StellariumSkyCultureConverter
    GITHUB_REPOSITORY Stellarium/stellarium-skyculture-converter
    GIT_TAG           master
    OPTIONS
      "SKYCULTURE_CONVERTER_BUILD_TESTS OFF"
  )

  # download https://github.com/selmf/unarr for archives
  CPMAddPackage(
    NAME              unarr
    GITHUB_REPOSITORY selmf/unarr
    GIT_TAG           v1.1.1
  )

  if(WIN32)
    # Patch unarr's common/stream.c to include <objidl.h> after windows.h
    # Ensure this runs after unarr source is available
    if(TARGET unarr AND EXISTS "${unarr_SOURCE_DIR}/common/stream.c")
      file(READ "${unarr_SOURCE_DIR}/common/stream.c" _stream_c_content)
      string(REPLACE "#define COBJMACROS\n#include <windows.h>\n"
                    "#define COBJMACROS\n#include <windows.h>\n#include <objidl.h>\n"
                    _stream_c_content "${_stream_c_content}")
      file(WRITE "${unarr_SOURCE_DIR}/common/stream.c" "${_stream_c_content}")
    else()
      message(WARNING "unarr source directory or stream.c not found for patching. This might be an issue on Windows if importer is enabled.")
    endif()
  endif(WIN32)
  ADD_DEFINITIONS(-DSCM_IMPORTER_ENABLED_CPP) # Define for C++
else()
  message(STATUS "Sky Culture Importer is DISABLED (Qt version 5 or SCM_SHOULD_ENABLE_IMPORTER is OFF).")
endif()

ADD_DEFINITIONS(-DSKYCULTUREMAKER_PLUGIN_VERSION="${SCM_VERSION}")
ADD_DEFINITIONS(-DSKYCULTUREMAKER_PLUGIN_LICENSE="GNU GPLv2 or later")

ADD_SUBDIRECTORY( src )