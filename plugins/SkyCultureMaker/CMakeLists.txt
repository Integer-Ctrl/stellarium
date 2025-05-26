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

# Add LibTidy dependency - try system first, then fallback to CPM
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(LIBTIDY QUIET tidy)
endif()

if(NOT LIBTIDY_FOUND)
  # Try to find system libtidy
  find_library(LIBTIDY_LIBRARY NAMES tidy)
  find_path(LIBTIDY_INCLUDE_DIR NAMES tidy.h)
  
  if(LIBTIDY_LIBRARY AND LIBTIDY_INCLUDE_DIR)
    set(LIBTIDY_FOUND TRUE)
    set(LibTidy_FOUND TRUE)
    
    # Create imported target for system libtidy
    add_library(LibTidy::LibTidy SHARED IMPORTED)
    set_target_properties(LibTidy::LibTidy PROPERTIES
      IMPORTED_LOCATION ${LIBTIDY_LIBRARY}
      INTERFACE_INCLUDE_DIRECTORIES ${LIBTIDY_INCLUDE_DIR}
    )
  else()
    # Last resort: use CPM with a fixed version that compiles better
    CPMAddPackage(
      NAME tidy-html5
      GITHUB_REPOSITORY htacg/tidy-html5
      GIT_TAG 5.6.0  # Use older, more stable version
      OPTIONS
        "BUILD_TAB2SPACE OFF"
        "BUILD_SHARED_LIB OFF"
        "TIDY_CONSOLE_SHARED OFF"
    )
    
    if(tidy-html5_ADDED)
      set(LIBTIDY_INCLUDE_DIR "${tidy-html5_SOURCE_DIR}/include" CACHE STRING "LibTidy include directory")
      set(LibTidy_FOUND TRUE CACHE BOOL "LibTidy found")
      set(LIBTIDY_FOUND TRUE CACHE BOOL "LibTidy found")
      
      # Create an alias to the existing target
      add_library(LibTidy::LibTidy ALIAS tidy-static)
      
      # Set variables for compatibility
      set(LIBTIDY_LIBRARY "tidy-static" CACHE STRING "LibTidy library path")
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

# download https://github.com/selmf/unarr for archives
CPMAddPackage(
  NAME              unarr
  GITHUB_REPOSITORY selmf/unarr
  GIT_TAG           v1.1.1
)


ADD_DEFINITIONS(-DSKYCULTUREMAKER_PLUGIN_VERSION="${SCM_VERSION}")
ADD_DEFINITIONS(-DSKYCULTUREMAKER_PLUGIN_LICENSE="GNU GPLv2 or later")

ADD_SUBDIRECTORY( src )
