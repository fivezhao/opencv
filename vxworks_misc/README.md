BUILD SETUP
-----------

vxWorks projects
----------------
Build vsb and kernel using the following:

VSB:

    64 bit, IVY_BRIDGE (for NUC compatible build)
    or 32/64 bit intel_generic_bsp (for qemu compatible build)
    FBDEV_ITLGMCFB
    LANG_LIB_CPLUS_CPLUS_USER_2011
    UNIX - Unix compatibility support

VIP:

    BUNDLE_STANDALONE_SHELL
    BUNDLE_RTP_DEVELOP
    BUNDLE_POSIX
    INCLUDE_POSIX_PTHREAD_SCHEDULER
    INCLUDE_USB_GEN2_STORAGE_INIT
    INCLUDE_SYS_WARM_USB
    USB_SHOW
    INCLUDE_UHCI_INIT
    INCLUDE_EHCI_INIT
    INCLUDE_OHCI_INIT
    INCLUDE_XHCI_INIT
    INCLUDE_DOSFS_FAT
    INCLUDE_DISK_UTIL
    INCLUDE_FBDEV_SPLASH
    INCLUDE_FBDEV_ITLGMC_2
    INCLUDE_I915
    INCLUDE_FBDEV_SHOW
    INCLUDE_PC_CONSOLE
    INCLUDE_IPTELNETS
    INCLUDE_IFCONFIG
    INCLUDE_PING

**NOTE:** this was done on branch vx7-release -> 92ed1218972e202a9245795009965ff6cb8ba288

OPENCV PROJECT SETUP
------------------------
1. BUILD a static user library project based on the vsb that uses opencv-vx as a source folder
2. On the source folder (opencv-vx) set the following recursive exclude filters (files and folders)
    - regexp ```(cmake)|(apps)|(samples)|(cud.*)|(viz)|(superres)|(java)|(python)|(.*winrt.*)|(vxworks.*)```
    - on 3rdparty set resource filter (folders only) to include only libjpeg


3. Set default and active build spec to gnu_C++2011
    - for ipp support set default and active build spec to icc_C++2011

4. Cflags
    - C tool flags: -UHAVE_CUDA -DONVXWORKS
    - C++ tool flags: -D__OPENCV_BUILD -DONVXWORKS   -UHAVE_OPENGL  -UHAVE_CUDA  -UHAVE_OPENCL  -UOPENCV_NOSTL -UOPENCV_NOSTL_TRANSITIONAL
    - for ipp support also add -DHAVE_IPP

5. Includes

    $(PRJ_ROOT_DIR)/opencv-vx/include  
    $(PRJ_ROOT_DIR)/opencv-vx/modules/world/include  
    $(PRJ_ROOT_DIR)/opencv-vx/modules/calib3d/include  
    $(PRJ_ROOT_DIR)/opencv-vx/modules/features2d/include  
    $(PRJ_ROOT_DIR)/opencv-vx/modules/flann/include  
    $(PRJ_ROOT_DIR)/opencv-vx/modules/highgui/include  
    $(PRJ_ROOT_DIR)/opencv-vx/modules/ml/include  
    $(PRJ_ROOT_DIR)/opencv-vx/modules/objdetect/include  
    $(PRJ_ROOT_DIR)/opencv-vx/modules/photo/include  
    $(PRJ_ROOT_DIR)/opencv-vx/modules/shape/include  
    $(PRJ_ROOT_DIR)/opencv-vx/modules/stitching/include  
    $(PRJ_ROOT_DIR)/opencv-vx/modules/video/include  
    $(PRJ_ROOT_DIR)/opencv-vx/modules/videoio/include  
    $(PRJ_ROOT_DIR)/opencv-vx/modules/videostab/include  
    $(PRJ_ROOT_DIR)/opencv-vx/modules/imgcodecs/include  
    $(PRJ_ROOT_DIR)/opencv-vx/modules/imgproc/include  
    $(PRJ_ROOT_DIR)/opencv-vx/3rdparty/libjpeg  
    $(PRJ_ROOT_DIR)/opencv-vx/modules/ts/include  
    $(PRJ_ROOT_DIR)/opencv-vx/modules/core/include  

    - for ipp add the path to the ipp include dir eg. ( $(WIND_HOME)/compilers/ipp_vxworks-9.0.3.034/include)
     - make sure <src_destination_dir>/opencv-vx/3rdparty/libjpeg/ is part of the include path list

6. Libs (ipp only):  
     -L$(WIND_HOME)/compilers/ipp_vxworks-9.0.3.034/lib/intel64
     -lippcore -lippi -lipps -lippcv -lippvm -lippcc

7. Issues:  
    - in case the build fails because of wchar.h, here is a temporary patch
```C
#ifdef HAVE_IPP / ICC /
#if defined(_STDARG_H) && defined(_C_IN_NS) && defined(__cplusplus)
typedef ::va_list va_list;
#endif
#else / else ICC /
 #if defined(_WR_VA_LIST_DEFINED_IN_STD) && defined(__cplusplus)
using _CSTD va_list;
 #endif / _WR_VA_LIST_DEFINED_IN_STD /
#endif / ICC /
```


RTP build setup
---------------
- Create a rtp project
- Add the same build spec, Cflags, includes and libs as in the opencv project
- Link the resulted opencv lib with this rtp
- Add the fboutput directory located in vxworks_misc to the project
- Add the desired opencv code to the project

 **IMPORTANT:** There are a few differences in the vxWorks port:

 - Writing video files and capturing from video files:  
 - only single channel(no sound) mjpeg encoded videos are supported
 - Writing and reading image files:  
     - only jpeg images are supported
 - Capturing from video camera:  
     - the camera capture is customized to work with the UVC driver and the format of the capture is YUV2, therefore after capturing a frame there may be a need for a conversion to the desired format.
- Displaying an image:  
    - in vxWorks the image will be displayed on a framebuffer  
    - the display will need to be initialized (cvVxInitDisplay()) before the display loop
    - the required display format is RGBA  
    - before displaying an image with cvVxShow() convert it to RGBA format  
 - The above differences can be observed in the provided samples



8. Additional libraries and support:
 - Thread Builing Blocks (multicore processing)
     - to enable TBB support edit include/cvconfig.h and add #define HAVE_TBB and #define HAVE_PTHREADS_PF
     - git clone http://mdragusu@stash.wrs.com/scm/~mdragusu/tbb.git
     - create a vx static library with TBB
     - On the source folder set the following recursive exclude filters (regexp, files and folders):
          - ```(server)|(test.*)|(perf.*)|(server)|(tbbmalloc)|(tbbproxy)|(ia64-gas)|(examples)```
     - add the following compilation flags -DDO_ITT_NOTIFY=0 -DTBB_NO_LEGACY=1 -DTBB_USE_DEBUG=0 -DTBB_USE_ICC_BUILTINS=1 -DUSE_PTHREAD -D__TBB_BUILD=1 -D__TBB_DYNAMIC_LOAD_ENABLED=0 -D__TBB_ICC_BUILTIN_ATOMICS_PRESENT=1 -D__TBB_SURVIVE_THREAD_SWITCH=0 -D__TBB_WEAK_SYMBOLS_PRESENT=0 -Dtbb_EXPORTS
       (replace TBB_USE_ICC_BUILTINS and TBB_ICC_BUILTIN_ATOMICS_PRESENT to TBB_USE_GCC_BUILTINS TBB_GCC_BUILTIN_ATOMICS_PRESENT depending on the compiler)
    - the resulting lib will need to be linked with opencv

OpenCV 3.3.1 / Clang update
--------------------------------

The recent switch to a CLANG/LLVM-based compiler and to a unified kernel and user space libc++ allowed including additional modules in the VxWorks port of OpenCV and enabling support for advanced CPU features like AVX2.

It is now possible to compile and run OpenCV applications on VxWorks with support for the Deep Neural Networks module, both as kernel applications or RTPs.

Baseline vxWorks tree: branch vx7-i1032-dev, commit 7a091abcbbeb28005061c2b8d330c7a096aa5c9a
H/w platform: Intel NUC with Core i7 Skylake CPU
Compiler: LLVM version 5

When building the opencv static library the filters need to be updated to enable protobuf and dnn.

Note: Only the differences from the build configurations previously mentioned are described in the following sections.

OpenCV static library filters:
- opencv-3.3.1 sources: exclude all, files and folders, all children (recursive), name matches regular expression
 - ```(cmake)|(apps)|(samples)|(cud.*)|(viz)|(superres)|(java)|(python)|(.*winrt.*)|(vxworks.*)|(platforms)|(data)|(doc)```
- 3rdparty: include only, files and folders, not recursive, name matches regular expression
 - ```libjpeg|protobuf```

- modules: include only, files and folders, not recursive, name matches regular expression
    - ```(calib3d)|(core)|(features2d)|(flann)|(highgui)|(imgcodecs)|(imgproc)|(java)|(ml)|(objdetect)|(photo)|(shape)|(stitching)|(ts)|(video)|(videoio)|(videostab)|(world)|(dnn)```

Compilation flags:
- C++-Compiler
 - -D__OPENCV_BUILD -DONVXWORKS -UHAVE_OPENGL -UHAVE_CUDA -UHAVE_OPENCL -UOPENCV_NOSTL -UOPENCV_NOSTL_TRANSITIONAL -msse -msse2 -msse3 -mssse3 -msse4.1 -mpopcnt -msse4.2 -mf16c -mfma -mavx -mavx2  -DCV_ENABLE_INTRINSICS=1 -DGOOGLE_PROTOBUF_HAS_CXX11_HASH -DHAVE_PROTOBUF -DHAVE_PTHREAD

Depending on your target platform, adjust the -msse -> -mavx2 section in the C++-Compiler tool flags and the CPU features enabled in opencv-vx/include/cv_cpu_config.h

TBB and IPP are still supported, assuming the compilation variables are updated to include them (see above).

Sample code from opencv-vx/samples/dnn can be used as basis for OpenCV validation apps. Object detection applications using Yolo v2 and MobileNET SSD models are known to work.
