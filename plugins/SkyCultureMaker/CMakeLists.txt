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

# download https://github.com/Stellarium/stellarium-skyculture-converter
CPMAddPackage(
  NAME              StellariumSkyCultureConverter
  GITHUB_REPOSITORY Stellarium/stellarium-skyculture-converter
  GIT_TAG           master
  OPTIONS
    "SKYCULTURE_CONVERTER_BUILD_TESTS OFF"
    "ENABLE_LIBTIDY OFF"  # Add this option if the converter supports it
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
