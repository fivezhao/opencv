#ifdef __VXWORKS__
#include "cv_cpu_config.h"
#endif

#ifdef CV_CPU_COMPILE_AVX
#undef CV_CPU_DISPATCH_MODE
#define CV_CPU_DISPATCH_MODE AVX
#endif

#include "precomp.hpp"
#include "mathfuncs_core.simd.hpp"
