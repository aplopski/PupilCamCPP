#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "pupilcam" for configuration "Release"
set_property(TARGET pupilcam APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(pupilcam PROPERTIES
  IMPORTED_IMPLIB_RELEASE "D:/Plopski/3rdparty/pupilcam/lib/pupilcam.lib"
  IMPORTED_LOCATION_RELEASE "D:/Plopski/3rdparty/pupilcam/bin/pupilcam.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS pupilcam )
list(APPEND _IMPORT_CHECK_FILES_FOR_pupilcam "D:/Plopski/3rdparty/pupilcam/lib/pupilcam.lib" "D:/Plopski/3rdparty/pupilcam/bin/pupilcam.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
