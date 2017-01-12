cmake_minimum_required(VERSION 3.0)
project(pupilcam)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif ()
if (NOT CMAKE_BUILD_TARGET)
  message(STATUS "No target type selected, default to shared library")
  set(CMAKE_BUILD_TARGET "Shared" CACHE STRING "" FORCE)
endif()

set(pupilcam_VERSION_MAJOR 0)
set(pupilcam_VERSION_MINOR 0)
set(pupilcam_VERSION_PATCH 1)
set(pupilcam_VERSION ${pupilcam_VERSION_MAJOR}.${pupilcam_VERSION_MINOR}.${pupilcam_VERSION_PATCH})
find_package(OpenCV REQUIRED core)

find_path(JPEG_INCLUDE_DIR jpeglib.h turbojpeg.h
          PATHS C:/3rdparty/jpegturbo/include)
find_path(LIBUVC_INCLUDE_DIR libuvc/libuvc.h
        PATHS C:/3rdparty/libuvc/include)
		
if (WIN32)
	find_library(LIBUVC_RELEASE_LIBRARY uvc
		PATHS C:/3rdparty/libuvc/lib/vc14/x64/Release)
	find_library(LIBUVC_DEBUG_LIBRARY uvc
		PATHS C:/3rdparty/libuvc/lib/vc14/x64/Debug)
	find_library(LIBUSB_DEBUG_LIBRARY libusb-1.0
		PATHS C:/3rdparty/libusb/lib/vc14/x64/Debug)
	find_library(LIBUSB_RELEASE_LIBRARY libusb-1.0
		PATHS C:/3rdparty/libusb/lib/vc14/x64/Release)
	find_path(LIBUSB_INCLUDE_DIR libusb/libusb.h
		PATHS C:/3rdparty/libusb/include)
	find_path(JPEG_DEBUG_LIBRARY_PATH jpeg.lib turbojpeg.lib
	   PATHS C:/3rdparty/jpegturbo/lib/vc14/Debug)
	find_path(JPEG_RELEASE_LIBRARY_PATH jpeg.lib turbojpeg.lib
	   PATHS C:/3rdparty/jpegturbo/lib/vc14/Release)
else()
	find_library(LIBUVC_LIBRARY_NAMES uvc
		PATHS /opt/local/lib)
	find_library(LIBUSB_LIBRARY_NAMES usb-1.0
		PATHS /opt/local/lib)
	find_path(LIBUSB_INCLUDE_DIR libusb-1.0/libusb.h
		PATHS /opt/local/include)
	find_path(JPEG_LIBRARY_NAMES jpeg turbojpeg
	   PATHS /opt/local/lib)
endif()


if(JPEG_INCLUDE_DIR)
    set(JPEG_FOUND ON)
else()
    unset(JPEG_RELEASE_LIBRARY_PATH CACHE)
	unset(JPEG_DEBUG_LIBRARY_PATH CACHE)
	unset(JPEG_LIBRARY_NAMES CACHE)
endif()

if (CMAKE_INSTALL_RPATH MATCHES "${CMAKE_INSTALL_PREFIX}/lib")
else()
unset(CMAKE_INSTALL_RPATH CACHE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH
	"Installation directory for CMake files")
endif()
SET(SOURCES FrameGrabber.cpp FrameProcessing.cpp)

include_directories(
  ${LIBUVC_INCLUDE_DIR}
  ${LIBUSB_INCLUDE_DIR}
  ${JPEG_INCLUDE_DIR}
)

if(${CMAKE_BUILD_TARGET} MATCHES "Shared")
  add_library(pupilcam SHARED ${SOURCES})
elseif(${CMAKE_BUILD_TARGET} MATCHES "Static")
  add_library(pupilcam STATIC ${SOURCES})
else()
  message( FATAL_ERROR "Invalid build type ${CMAKE_BUILD_TARGET}" )
endif()

set_target_properties(pupilcam PROPERTIES
  PUBLIC_HEADER "FrameGrabber.hpp" DEBUG_POSTFIX d WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
if (WIN32)
	target_link_libraries(pupilcam optimized ${LIBUSB_RELEASE_LIBRARY} debug ${LIBUSB_DEBUG_LIBRARY} 
					optimized ${LIBUVC_RELEASE_LIBRARY} debug ${LIBUVC_DEBUG_LIBRARY} 
					optimized ${JPEG_RELEASE_LIBRARY_PATH}/turbojpeg.lib debug ${JPEG_DEBUG_LIBRARY_PATH}/turbojpeg.lib
					optimized ${JPEG_RELEASE_LIBRARY_PATH}/jpeg.lib debug ${JPEG_DEBUG_LIBRARY_PATH}/jpeg.lib
					optimized ${OpenCV_LIBS} debug ${OpenCV_LIBS})
else()
	target_link_libraries (pupilcam ${JPEG_LIBRARIES} ${LIBUSB_LIBRARY_NAMES} ${LIBUVC_LIBRARY_NAMES} ${JPEG_LIBRARY_NAMES}/jpeg.lib ${JPEG_LIBRARY_NAMES}/turbojpeg.lib ${OpenCV_LIBS})
endif()
 
install(TARGETS pupilcam
  EXPORT pupilcamTargets
  LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
  RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pupilcam"
)

export(TARGETS pupilcam
  FILE "${PROJECT_BINARY_DIR}/pupilcamTargets.cmake")
export(PACKAGE pupilcam)


set(CONF_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(CONF_LIBRARY "${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}cam${CMAKE_SHARED_LIBRARY_SUFFIX}")

configure_file(pupilcamConfig.cmake.in ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/pupilcamConfig.cmake)

configure_file(pupilcamConfigVersion.cmake.in ${PROJECT_BINARY_DIR}/pupilcamConfigVersion.cmake @ONLY)



install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/pupilcamConfig.cmake"
  "${PROJECT_BINARY_DIR}/pupilcamConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_RPATH}")

install(EXPORT pupilcamTargets
  DESTINATION "${CMAKE_INSTALL_RPATH}")
