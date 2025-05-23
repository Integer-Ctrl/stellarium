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
  GITHUB_REPOSITORY MherMnatsakanyan03/stellarium-skyculture-converter
  GIT_TAG           MakeLib
  OPTIONS
    "SKYCULTURE_CONVERTER_BUILD_TESTS OFF"
)

# download https://github.com/artemvlas/qmicroz for archives
CPMAddPackage(
  NAME              qmicroz
  GITHUB_REPOSITORY artemvlas/qmicroz
  GIT_TAG           v0.4
)


ADD_DEFINITIONS(-DSKYCULTUREMAKER_PLUGIN_VERSION="${SCM_VERSION}")
ADD_DEFINITIONS(-DSKYCULTUREMAKER_PLUGIN_LICENSE="GNU GPLv2 or later")

ADD_SUBDIRECTORY( src )
