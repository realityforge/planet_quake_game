#include "include_math.h"

void vec3_a_is_b_plus_c_times_d(float *ret, float *eye, float *forward, float distance) {
	ret[0] = eye[0] + forward[0] * distance;
	ret[1] = eye[1] + forward[1] * distance;
	ret[2] = eye[2] + forward[2] * distance;
}

void vec3_a_is_b_plus_c(float *a, float *b, float *c) {
	a[0] = b[0] + c[0];
	a[1] = b[1] + c[1];
	a[2] = b[2] + c[2];
}

void vec3_a_is_b_minus_c(float *a, float *b, float *c) {
	a[0] = b[0] - c[0];
	a[1] = b[1] - c[1];
	a[2] = b[2] - c[2];
}

void vec3_a_is_b_times_c(float *a, float *b, float c) {
	a[0] = b[0] * c;
	a[1] = b[1] * c;
	a[2] = b[2] * c;
}