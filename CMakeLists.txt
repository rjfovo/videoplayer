cmake_minimum_required(VERSION 3.14)

project(cutefish-videoplayer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Qt6 REQUIRED COMPONENTS Core Quick DBus Widgets LinguistTools)
find_package(Libmpv)

if(NOT Libmpv_FOUND)
    message(WARNING "Libmpv not found, trying to find mpv library directly")
    find_library(MPV_LIBRARY NAMES mpv)
    find_path(MPV_INCLUDE_DIR NAMES client.h PATH_SUFFIXES mpv)
    
    if(MPV_LIBRARY AND MPV_INCLUDE_DIR)
        message(STATUS "Found mpv library: ${MPV_LIBRARY}")
        message(STATUS "Found mpv include dir: ${MPV_INCLUDE_DIR}")
        set(Libmpv_FOUND TRUE)
        set(Libmpv_LIBRARIES ${MPV_LIBRARY})
        set(Libmpv_INCLUDE_DIRS ${MPV_INCLUDE_DIR})
    else()
        message(WARNING "Libmpv not found and cannot find mpv library directly. The project will be configured but cannot be built without libmpv. Please install libmpv-dev package to build.")
        set(Libmpv_FOUND FALSE)
        set(Libmpv_LIBRARIES "")
        set(Libmpv_INCLUDE_DIRS "")
    endif()
endif()

qt6_add_dbus_interface(screensaver_inhibit_SRCS ./org.freedesktop.ScreenSaver.xml screensaverdbusinterface)

set(PROJECT_SOURCES
    src/main.cpp
    src/application.cpp
    src/playlistmodel.cpp
    src/playlistitem.cpp
    src/track.cpp
    src/tracksmodel.cpp
    src/mpvobject.cpp
    src/qthelper.h
    src/lockmanager.cpp
    src/mpris2/mediaplayer2.cpp
    src/mpris2/mediaplayer2player.cpp
    qml.qrc
    ${screensaver_inhibit_SRCS}
)

add_executable(cutefish-videoplayer
    ${PROJECT_SOURCES}
)

target_link_libraries(cutefish-videoplayer
  PRIVATE
  Qt6::Core
  Qt6::Quick
  Qt6::DBus
  Qt6::Widgets
)

if(Libmpv_FOUND)
    target_link_libraries(cutefish-videoplayer PRIVATE ${Libmpv_LIBRARIES})
    target_include_directories(cutefish-videoplayer PRIVATE ${Libmpv_INCLUDE_DIRS})
endif()

install(TARGETS cutefish-videoplayer DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES
    cutefish-videoplayer.desktop
    DESTINATION /usr/share/applications/
    COMPONENT Runtime
)

# 暂时禁用翻译构建
# file(GLOB TS_FILES translations/*.ts)
# if(TS_FILES)
#     qt6_create_translation(QM_FILES ${TS_FILES})
#     add_custom_target(translations DEPENDS ${QM_FILES} SOURCES ${TS_FILES})
#     add_dependencies(cutefish-videoplayer translations)
# endif()

# install(FILES ${QM_FILES} DESTINATION /usr/share/cutefish-videoplayer/translations)
