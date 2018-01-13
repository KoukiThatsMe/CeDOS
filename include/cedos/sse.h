/*! \file
 * Provides information about the CPUs support for SSE instructions 
 * and functions to enable/disable them.
 */
#ifndef SSE_H
#define SSE_H

typedef enum {
    SSE_UNSUPPORTED,
    SSE_V1,
    SSE_V2,
    SSE_V3,
    SSSE_V3,
    SSE_V4_1,
    SSE_V4_2,
    SSE_V4_A,
    SSE_V5
} SSE_VERSION;

typedef enum {
    SSE_V5_UNSUPP,
    SSE_V5_XOP,
    SSE_V5_FMA4,
    SSE_V5_CVT16,
    SSE_V5_AVX,
    SSE_V5_XSAVE,
    SSE_V5_AVX_512
} SSE_V5_FEATURES;

SSE_VERSION sse_check_support(void);
int avx_check_support(SSE_V5_FEATURES);

void sse_enable(void);
void sse_disable(void);

void avx_enable(void);
void avx_disable(void);

#endif SSE_H