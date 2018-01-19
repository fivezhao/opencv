#if 1
// OpenCV CPU baseline features

#define CV_CPU_COMPILE_SSE 1
#define CV_CPU_BASELINE_COMPILE_SSE 1

#define CV_CPU_COMPILE_SSE2 1
#define CV_CPU_BASELINE_COMPILE_SSE2 1

#define CV_CPU_COMPILE_SSE3 1
#define CV_CPU_BASELINE_COMPILE_SSE3 1

#define CV_CPU_BASELINE_FEATURES 0 \
    , CV_CPU_SSE \
    , CV_CPU_SSE2 \
    , CV_CPU_SSE3 \


/* VxWorks: Tweak CPU features here! */
#define CV_CPU_COMPILE_SSE4_1 1
#define CV_CPU_COMPILE_POPCNT 1
#define CV_CPU_COMPILE_SSE4_2 1
#define CV_CPU_COMPILE_AVX    1
/* avx2-> */
#define CV_CPU_COMPILE_FP16   1
#define CV_CPU_COMPILE_FMA3   1
#define CV_CPU_COMPILE_AVX2   1

// OpenCV supported CPU dispatched features
#ifdef CV_CPU_COMPILE_SSE4_1
#define CV_CPU_DISPATCH_COMPILE_SSE4_1 1
#endif

#ifdef CV_CPU_COMPILE_SSE4_2
#define CV_CPU_DISPATCH_COMPILE_SSE4_2 1
#endif

#ifdef CV_CPU_COMPILE_FP16
#define CV_CPU_DISPATCH_COMPILE_FP16 1
#endif

#ifdef CV_CPU_COMPILE_AVX
#define CV_CPU_DISPATCH_COMPILE_AVX 1
#endif

#ifdef CV_CPU_COMPILE_AVX2
#define CV_CPU_DISPATCH_COMPILE_AVX2 1
#endif

#else /* minimal defaults */
#define CV_CPU_BASELINE_FEATURES 0 
#endif
