SET(SOURCES FrameGrabber.cpp FrameProcessing.cpp)
SET(HEADERS FrameGrabber.hpp)

include_directories(
  ${LIBUVC_INCLUDE_DIR}
  ${LIBUSB_INCLUDE_DIR}
  ${JPEG_INCLUDE_DIR}
  ${OpenCV_INCLUDE_DIRS}
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
