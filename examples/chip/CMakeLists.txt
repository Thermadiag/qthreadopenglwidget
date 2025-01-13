cmake_minimum_required(VERSION 3.16)
project(chip VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(qthreadopenglwidget REQUIRED)

# Create executable
add_executable(chip main.cpp view.h view.cpp chip.h chip.cpp mainwindow.h mainwindow.cpp)

qt_add_resources(chip "images"
    PREFIX ""
    FILES qt4logo.png zoomin.png zoomout.png rotateleft.png rotateright.png fileprint.png)

# Configure project
target_link_libraries(chip PRIVATE ${QTOW_QT_LIBS} qthreadopenglwidget opengl32)
