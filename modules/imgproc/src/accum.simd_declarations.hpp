#ifdef __VXWORKS__
#include "cv_cpu_config.h"
#endif

#define CV_CPU_SIMD_FILENAME "accum.simd.hpp"
#define CV_CPU_DISPATCH_MODE SSE2
#include "opencv2/core/private/cv_cpu_include_simd_declarations.hpp"
#define CV_CPU_DISPATCH_MODES_ALL SSE2, BASELINE

#ifdef CV_CPU_COMPILE_AVX
#define CV_CPU_DISPATCH_MODE AVX
#include "opencv2/core/private/cv_cpu_include_simd_declarations.hpp"
#undef CV_CPU_DISPATCH_MODES_ALL
#define CV_CPU_DISPATCH_MODES_ALL NEON, AVX, SSE2, BASELINE
#endif

#define CV_CPU_DISPATCH_MODE NEON
#include "opencv2/core/private/cv_cpu_include_simd_declarations.hpp"
#define CV_CPU_DISPATCH_MODES_ALL NEON, AVX, SSE2, BASELINE

/*
#define CV_CPU_DISPATCH_MODES_ALL BASELINE
*/
