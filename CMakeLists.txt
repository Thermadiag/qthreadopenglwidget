cmake_minimum_required(VERSION 3.16)

project(qthreadopenglwidget
    VERSION 1.0
    DESCRIPTION "Threaded Qt OpenGL Widget"
    HOMEPAGE_URL "https://github.com/Thermadiag/qthreadopenglwidget"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(GNUInstallDirs)

# project options
option(NO_WARNINGS "Treat warnings as errors" OFF)

# create source file list
file(GLOB Sources
     "src/*.h"
     "src/*.cpp" 
)

# Disable header only mode
set(PROJECT_HEADER_ONLY 0)


set(QTOW_INSTALL_LIBDIR       "${CMAKE_INSTALL_LIBDIR}")
set(QTOW_INSTALL_BINDIR       "${CMAKE_INSTALL_BINDIR}")
set(QTOW_INSTALL_DATAROOTDIR  "${CMAKE_INSTALL_DATAROOTDIR}")
set(QTOW_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/qthreadopenglwidget")


# add Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(QT NAMES Qt5 Qt6 REQUIRED )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGL Core Gui)



# add sources
add_library(qthreadopenglwidget SHARED 
	${Sources}
)

# add preprocessor definitions
target_compile_definitions(qthreadopenglwidget PRIVATE -DQTOW_BUILD_LIBRARY)

# link to Qt
if(${QT_VERSION_MAJOR} LESS 6)
set(QT_LIBS Qt5::Core
    Qt5::Gui
	Qt5::Widgets
	Qt5::OpenGL
	)
else()
set(QT_LIBS Qt::Core
    Qt::Gui
	Qt::Widgets
	Qt::OpenGL
	)
endif()
target_link_libraries(qthreadopenglwidget PRIVATE ${QT_LIBS})

if(${QT_VERSION_MAJOR} LESS 6)
else()
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)
target_link_libraries(qthreadopenglwidget PRIVATE Qt6::OpenGLWidgets)
endif()


#add linker options
if (WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	# mingw
	target_link_options(qthreadopenglwidget PRIVATE -lKernel32 -lpsapi -lBcrypt)
endif()

if(WIN32)
	target_compile_definitions(qthreadopenglwidget PRIVATE -D_CRT_SECURE_NO_WARNINGS)
endif()

if(NO_WARNINGS)
	if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
		target_compile_options(qthreadopenglwidget PRIVATE  /WX /W3 )
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		target_compile_options(qthreadopenglwidget PRIVATE -Werror -Wall -Wno-c++98-compat -Wno-c++98-compat-pedantic)
	else()
		target_compile_options(qthreadopenglwidget PRIVATE -Werror -Wall)
	endif()
endif()

target_include_directories(qthreadopenglwidget PUBLIC
					   "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/qthreadopenglwidget>"
					   "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
					   

install(TARGETS qthreadopenglwidget EXPORT qthreadopenglwidget ARCHIVE DESTINATION ${QTOW_INSTALL_LIBDIR} RUNTIME DESTINATION ${QTOW_INSTALL_BINDIR} LIBRARY DESTINATION ${QTOW_INSTALL_LIBDIR})
install(EXPORT qthreadopenglwidget DESTINATION ${QTOW_CMAKEDIR})



# Install headers and sources (for header-only mode)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/QThreadOpenGLWidget.h"
		DESTINATION "${CMAKE_INSTALL_PREFIX}/include")


# Configure and install qthreadopenglwidget.pc
configure_file(qthreadopenglwidget.pc.in qthreadopenglwidget.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qthreadopenglwidget.pc
			DESTINATION ${QTOW_INSTALL_DATAROOTDIR}/pkgconfig)


# Configure and install seqConfig.cmake and qthreadopenglwidgetConfigVersion.cmake
include(CMakePackageConfigHelpers)

configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/qthreadopenglwidgetConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/qthreadopenglwidgetConfig.cmake"
  INSTALL_DESTINATION ${QTOW_INSTALL_LIBDIR}/cmake/qthreadopenglwidget
  PATH_VARS )

  
# Generate seqConfigVersion.cmake
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/qthreadopenglwidgetConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion )
  
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qthreadopenglwidgetConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/qthreadopenglwidgetConfigVersion.cmake
        DESTINATION ${QTOW_INSTALL_LIBDIR}/cmake/qthreadopenglwidget )
		
