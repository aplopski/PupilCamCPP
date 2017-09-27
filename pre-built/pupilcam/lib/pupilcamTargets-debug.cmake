#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "pupilcam" for configuration "Debug"
set_property(TARGET pupilcam APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(pupilcam PROPERTIES
  IMPORTED_IMPLIB_DEBUG "D:/Plopski/3rdparty/pupilcam/lib/pupilcamd.lib"
  IMPORTED_LOCATION_DEBUG "D:/Plopski/3rdparty/pupilcam/bin/pupilcamd.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS pupilcam )
list(APPEND _IMPORT_CHECK_FILES_FOR_pupilcam "D:/Plopski/3rdparty/pupilcam/lib/pupilcamd.lib" "D:/Plopski/3rdparty/pupilcam/bin/pupilcamd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
