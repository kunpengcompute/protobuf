#ifdef __aarch64__
#include <stdio.h>
#include "simdutf.h"

extern "C" {
int utf8_simdutf(const char *data, int len)
{
    return simdutf::validate_utf8(data, len) ? 1 : 0;
}
} // extern 
#endif