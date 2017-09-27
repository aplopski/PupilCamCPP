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

find_path(JPEG_INCLUDE_DIR turbojpeg/jpeglib.h turbojpeg/turbojpeg.h
          PATHS C:/3rdparty/jpegturbo/include)
find_path(LIBUVC_INCLUDE_DIR libuvc/libuvc.h
        PATHS C:/3rdparty/libuvc/include)
		
if (WIN32)
	find_library(LIBUVC_RELEASE_LIBRARY uvc
		PATHS C:/3rdparty/libuvc/lib/vc14/x64/Release)
	find_library(LIBUVC_DEBUG_LIBRARY uvc
		PATHS C:/3rdparty/libuvc/lib/vc14/x64/Debug)
	find_library(LIBUSB_DEBUG_LIBRARY libusb
		PATHS C:/3rdparty/libusb/lib/vc14/x64/Debug)
	find_library(LIBUSB_RELEASE_LIBRARY libusb
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

include_directories(
  ${LIBUVC_INCLUDE_DIR}
  ${LIBUSB_INCLUDE_DIR}
  ${JPEG_INCLUDE_DIR}
  ${OpenCV_INCLUDE_DIRS}
)

add_subdirectory(pupilcam)
add_subdirectory(example)