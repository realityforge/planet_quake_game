#include "cg_huds.h"


hudelem_t hudelems[128];

void CG_ParseHUD(msg_t *msg) {
	int hudid = MSG_ReadLong(msg);
	hudelem_t *hudelem = hudelems + hudid;
	hudelem->active = 1;

	hudelem->left = MSG_ReadLong(msg); // maybe use MSG_ReadByte and scale from 0-255 to 0-640 and 0-480?
	hudelem->top = MSG_ReadLong(msg);
	hudelem->width = MSG_ReadLong(msg);
	hudelem->height = MSG_ReadLong(msg);
	hudelem->color[0] = (float)MSG_ReadByte(msg) / 255.0;
	hudelem->color[1] = (float)MSG_ReadByte(msg) / 255.0;
	hudelem->color[2] = (float)MSG_ReadByte(msg) / 255.0;
	hudelem->color[3] = (float)MSG_ReadByte(msg) / 255.0;
	strcpy(hudelem->text, MSG_ReadString(msg));
	hudelem->hidden   = MSG_ReadByte(msg);
}

void CG_FreeHUD(msg_t *msg) {
	int hudid = MSG_ReadLong(msg);
	hudelem_t *hudelem = hudelems + hudid;
	hudelem->active = 0;
}

CCALL void SCR_FillRect( float x, float y, float width, float height, const float *color );
CCALL void SCR_DrawStringExt( int x, int y, float size, const char *string, float *setColor, qboolean forceColor, qboolean noColorEscape );
CCALL void SCR_DrawPic( float x, float y, float width, float height, qhandle_t hShader );

void CG_RenderHUDS() {
	//static int first = 1;
	//if (first) {
	//	memset(&hudelems, 0, sizeof(hudelems));
	//	first = 0;
	//}
	hudelem_t *hudelem;
	for (int i=0; i<128; i++) {
		
		hudelem = hudelems + i;
		if (hudelem->active == 0)
			continue;
		if (hudelem->hidden)
			continue;
		//SCR_FillRect(hudelem->left, hudelem->top, hudelem->width, hudelem->height, hudelem->color);
		SCR_DrawPic(hudelem->left, hudelem->top, hudelem->width, hudelem->height, cls.whiteShader);
		// lets say the hud is left=10, top=20, width=500, height=30;
		// so that the text is aligned inside the shader background, add height/2 to the text position

		if (hudelem->text[0]) {
			int offset_top = hudelem->height >> 1; // e.g. 6 >> 1 == 3, 5 >> 1 == 2
			offset_top -= 3; // remove some amount to compensate font size
			int offset_left = 10; // just some static value to put text into shader a bit
			SCR_DrawBigString(hudelem->left + offset_left, hudelem->top + offset_top, hudelem->text, hudelem->color[3], qtrue);
		}
	}
}




void SV_WriteSnapshotHUDS(client_t *client, msg_t *msg ) {
	hudelem_t *hud;
	for (int i=0; i<128; i++) {
		hud = client->clienthudelems + i;
		if (hud->active == 0)
			continue;
		if (hud->resend == 0)
			continue;

		hud->resend = 0;

		if (hud->free) {
			
			MSG_WriteByte( msg, svc_hud_free );
			MSG_WriteLong(msg, i);
			hud->active = 0;
			hud->free = 0;
		} else {
			MSG_WriteByte( msg, svc_usermessage_1 );
			MSG_WriteLong(msg, i);
			MSG_WriteLong(msg, hud->left);
			MSG_WriteLong(msg, hud->top);
			MSG_WriteLong(msg, hud->width);
			MSG_WriteLong(msg, hud->height);
			MSG_WriteByte(msg, (int)(hud->color[0] * 255.0f));
			MSG_WriteByte(msg, (int)(hud->color[1] * 255.0f));
			MSG_WriteByte(msg, (int)(hud->color[2] * 255.0f));
			MSG_WriteByte(msg, (int)(hud->color[3] * 255.0f));
			MSG_WriteString(msg, hud->text);
			MSG_WriteByte(msg, hud->hidden);
		}
	}
}