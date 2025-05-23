# Due to incomprehensible errors between ASCOM and Qt6OpenGL!QOpenGL2PaintEngineEx::setState(), we temporarily had to disable ASCOM in QT6-based builds.
# No other code change related to Qt6 has been done so far, only the ASCOM selector has been greyed out.
# Rather sooner than later ASCOM must be replaced by Alpaca.
if(WIN32)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Threads REQUIRED)

SET(TelescopeControl_ASCOM_UIS
    TelescopeClientASCOMWidget.ui
    )

IF (${QT_VERSION_MAJOR} EQUAL "5")
     QT5_WRAP_UI(TelescopeControl_ASCOM_UIS_H ${TelescopeControl_ASCOM_UIS})
ELSE(${QT_VERSION_MAJOR} EQUAL "5")
     QT_WRAP_UI(TelescopeControl_ASCOM_UIS_H ${TelescopeControl_ASCOM_UIS})
ENDIF(${QT_VERSION_MAJOR} EQUAL "5")

add_library(TelescopeControl_ASCOM STATIC
    ASCOMDevice.hpp
    ASCOMDevice.cpp
    TelescopeClientASCOM.hpp
    TelescopeClientASCOM.cpp
    TelescopeClientASCOMWidget.hpp
    TelescopeClientASCOMWidget.cpp
    ${TelescopeControl_ASCOM_UIS_H}
    )

target_link_libraries(TelescopeControl_ASCOM
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    TelescopeControl_common
    ${CMAKE_THREAD_LIBS_INIT}
    )

target_include_directories(TelescopeControl_ASCOM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

IF(ENABLE_TESTING)
    add_subdirectory(test)
ENDIF(ENABLE_TESTING)

SET_TARGET_PROPERTIES(TelescopeControl_ASCOM PROPERTIES FOLDER "plugins/TelescopeControl")

ENDIF(WIN32)
