#ifdef __VXWORKS__
#include "cv_cpu_config.h"
#endif

#ifdef CV_CPU_COMPILE_SSE2
#undef CV_CPU_DISPATCH_MODE
#define CV_CPU_DISPATCH_MODE SSE2
#endif

#include "precomp.hpp"
#include "accum.simd.hpp"

