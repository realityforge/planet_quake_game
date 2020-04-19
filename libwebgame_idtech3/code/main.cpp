
#ifndef EMSCRIPTEN
#include <stdio.h>
#include "include_ccall.h"

CCALL unsigned int SDL_GetTicks() {
	return 0;
}

#include "q_shared.h"


class pc_Vec3 { public:
	float x;
	float y;
	float z;
	//operator void* () { return (void*)this; }
	//operator float[3]() { return (float*)this; }
	float operator [] (int i) const { return (&this->x)[i]; }
	float& operator [] (int i) { return (&this->x)[i]; }
};



void test_vec3(vec3_t a[3]) {
	int index;

	printf("a (%f %f %f) (%f %f %f) (%f %f %f)\n",
		a[0][0],
		a[0][1],
		a[0][2],
		a[1][0],
		a[1][1],
		a[1][2],
		a[2][0],
		a[2][1],
		a[2][2]
	);
}

int main() {
	pc_Vec3 a[3] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
	pc_Vec3 b[3] = { {11, 22, 33}, {44, 55, 66}, {77, 88, 99} };
	pc_Vec3 c[3] = { {111, 222, 333}, {444, 555, 666}, {777, 888, 999} };
	printf("main()\n");
	//test_vec3(a);
	return 0;
}
#endif
