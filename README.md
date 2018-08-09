# PupilCamCPP
interface for capture of Pupil labs cameras with C++

The framework handles the pupillabs cameras (https://github.com/pupil-labs/) with asynchronous input on windows
It is based on pyuvc example by Pupil labs (https://github.com/pupil-labs/pyuvc)

Requirements:
- libusbK enabled pupil-labs cameras (https://github.com/pupil-labs/pupil/wiki/Windows-Driver-Setup)
- libusb (https://github.com/pupil-labs/libusb)
- libuvc (https://github.com/pupil-labs/libuvc)
- OpenCV
- turbojpeg (https://github.com/libjpeg-turbo/libjpeg-turbo)

The current cmake is linking against custom-named libraries, if you use off-the-shelf builds, you have to change the naming in the cmakelists.txt file, as well as change the included headers to the new locations!

Steps:

1. Make sure that the header files are in correct locations, as specified in the CmakeLists.txt files and that the naming is consistent
2. Build and link the library
3. Enjoy

The example program builds an example to application. The code can be found in example/example.cpp

Known issues:

If you recieve an "Out of memory" error, reduce LIBUVC_NUM_TRANSFER_BUFS in libuvc_internal.h and recompile it.

I had no problems running 5 cameras at the same time with LIBUVC_NUM_TRANSFER_BUFS 8, but could not open more than two at the default setting. The included pre-built libraries include both versions of LIBUVC.


If one of the cameras crashes/is not working, the program may crash as a whole.
