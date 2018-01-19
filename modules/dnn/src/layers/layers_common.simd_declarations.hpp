#ifdef __VXWORKS__
#include "cv_cpu_config.h"
#endif

#define CV_CPU_SIMD_FILENAME "layers/layers_common.simd.hpp"

#define CV_CPU_DISPATCH_MODES_ALL BASELINE

#ifdef CV_CPU_COMPILE_AVX
#define CV_CPU_DISPATCH_MODE AVX
/*#error AVX mode enabled*/
#include "opencv2/core/private/cv_cpu_include_simd_declarations.hpp"
#undef CV_CPU_DISPATCH_MODES_ALL
#define CV_CPU_DISPATCH_MODES_ALL AVX, BASELINE
#endif

#ifdef CV_CPU_COMPILE_AVX2
/*#error AVX2 mode enabled*/
#define CV_CPU_DISPATCH_MODE AVX2
#include "opencv2/core/private/cv_cpu_include_simd_declarations.hpp"
#undef CV_CPU_DISPATCH_MODES_ALL
#define CV_CPU_DISPATCH_MODES_ALL AVX2, AVX, BASELINE
#endif
