add_executable(example example.cpp)

include_directories(
  ${LIBUVC_INCLUDE_DIR}
  ${LIBUSB_INCLUDE_DIR}
  ${JPEG_INCLUDE_DIR}
  ${OpenCV_INCLUDE_DIRS}
  ${CMAKE_INSTALL_PREFIX}/include
)

if (WIN32)
	target_link_libraries(example optimized ${CMAKE_INSTALL_PREFIX}/lib/pupilcam.lib debug ${CMAKE_INSTALL_PREFIX}/lib/pupilcamd.lib
					optimized ${LIBUSB_RELEASE_LIBRARY} debug ${LIBUSB_DEBUG_LIBRARY} 
					optimized ${LIBUVC_RELEASE_LIBRARY} debug ${LIBUVC_DEBUG_LIBRARY} 
					optimized ${JPEG_RELEASE_LIBRARY_PATH}/turbojpeg.lib debug ${JPEG_DEBUG_LIBRARY_PATH}/turbojpeg.lib
					optimized ${JPEG_RELEASE_LIBRARY_PATH}/jpeg.lib debug ${JPEG_DEBUG_LIBRARY_PATH}/jpeg.lib
					optimized ${OpenCV_LIBS} debug ${OpenCV_LIBS})
else()
	target_link_libraries (example ${JPEG_LIBRARIES} ${LIBUSB_LIBRARY_NAMES} ${LIBUVC_LIBRARY_NAMES} ${JPEG_LIBRARY_NAMES}/jpeg.lib ${JPEG_LIBRARY_NAMES}/turbojpeg.lib ${OpenCV_LIBS})
endif()
 
install(TARGETS example
  RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
)