#ifdef __VXWORKS__
#include "cv_cpu_config.h"
#endif

#define CV_CPU_SIMD_FILENAME "stat.simd.hpp"
#define CV_CPU_DISPATCH_MODE SSE4_2
#include "opencv2/core/private/cv_cpu_include_simd_declarations.hpp"
#define CV_CPU_DISPATCH_MODES_ALL SSE4_2, BASELINE

#ifdef CV_CPU_COMPILE_AVX2
#define CV_CPU_DISPATCH_MODE AVX2
#include "opencv2/core/private/cv_cpu_include_simd_declarations.hpp"
#undef CV_CPU_DISPATCH_MODES_ALL
#define CV_CPU_DISPATCH_MODES_ALL AVX2, SSE4_2, BASELINE
#endif

/*
#define CV_CPU_DISPATCH_MODES_ALL BASELINE
*/
