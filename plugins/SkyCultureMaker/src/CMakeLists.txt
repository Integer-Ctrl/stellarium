INCLUDE_DIRECTORIES(
     .
     gui
     ${CMAKE_BINARY_DIR}/plugins/SkyCultureMaker/src
     ${CMAKE_BINARY_DIR}/plugins/SkyCultureMaker/src/gui
)

LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/src)

SET(SkyCultureMaker_SRCS
     SkyCultureMaker.hpp
     SkyCultureMaker.cpp
)

SET(StartDialog_UIS
     gui/startDialog.ui
)     

################# compiles resources files ############
SET(SkyCultureMaker_RES ../resources/SkyCultureMaker.qrc)
IF (${QT_VERSION_MAJOR} EQUAL "5")
     QT5_WRAP_UI(StartDialog_UIS_H ${StartDialog_UIS})
     QT5_ADD_RESOURCES(SkyCultureMaker_RES_CXX ${SkyCultureMaker_RES})
ELSE()
     QT_WRAP_UI(StartDialog_UIS_H ${StartDialog_UIS})
     QT_ADD_RESOURCES(SkyCultureMaker_RES_CXX ${SkyCultureMaker_RES})
ENDIF()

ADD_LIBRARY(SkyCultureMaker-static STATIC ${SkyCultureMaker_SRCS} ${SkyCultureMaker_RES_CXX} ${StartDialog_UIS})
SET_TARGET_PROPERTIES(SkyCultureMaker-static PROPERTIES OUTPUT_NAME "SkyCultureMaker")
TARGET_LINK_LIBRARIES(SkyCultureMaker-static Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets)
SET_TARGET_PROPERTIES(SkyCultureMaker-static PROPERTIES COMPILE_FLAGS "-DQT_STATICPLUGIN")
ADD_DEPENDENCIES(AllStaticPlugins SkyCultureMaker-static)

SET_TARGET_PROPERTIES(SkyCultureMaker-static PROPERTIES FOLDER "plugins/SkyCultureMaker")
