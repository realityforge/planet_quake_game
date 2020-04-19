#ifndef CG_HUDS_STRUCTS_H
#define CG_HUDS_STRUCTS_H

typedef struct hudelem_s {
	int active;
	int left;
	int top;
	int width;
	int height;
	float color[4]; // rgba
	char text[256];
	int hidden;
	int fadein;
	int fadeout;
	int free; // set to one and loop will send svc_hud_free + id
	int resend;
} hudelem_t;

#endif