# PupilCapture
interface for capture of Pupil labs cameras with C++

The framework handles the pupillabs cameras (https://github.com/pupil-labs/) with asynchronous input on windows
It is based on pyuvc example by Pupil labs (https://github.com/pupil-labs/pyuvc)

Requirement:
- libusbK enabled pupil-labs cameras (https://github.com/pupil-labs/pupil/wiki/Windows-Driver-Setup)
- libusb (https://github.com/pupil-labs/libusb)
- libuvc (https://github.com/pupil-labs/libuvc)
- OpenCV
- turbojpeg

Steps:

1. Make sure that the header files are in correct locations, as specified in the CmakeLists.txt files
2. Build and link the library
3. Enjoy

Example code can be found in example.cpp

Known issues:

If you recieve an "Out of memory" error, reduce LIBUVC_NUM_TRANSFER_BUFS in libuvc_internal.h and recompile it.

I had no problems running 5 cameras at the same time with LIBUVC_NUM_TRANSFER_BUFS 8, but could not open more than two at LIBUVC_NUM_TRANSFER_BUFS 100.



