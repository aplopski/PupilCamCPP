#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "uvc" for configuration "Release"
set_property(TARGET uvc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(uvc PROPERTIES
  IMPORTED_IMPLIB_RELEASE "D:/Plopski/3rdparty/libuvc/lib/uvc.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "D:/Plopski/3rdparty/turbojpeg/lib/Release/jpeg.lib;D:/Plopski/3rdparty/turbojpeg/lib/Release/turbojpeg.lib;D:/Plopski/3rdparty/libusb/lib/libusb.lib;D:/Plopski/3rdparty/pthreads/lib/x64/pthreadVC2.lib"
  IMPORTED_LOCATION_RELEASE "D:/Plopski/3rdparty/libuvc/bin/uvc.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS uvc )
list(APPEND _IMPORT_CHECK_FILES_FOR_uvc "D:/Plopski/3rdparty/libuvc/lib/uvc.lib" "D:/Plopski/3rdparty/libuvc/bin/uvc.dll" )

# Import target "uvc_static" for configuration "Release"
set_property(TARGET uvc_static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(uvc_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "D:/Plopski/3rdparty/libuvc/lib/uvc.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS uvc_static )
list(APPEND _IMPORT_CHECK_FILES_FOR_uvc_static "D:/Plopski/3rdparty/libuvc/lib/uvc.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
