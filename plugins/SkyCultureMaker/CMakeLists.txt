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

find_package(LibTidy QUIET)

if(NOT LibTidy_FOUND)
  CPMAddPackage(
    NAME tidy
    GITHUB_REPOSITORY htacg/tidy-html5
    GIT_TAG           5.8.0
  )

if(tidy_ADDED)
    set(LIBTIDY_INCLUDE_DIR "${tidy_SOURCE_DIR}/include")
    set(LIBTIDY_LIBRARY "${tidy_BINARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}tidy_static${CMAKE_STATIC_LIBRARY_SUFFIX}")

    # Patch include/tidyplatform.h to add typedef for uint
    set(_tidyplatform_h "${tidy_SOURCE_DIR}/include/tidyplatform.h")
    if(EXISTS "${_tidyplatform_h}")
        file(READ "${_tidyplatform_h}" _tidyplatform_h_content)
        string(REPLACE
            "#endif\n\n#if !defined(HPUX_OS) && !defined(CYGWIN_OS) && !defined(MAC_OS_X) && !defined(BE_OS) && !defined(SOLARIS_OS) && !defined(BSD_BASED_OS) && !defined(OSF_OS) && !defined(IRIX_OS) && !defined(AIX_OS) && !defined(LINUX_OS) && !defined(__HAIKU__)"
            "#else\n#undef ulong\n   typedef unsigned long ulong;"
            _tidyplatform_h_content "${_tidyplatform_h_content}")
        file(WRITE "${_tidyplatform_h}" "${_tidyplatform_h_content}")
    endif()
  endif()
endif()

# download https://github.com/Stellarium/stellarium-skyculture-converter
CPMAddPackage(
  NAME              StellariumSkyCultureConverter
  GITHUB_REPOSITORY Stellarium/stellarium-skyculture-converter
  GIT_TAG           master
  OPTIONS
    "SKYCULTURE_CONVERTER_BUILD_TESTS OFF"
)

if(StellariumSkyCultureConverter_ADDED AND tidy_ADDED)
  ADD_DEPENDENCIES(skyculture_converter_lib tidy-static)
endif()

# download https://github.com/selmf/unarr for archives
CPMAddPackage(
  NAME              unarr
  GITHUB_REPOSITORY selmf/unarr
  GIT_TAG           v1.1.1
)

if(WIN32)
  # Patch unarr's common/stream.c to include <objidl.h> after windows.h
  if(EXISTS "${unarr_SOURCE_DIR}/common/stream.c")
    file(READ "${unarr_SOURCE_DIR}/common/stream.c" _stream_c_content)
    string(REPLACE "#define COBJMACROS\n#include <windows.h>\n"
                  "#define COBJMACROS\n#include <windows.h>\n#include <objidl.h>\n"
                  _stream_c_content "${_stream_c_content}")
    file(WRITE "${unarr_SOURCE_DIR}/common/stream.c" "${_stream_c_content}")
  endif()
endif(WIN32)

ADD_DEFINITIONS(-DSKYCULTUREMAKER_PLUGIN_VERSION="${SCM_VERSION}")
ADD_DEFINITIONS(-DSKYCULTUREMAKER_PLUGIN_LICENSE="GNU GPLv2 or later")

ADD_SUBDIRECTORY( src )
