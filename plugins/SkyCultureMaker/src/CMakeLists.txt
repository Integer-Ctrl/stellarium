INCLUDE_DIRECTORIES(#
     .
     ${CMAKE_BINARY_DIR}/plugins/SkyCultureMaker/src
)

LINK_DIRECTORIES(${BUILD_DIR}/src)

SET(SkyCultureMaker_SRCS
     SkyCultureMaker.hpp
     SkyCultureMaker.cpp
)

ADD_LIBRARY(SkyCultureMaker-static STATIC ${SkyCultureMaker_SRCS})
SET_TARGET_PROPERTIES(SkyCultureMaker-static PROPERTIES OUTPUT_NAME "SkyCultureMaker")
TARGET_LINK_LIBRARIES(SkyCultureMaker-static Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui)
SET_TARGET_PROPERTIES(SkyCultureMaker-static PROPERTIES COMPILE_FLAGS "-DQT_STATICPLUGIN")
ADD_DEPENDENCIES(AllStaticPlugins SkyCultureMaker-static)

SET_TARGET_PROPERTIES(SkyCultureMaker-static PROPERTIES FOLDER "plugins/SkyCultureMaker")
