#ifndef INCLUDE_MATH_H
#define INCLUDE_MATH_H

#include "include_ccall.h"

CCALL void vec3_a_is_b_plus_c_times_d(float *ret, float *eye, float *forward, float distance);
CCALL void vec3_a_is_b_plus_c(float *a, float *b, float *c);
CCALL void vec3_a_is_b_minus_c(float *a, float *b, float *c);
CCALL void vec3_a_is_b_times_c(float *a, float *b, float c);

#endif