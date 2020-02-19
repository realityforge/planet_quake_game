


#include "ui_local.h"

typedef struct {
    menutext_s *text;
    menubitmap_s *bitmat;
    oaitem_t *next;
} oaitem_t;

typedef struct {
	menuframework_s	menu;

        int sizeoftexts;
	menutext_s	*texts;

        int sizeofbitmaps;
	menubitmap_s	*bitmaps;
} oamenu_t;

int OaUi_InitMenu(oamenu_t *menu, int texts, int bitmaps) {
    if(texts>0) {
        menu->sizeoftexts=texts;
        menu->texts = (menutext_s*)UI_Alloc(texts*sizeof(menutext_s));
    } else {
        menu->sizeoftexts = 0;
        menu->texts = NULL;
    }

    if(bitmaps>0) {
        menu->sizeofbitmaps = bitmaps;
        menu->texts = (menutext_s*)UI_Alloc(bitmaps*sizeof(menubitmap_s));
    } else {
        menu->sizeofbitmaps = 0;
        menu->bitmaps = NULL;
    }
}