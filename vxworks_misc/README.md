OPENCV FOR VXWORKS
-------------------

There are two ways to build OpenCV

 I. With the VxWorks SDK for non-commercial https://labs.windriver.com/downloads/wrsdk.html 

II. With  Wind River Workbench and Wind River VxWorks products 


------------
I. Using The VxWorks SDK

NOTE: assuming vxsdk environment is set up

1. Clone the opencv4.1 customized for vxworks

2. Create a sysroot directory (this will be a directory for the opencv installation)

3. Create a directory for you opencv build 

4. Run:
   $  source <path-to-opencv-vx-sources-dir\>/platforms/vxworks/vxenv

5. In the newly created opencv build directory  you have two options:

 a. Comand line:

     Note: All paths in the following command will have to be edited to match your environment 


 $ cmake -DCMAKE_TOOLCHAIN_FILE=\<path-to-opencv-vx-sources-dir\>/platforms/vxworks/vxsdk.toolchain.cmake -DCMAKE_INSTALL_PREFIX=\<path-to-sysroot-directory\>/usr -DBUILD_PNG=ON  -DWITH_JPEG=ON -DBUILD_JPEG=ON -DBUILD_ZLIB=OFF -DBUILD_JAVA=OFF -DBUILD_PACKAGE=OFF -DBUILD_PERF_TESTS=OFF -DOPENCL_FOUND=OFF -DOPENCV_DNN_OPENCL=OFF -DWITH_1394=OFF -DWITH_ADE=OFF -DWITH_ARITH_DEC=OFF -DWITH_ARITH_ENC=OFF -DWITH_FFMPEG=OFF -DWITH_GSTREAMER=OFF -DWITH_IMGCODEC_PFM=OFF -DWITH_IMGCODEC_HDR=OFF -DWITH_IMGCODEC_PXM=OFF -DWITH_IMGCODEC_SUNRASTER=OFF -DWITH_ITT=OFF -DWITH_JASPER=OFF -DWITH_JPEG=OFF -DWITH_LAPACK=OFF -DWITH_OPENCL=OFF -DWITH_OPENCLAMDBLAS=OFF -DWITH_OPENCLAMDFFT=OFF -DWITH_OPENEXR=OFF  -DWITH_QUIRC=OFF -DWITH_TIFF=OFF -DWITH_WEBP=OFF -DWITH_EIGEN=OFF -DWITH_GTK=OFF -DWITH_VTK=OFF -DWITH_V4L=OFF -DBUILD_PROTOBUF=ON -DWITH_PROTOBUF=ON -DHAVE_PTHREAD=ON -DWITH_DNN=ON  \<path-to-opencv-vx-sources-dir\> 



 b. Run cmake-gui and make sure to set the following options as such:



-DCMAKE_CROSSCOMPILING=TRUE

-DCMAKE_SYSTEM_NAME=vxworks

CMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY
CMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY
CMAKE_FIND_ROOT_PATH_MODE_PROGRAM=ONLY

CMAKE_FIND_ROOT_PATH=\<path-to-sdk>

CMAKE_INSTALL_PREFIX=\<path-of-your-choosing\>/sysroot/usr
CMAKE_C_COMPILER=\<path-to-sdk\>/bin/x86-wrs-vxworks-cc
CMAKE_CXX_COMPILER=\<path-to-sdk\>/bin/x86-wrs-vxworks-cxx
CMAKE_LINKER=\<path-to-sdk\>/bin/x86-wrs-vxworks-ld
CMAKE_STRIP=\<path-to-sdk\>/bin/x86-wrs-vxworks-strip
CMAKE_NM=\<path-to-sdk\>/bin/x86-wrs-vxworks-nm

CMAKE_MAKE_PROGRAM=/usr/bin/make

 

Or instead of setting the above options, when clicking configure for the first time, select "Unix Makefiles" and go to Specify toolchain file for cross-compiling and load  <path-to-opencv-vx-sources-dir\>/platforms/vxworks/vxsdk.toolchain.cmake

 

ENABLE:

    BUILD_PNG
    BUILD_JPG
    WITH_JPEG
    BUILD_PROTOBUF
    WITH_PROTOBUF
    HAVE_PTHREAD
    WITH_DNN
 

DISABLE:

    BUILD_PACKAGE 
    BUILD_PERF_TESTS 
    BUILD_ZLIB 
    BUILD_JAVA 
    OPENCL_FOUND 
    OPENCV_DNN_OPENCL 
    WITH_1394 
    WITH_ADE 
    WITH_ARITH_DEC 
    WITH_ARITH_ENC 
    WITH_FFMPEG 
    WITH_GSTREAMER 
    WITH_IMGCODEC_PFM
    WITH_IMGCODEC_PXM
    WITH_IMGCODEC_HDR
    WITH_IMGCODEC_SUNRASTER
    WITH_ITT
    WITH_JASPER
    WITH_LAPACK
    WITH_OPENCL
    WITH_OPENCLAMDBLAS
    WITH_OPENCLAMDFFT
    WITH_OPENEXR
    WITH_QUIRC
    WITH_TIFF
    WITH_WEBP
    WITH_EIGEN
    WITH_GTK
    WITH_VTK 
    WITH_V4L

 

Click Configure then Generate

 
5.Compile everything:

    $ make 
    $ make install

 

The resulting binaries  should be installed in the sysroot directory.


RTP build setup
---------------

1. VxWorks  SDK:

-  In opencv-vx/vxworks-misc there are a few code samples to try out
-  Go to the sdk directory and run:

    $ x86-wrs-vxworks-cxx canny_image_sample.cpp -o canny.vxe -I<path-to-your-sysroot\>/sysroot/usr/include/opencv4 -L<path-to-your-sysroot\>/sysroot/usr/lib -lopencv_imgproc -lopencv_imgcodecs -lopencv_core

**Displaying the result on screen**

     cd opencv-vx/vxworks-misc/fboutput
     Note: depending on what on the hardware used and the initialized framebuffer the value of DEFAULT_FBDEV (inc cvvxdisplay.hpp) may need to change 
    $ cmake -DCMAKE_TOOLCHAIN_FILE=\<path-to-toolchain-file\>/vxsdk.toolchain.cmake   -DCMAKE_CXX_FLAGS=-I<path-to-opencv-install-dirt\>/usr/include/opencv4  .
    $ make 
    $ cd opencv-vx/vxworks-misc/
    $ x86-wrs-vxworks-cxx canny_video_sample.cpp -o canny.vxe -I<path-to-your-sysroo>/sysroot/usr/include/opencv4  -L<path-to-your-sysroot>/sysroot/usr/lib  -lopencv_imgcodecs -lopencv_imgproc  -lopencv_core -lopencv_videoio -lopencv_highgui -lopencv_photo -lopencv_video -lopencv_imgcodecs -L<path-to-opencv-vx>/opencv-vx/vxworks_misc/fboutput/ -lcvvxdisplay

2. Workbench:

- Create a rtp project
- Link the resulted opencv lib with this rtp
- Add the fboutput directory located in vxworks_misc to the project
- Add the desired opencv code to the project


 **IMPORTANT:** There are a few differences in the vxWorks port:

 - Writing video files and capturing from video files:  
    - only single channel(no sound) mjpeg encoded videos are supported
 - Writing and reading image files:  
     - only jpeg and png images are supported
 - Displaying an image:  
    - in vxWorks the image will be displayed on a framebuffer  
    - the display will need to be initialized (cvVxInitDisplay()) before the display loop
    - the required display format is RGBA  
    - before displaying an image with cvVxShow() convert it to RGBA format  
    - the framebuffer solution is intel only
 - The above differences from standard opencv examples can be observed in the provided samples


**Multicore using TBB**

Using vxworks integrated TBB 

- Build the VSB with UTILS_TBB
- Add the following settings to the opencv cmake command line, and rebuild opencv:

    -DCMAKE_CXX_FLAGS="-ltbb" 
    -DTBB_ENV_INCLUDE=\<path-to-sdk\>/include/usr/h/public 
    -DTBB_VER_FILE=\<path-to-sdk\>/include/usr/h/public/tbb/tbb_stddef.h 
    -DTBB_ENV_LIB=\<path-to-sdk\>/usr/lib/common/libtbb.so 

