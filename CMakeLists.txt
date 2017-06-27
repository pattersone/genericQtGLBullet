# Generic Qt 5 OpenGL Project that happens to include Bullet Physics also. :)
# EKP and JRB June 2017
#
# This should work on Windows, Mac, and Linux assuming the following dependencies:
# Qt 5.x is installed -- check paths below for Windows or other OS.
# Bullet is built with include and lib files available in project/include/bullet and project/lib/bullet folders.
#
# On Windows -- using CLion's CMake currently (although should work with whatever CMake with little change), and
# mingw32 is installed with Qt.  Again, see paths below for Windows.
# Qt dlls are copied to the bin folder where code builds.
# On MacOS and probably Linux, bullet dylibs need to also be available in the lib/bullet folders.

cmake_minimum_required(VERSION 3.3)
project(genericQtGLBullet)

# AUTOMOC invokes the Meta-Object Compiler to interpret QObjects, signals, slots, etc. in Qt Code.
# AUTOUIC takes the .ui form file and generates additional h/cpp files as needed for GUI.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# The line below does not seem to work in CLion (not sure about standalone CMake), so for now,
# need to make sure that CLion project build settings place executable in project/bin
# where Qt DLLs also reside.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

set(EXECUTABLE_NAME "${PROJECT_NAME}")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")  # If you like warnings, haha..

if(WIN32)
    set(QT_PATH "C:/Qt")
    set(Qt5_DIR "C:/Qt/5.9/mingw53_32/lib/cmake/Qt5")
    set(QT_MINGW "${QT_PATH}/${QT_VERSION}/${QT_MINGW_VERSION}")
    set(QT_MINGW_TOOLS "${QT_PATH}/Tools/mingw530_32")
    set(QT_MINGW_BIN "${QT_MINGW}/bin")
    set(CMAKE_CXX_COMPILER "${QT_MINGW_TOOLS}/bin/g++.exe")
    set(CMAKE_AR "${QT_MINGW_TOOLS}/bin/ar.exe")
    set(CMAKE_LINKER "${QT_MINGW_TOOLS}/bin/g++.exe")
else(WIN32)
    set(Qt5_DIR "~/Qt/5.7/clang_64/lib/cmake/Qt5")
endif(WIN32)

find_package(OpenGL REQUIRED)
find_package(Qt5 REQUIRED Core)
find_package(Qt5 REQUIRED Gui)
find_package(Qt5 REQUIRED Widgets)

set(SOURCES     main.cpp mainwindow.cpp MyQOpenGlWidget.cpp
                Controller.cpp PhysicsSimulator.cpp
                obj_parser.cpp)
set(HEADERS     mainwindow.h MyQOpenGlWidget.h
                Controller.h PhysicsSimulator.h
                obj_parser.h glAssert.h)
set(FORMS       mainwindow.ui)

include_directories("./include/bullet")
link_directories("./lib/bullet")

add_executable(${EXECUTABLE_NAME} ${SOURCES} ${HEADERS} ${FORMS})

target_link_libraries(${EXECUTABLE_NAME}
        Qt5::Widgets

        ${OPENGL_LIBRARIES}

        "libBulletDynamics.a"
        "libBulletCollision.a"
        "libLinearMath.a"
        )

