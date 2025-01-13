cmake_minimum_required(VERSION 3.16)
project(ellipse VERSION 1.0 LANGUAGES C CXX)

find_package(qthreadopenglwidget REQUIRED)

# Create executable
add_executable(ellipse main.cpp)
# Configure project
target_link_libraries(ellipse PRIVATE ${QTOW_QT_LIBS} qthreadopenglwidget)
if(WIN32)
target_link_libraries(ellipse PRIVATE opengl32)
else()
target_link_libraries(ellipse PRIVATE GL)
endif()
