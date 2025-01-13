cmake_minimum_required(VERSION 3.16)
project(chip VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# find qthreadopenglwidget
find_package(qthreadopenglwidget REQUIRED)

# create executable
add_executable(chip main.cpp view.h view.cpp chip.h chip.cpp mainwindow.h mainwindow.cpp)

# add ressources
qt_add_resources(chip "images"
    PREFIX ""
    FILES qt4logo.png zoomin.png zoomout.png rotateleft.png rotateright.png fileprint.png)

# link to qthreadopenglwidget as well as required Qt libraries
target_link_libraries(chip PRIVATE ${QTOW_QT_LIBS} qthreadopenglwidget)
