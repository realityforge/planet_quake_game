// main.cpp: initialisation & main loop

#include "engine.h"

extern void cleargamma();

void cleanup()
{
    recorder::stop();
    cleanupserver();
    if(screen) SDL_SetWindowGrab(screen, SDL_FALSE);
    SDL_SetRelativeMouseMode(SDL_FALSE);
    SDL_ShowCursor(SDL_TRUE);
    cleargamma();
    freeocta(worldroot);
    UI::cleanup();
    extern void clear_command(); clear_command();
    extern void clear_console(); clear_console();
    extern void clear_models();  clear_models();
    extern void clear_sound();   clear_sound();
    closelogfile();
    ovr::destroy();
    #ifdef __APPLE__
        if(screen) SDL_SetWindowFullscreen(screen, 0);
    #endif
    SDL_Quit();
}

extern void writeinitcfg();

void quit()                     // normal exit
{
    writeinitcfg();
    writeservercfg();
    abortconnect();
    disconnect();
    localdisconnect();
    writecfg();
    cleanup();
    exit(EXIT_SUCCESS);
}

void fatal(const char *s, ...)    // failure exit
{
    static int errors = 0;
    errors++;

    if(errors <= 2) // print up to one extra recursive error
    {
        defvformatstring(msg,s,s);
        logoutf("%s", msg);

        if(errors <= 1) // avoid recursion
        {
            if(SDL_WasInit(SDL_INIT_VIDEO))
            {
                if(screen) SDL_SetWindowGrab(screen, SDL_FALSE);
                SDL_SetRelativeMouseMode(SDL_FALSE);
                SDL_ShowCursor(SDL_TRUE);
                cleargamma();
                #ifdef __APPLE__
                    if(screen) SDL_SetWindowFullscreen(screen, 0);
                #endif
            }
            SDL_Quit();
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Tesseract fatal error", msg, NULL);
        }
    }

    exit(EXIT_FAILURE);
}

VAR(desktopw, 1, 0, 0);
VAR(desktoph, 1, 0, 0);
int screenw = 0, screenh = 0;
SDL_Window *screen = NULL;
SDL_GLContext glcontext = NULL;

int curtime = 0, lastmillis = 1, elapsedtime = 0, totalmillis = 1;

dynent *player = NULL;

int initing = NOT_INITING;

bool initwarning(const char *desc, int level, int type)
{
    if(initing < level)
    {
        addchange(desc, type);
        return true;
    }
    return false;
}

#define SCR_MINW 320
#define SCR_MINH 200
#define SCR_MAXW 10000
#define SCR_MAXH 10000
#define SCR_DEFAULTW 1024
#define SCR_DEFAULTH 768
VARFN(screenw, scr_w, SCR_MINW, -1, SCR_MAXW, initwarning("screen resolution"));
VARFN(screenh, scr_h, SCR_MINH, -1, SCR_MAXH, initwarning("screen resolution"));

void writeinitcfg()
{
    stream *f = openutf8file("config/init.cfg", "w");
    if(!f) return;
    f->printf("// automatically written on exit, DO NOT MODIFY\n// modify settings in game\n");
    extern int fullscreen;
    f->printf("fullscreen %d\n", fullscreen);
    f->printf("screenw %d\n", scr_w);
    f->printf("screenh %d\n", scr_h);
    extern int sound, soundchans, soundfreq, soundbufferlen;
    f->printf("sound %d\n", sound);
    f->printf("soundchans %d\n", soundchans);
    f->printf("soundfreq %d\n", soundfreq);
    f->printf("soundbufferlen %d\n", soundbufferlen);
    delete f;
}

COMMAND(quit, "");

static void getbackgroundres(int &w, int &h)
{
    float wk = 1, hk = 1;
    if(w < 1024) wk = 1024.0f/w;
    if(h < 768) hk = 768.0f/h;
    wk = hk = max(wk, hk);
    w = int(ceil(w*wk));
    h = int(ceil(h*hk));
}

string backgroundcaption = "";
Texture *backgroundmapshot = NULL;
string backgroundmapname = "";
char *backgroundmapinfo = NULL;

void bgquad(float x, float y, float w, float h, float tx = 0, float ty = 0, float tw = 1, float th = 1)
{
    gle::begin(GL_TRIANGLE_STRIP);
    gle::attribf(x,   y);   gle::attribf(tx,      ty);
    gle::attribf(x+w, y);   gle::attribf(tx + tw, ty);
    gle::attribf(x,   y+h); gle::attribf(tx,      ty + th);
    gle::attribf(x+w, y+h); gle::attribf(tx + tw, ty + th);
    gle::end();
}

void renderbackgroundview(int w, int h, const char *caption, Texture *mapshot, const char *mapname, const char *mapinfo)
{
    static int lastupdate = -1, lastw = -1, lasth = -1;
    static float backgroundu = 0, backgroundv = 0;
    if((renderedframe && !mainmenu && lastupdate != lastmillis) || lastw != w || lasth != h)
    {
        lastupdate = lastmillis;
        lastw = w;
        lasth = h;

        backgroundu = rndscale(1);
        backgroundv = rndscale(1);
    }
    else if(lastupdate != lastmillis) lastupdate = lastmillis;

    hudmatrix.ortho(0, w, h, 0, -1, 1);
    resethudmatrix();
    hudshader->set();

    gle::defvertex(2);
    gle::deftexcoord0();

    gle::colorf(1, 1, 1);
    settexture("media/interface/background.png", 0);
    float bu = w*0.67f/256.0f, bv = h*0.67f/256.0f;
    bgquad(0, 0, w, h, backgroundu, backgroundv, bu, bv);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    settexture("media/interface/shadow.png", 3);
    bgquad(0, 0, w, h);

    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    float lh = 0.5f*min(w, h), lw = lh*2,
          lx = 0.5f*(w - lw), ly = 0.5f*(h*0.5f - lh);
    settexture((maxtexsize ? min(maxtexsize, hwtexsize) : hwtexsize) >= 1024 && (hudw > 1280 || hudh > 800) ? "<premul>media/interface/logo_1024.png" : "<premul>media/interface/logo.png", 3);
    bgquad(lx, ly, lw, lh);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if(caption)
    {
        int tw = text_width(caption);
        float tsz = 0.04f*min(w, h)/FONTH,
              tx = 0.5f*(w - tw*tsz), ty = h - 0.075f*1.5f*min(w, h) - FONTH*tsz;
        pushhudmatrix();
        hudmatrix.translate(tx, ty, 0);
        hudmatrix.scale(tsz, tsz, 1);
        flushhudmatrix();
        draw_text(caption, 0, 0);
        pophudmatrix();
    }
    if(mapshot || mapname)
    {
        float infowidth = 14*FONTH, sz = 0.35f*min(w, h), msz = (0.85f*min(w, h) - sz)/(infowidth + FONTH), x = 0.5f*w, y = ly+lh - sz/15, mx = 0, my = 0, mw = 0, mh = 0;
        if(mapinfo)
        {
            text_boundsf(mapinfo, mw, mh, infowidth);
            x -= 0.5f*mw*msz;
            if(mapshot && mapshot!=notexture)
            {
                x -= 0.5f*FONTH*msz;
                mx = sz + FONTH*msz;
            }
        }
        if(mapshot && mapshot!=notexture)
        {
            x -= 0.5f*sz;
            glBindTexture(GL_TEXTURE_2D, mapshot->id);
            bgquad(x, y, sz, sz);
        }
        if(mapname)
        {
            float tw = text_widthf(mapname), tsz = sz/(8*FONTH), tx = max(0.5f*(mw*msz - tw*tsz), 0.0f);
            pushhudmatrix();
            hudmatrix.translate(x+mx+tx, y, 0);
            hudmatrix.scale(tsz, tsz, 1);
            flushhudmatrix();
            draw_text(mapname, 0, 0);
            pophudmatrix();
            my = 1.5f*FONTH*tsz;
        }
        if(mapinfo)
        {
            pushhudmatrix();
            hudmatrix.translate(x+mx, y+my, 0);
            hudmatrix.scale(msz, msz, 1);
            flushhudmatrix();
            draw_text(mapinfo, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, -1, infowidth);
            pophudmatrix();
        }
    }

    glDisable(GL_BLEND);

    gle::disable();
}

void renderbackground(const char *caption, Texture *mapshot, const char *mapname, const char *mapinfo, bool force)
{
    if(!inbetweenframes && !force) return;

    stopsounds(); // stop sounds while loading

    int w = hudw, h = hudh;
    if(forceaspect) w = int(ceil(h*forceaspect));
    getbackgroundres(w, h);
    gettextres(w, h);

    if(force)
    {
        renderbackgroundview(w, h, caption, mapshot, mapname, mapinfo);
        return;
    }

    loopi(3)
    {
        if(ovr::enabled)
        {
            aspect = forceaspect ? forceaspect : hudw/float(hudh);
            for(viewidx = 0; viewidx < 2; viewidx++, hudx += hudw)
            {
                if(!i)
                {
                    glBindFramebuffer_(GL_FRAMEBUFFER, ovr::lensfbo[viewidx]);
                    glViewport(0, 0, hudw, hudh);
                    glClearColor(0, 0, 0, 0);
                    glClear(GL_COLOR_BUFFER_BIT);
                    renderbackgroundview(w, h, caption, mapshot, mapname, mapinfo);
                }
                ovr::warp();
            }
            viewidx = 0;
            hudx = 0;
        }
        else renderbackgroundview(w, h, caption, mapshot, mapname, mapinfo);
        swapbuffers(false);
    }

    renderedframe = false;
    copystring(backgroundcaption, caption ? caption : "");
    backgroundmapshot = mapshot;
    copystring(backgroundmapname, mapname ? mapname : "");
    if(mapinfo != backgroundmapinfo)
    {
        DELETEA(backgroundmapinfo);
        if(mapinfo) backgroundmapinfo = newstring(mapinfo);
    }
}

void restorebackground(int w, int h)
{
    if(renderedframe) return;
    renderbackgroundview(w, h, backgroundcaption[0] ? backgroundcaption : NULL, backgroundmapshot, backgroundmapname[0] ? backgroundmapname : NULL, backgroundmapinfo);
}

float loadprogress = 0;

void renderprogressview(int w, int h, float bar, const char *text)   // also used during loading
{
    hudmatrix.ortho(0, w, h, 0, -1, 1);
    resethudmatrix();
    hudshader->set();

    gle::defvertex(2);
    gle::deftexcoord0();

    gle::colorf(1, 1, 1);

    float fh = 0.060f*min(w, h), fw = fh*15,
          fx = renderedframe ? w - fw - fh/4 : 0.5f*(w - fw),
          fy = renderedframe ? fh/4 : h - fh*1.5f;
    settexture("media/interface/loading_frame.png", 3);
    bgquad(fx, fy, fw, fh);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float bw = fw*(512 - 2*8)/512.0f, bh = fh*20/32.0f,
          bx = fx + fw*8/512.0f, by = fy + fh*6/32.0f,
          su1 = 0/32.0f, su2 = 8/32.0f, sw = fw*8/512.0f,
          eu1 = 24/32.0f, eu2 = 32/32.0f, ew = fw*8/512.0f,
          mw = bw - sw - ew,
          ex = bx+sw + max(mw*bar, fw*8/512.0f);
    if(bar > 0)
    {
        settexture("media/interface/loading_bar.png", 3);
        bgquad(bx, by, sw, bh, su1, 0, su2-su1, 1);
        bgquad(bx+sw, by, ex-(bx+sw), bh, su2, 0, eu1-su2, 1);
        bgquad(ex, by, ew, bh, eu1, 0, eu2-eu1, 1);
    }

    if(text)
    {
        int tw = text_width(text);
        float tsz = bh*0.6f/FONTH;
        if(tw*tsz > mw) tsz = mw/tw;
        pushhudmatrix();
        hudmatrix.translate(bx+sw, by + (bh - FONTH*tsz)/2, 0);
        hudmatrix.scale(tsz, tsz, 1);
        flushhudmatrix();
        draw_text(text, 0, 0);
        pophudmatrix();
    }

    glDisable(GL_BLEND);

    gle::disable();
}

void renderprogress(float bar, const char *text, bool background)   // also used during loading
{
    if(!inbetweenframes || drawtex) return;

    clientkeepalive();      // make sure our connection doesn't time out while loading maps etc.

    #ifdef __APPLE__
    interceptkey(SDLK_UNKNOWN); // keep the event queue awake to avoid 'beachball' cursor
    #endif

    int w = hudw, h = hudh;
    if(forceaspect) w = int(ceil(h*forceaspect));
    getbackgroundres(w, h);
    gettextres(w, h);

    if(ovr::enabled)
    {
        aspect = forceaspect ? forceaspect : hudw/float(hudh);
        for(viewidx = 0; viewidx < 2; viewidx++, hudx += hudw)
        {
            glBindFramebuffer_(GL_FRAMEBUFFER, ovr::lensfbo[viewidx]);
            glViewport(0, 0, hudw, hudh);
            if(background)
            {
                glClearColor(0, 0, 0, 0);
                glClear(GL_COLOR_BUFFER_BIT);
                restorebackground(w, h);
            }
            renderprogressview(w, h, bar, text);
            ovr::warp();
        }
        viewidx = 0;
        hudx = 0;
    }
    else
    {
        if(background) restorebackground(w, h);
        renderprogressview(w, h, bar, text);
    }
    swapbuffers(false);
}

VARNP(relativemouse, userelativemouse, 0, 1, 1);

bool shouldgrab = false, grabinput = false, minimized = false, canrelativemouse = true, relativemouse = false;
int keyrepeatmask = 0, textinputmask = 0;

void keyrepeat(bool on, int mask)
{
    if(on) keyrepeatmask |= mask;
    else keyrepeatmask &= ~mask;
}

void textinput(bool on, int mask)
{
    if(on)
    {
        if(!textinputmask) SDL_StartTextInput();
        textinputmask |= mask;
    }
    else
    {
        textinputmask &= ~mask;
        if(!textinputmask) SDL_StopTextInput();
    }
}

void inputgrab(bool on)
{
    if(on)
    {
        SDL_ShowCursor(SDL_FALSE);
        if(canrelativemouse && userelativemouse)
        {
            if(SDL_SetRelativeMouseMode(SDL_TRUE) >= 0)
            {
                SDL_SetWindowGrab(screen, SDL_TRUE);
                relativemouse = true;
            }
            else
            {
                SDL_SetWindowGrab(screen, SDL_FALSE);
                canrelativemouse = false;
                relativemouse = false;
            }
        }
    }
    else
    {
        SDL_ShowCursor(SDL_TRUE);
        if(relativemouse)
        {
            SDL_SetWindowGrab(screen, SDL_FALSE);
            SDL_SetRelativeMouseMode(SDL_FALSE);
            relativemouse = false;
        }
    }
    shouldgrab = false;
}

bool initwindowpos = false;

void setfullscreen(bool enable)
{
    if(!screen) return;
    //initwarning(enable ? "fullscreen" : "windowed");
    SDL_SetWindowFullscreen(screen, enable ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
    if(!enable)
    {
        SDL_SetWindowSize(screen, scr_w, scr_h);
        if(initwindowpos)
        {
            int winx = SDL_WINDOWPOS_CENTERED, winy = SDL_WINDOWPOS_CENTERED;
            if(ovr::enabled) winx = winy = 0;
            SDL_SetWindowPosition(screen, winx, winy);
            initwindowpos = false;
        }
    }
}

#ifdef _DEBUG
VARF(fullscreen, 0, 0, 1, setfullscreen(fullscreen!=0));
#else
VARF(fullscreen, 0, 1, 1, setfullscreen(fullscreen!=0));
#endif

void screenres(int w, int h)
{
    scr_w = clamp(w, SCR_MINW, SCR_MAXW);
    scr_h = clamp(h, SCR_MINH, SCR_MAXH);
    if(screen)
    {
        scr_w = min(scr_w, desktopw);
        scr_h = min(scr_h, desktoph);
        if(SDL_GetWindowFlags(screen) & SDL_WINDOW_FULLSCREEN) gl_resize();
        else SDL_SetWindowSize(screen, scr_w, scr_h);
    }
    else
    {
        initwarning("screen resolution");
    }
}

ICOMMAND(screenres, "ii", (int *w, int *h), screenres(*w, *h));

static void setgamma(int val)
{
    if(screen && SDL_SetWindowBrightness(screen, val/100.0f) < 0) conoutf(CON_ERROR, "Could not set gamma: %s", SDL_GetError());
}

static int curgamma = 100;
VARFP(gamma, 30, 100, 300,
{
    if(initing || gamma == curgamma) return;
    curgamma = gamma;
    setgamma(curgamma);
});

void restoregamma()
{
    if(initing || curgamma == 100) return;
    setgamma(curgamma);
}

void cleargamma()
{
    if(curgamma != 100 && screen) SDL_SetWindowBrightness(screen, 1.0f);
}

void restorevsync()
{
    if(initing || !glcontext) return;
    extern int vsync, vsynctear;
    SDL_GL_SetSwapInterval(vsync ? (vsynctear ? -1 : 1) : 0);
}

VARFP(vsync, 0, 0, 1, restorevsync());
VARFP(vsynctear, 0, 0, 1, { if(vsync) restorevsync(); });

VAR(dbgmodes, 0, 0, 1);

void setupscreen()
{
    if(glcontext)
    {
        SDL_GL_DeleteContext(glcontext);
        glcontext = NULL;
    }
    if(screen)
    {
        SDL_DestroyWindow(screen);
        screen = NULL;
    }

    SDL_DisplayMode desktop;
    if(SDL_GetDesktopDisplayMode(0, &desktop) < 0) fatal("failed querying desktop display mode: %s", SDL_GetError());
    desktopw = desktop.w;
    desktoph = desktop.h;

    if(scr_h < 0) scr_h = SCR_DEFAULTH;
    if(scr_w < 0) scr_w = (scr_h*desktopw)/desktoph;
    scr_w = min(scr_w, desktopw);
    scr_h = min(scr_h, desktoph);

    int winx = SDL_WINDOWPOS_UNDEFINED, winy = SDL_WINDOWPOS_UNDEFINED, winw = scr_w, winh = scr_h, flags = SDL_WINDOW_RESIZABLE;
    if(fullscreen)
    {
        winw = desktopw;
        winh = desktoph;
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        initwindowpos = true;
    }
    if(ovr::enabled) winx = winy = 0;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    screen = SDL_CreateWindow("Tesseract", winx, winy, winw, winh, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS | flags);
    if(!screen) fatal("failed to create OpenGL window: %s", SDL_GetError());

    SDL_SetWindowMinimumSize(screen, SCR_MINW, SCR_MINH);
    SDL_SetWindowMaximumSize(screen, SCR_MAXW, SCR_MAXH);

    static const struct { int major, minor; } coreversions[] = { { 4, 0 }, { 3, 3 }, { 3, 2 }, { 3, 1 }, { 3, 0 } };
    loopi(sizeof(coreversions)/sizeof(coreversions[0]))
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, coreversions[i].major);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, coreversions[i].minor);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        glcontext = SDL_GL_CreateContext(screen);
        if(glcontext) break;
    }
    if(!glcontext)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 0);
        glcontext = SDL_GL_CreateContext(screen);
        if(!glcontext) fatal("failed to create OpenGL context: %s", SDL_GetError());
    }

    SDL_GetWindowSize(screen, &screenw, &screenh);
    renderw = min(scr_w, screenw);
    renderh = min(scr_h, screenh);
    hudw = screenw;
    hudh = screenh;
}

void resetgl()
{
    clearchanges(CHANGE_GFX|CHANGE_SHADERS);

    renderbackground("resetting OpenGL");

    recorder::cleanup();
    cleanupva();
    cleanupparticles();
    cleanupstains();
    cleanupsky();
    cleanupmodels();
    cleanupprefabs();
    cleanuptextures();
    cleanupblendmap();
    cleanuplights();
    cleanupshaders();
    cleanupgl();

    setupscreen();

    inputgrab(grabinput);

    gl_init();

    inbetweenframes = false;
    if(!reloadtexture(*notexture) ||
       !reloadtexture("<premul>media/interface/logo.png") ||
       !reloadtexture("<premul>media/interface/logo_1024.png") ||
       !reloadtexture("media/interface/background.png") ||
       !reloadtexture("media/interface/shadow.png") ||
       !reloadtexture("media/interface/mapshot_frame.png") ||
       !reloadtexture("media/interface/loading_frame.png") ||
       !reloadtexture("media/interface/loading_bar.png"))
        fatal("failed to reload core texture");
    reloadfonts();
    inbetweenframes = true;
    renderbackground("initializing...");
    restoregamma();
    restorevsync();
    initgbuffer();
    reloadshaders();
    reloadtextures();
    initlights();
    allchanged(true);
}

COMMAND(resetgl, "");

vector<SDL_Event> events;

void pushevent(const SDL_Event &e)
{
    events.add(e);
}

static bool filterevent(const SDL_Event &event)
{
    switch(event.type)
    {
        case SDL_MOUSEMOTION:
            if(grabinput && !relativemouse && !(SDL_GetWindowFlags(screen) & SDL_WINDOW_FULLSCREEN))
            {
                if(event.motion.x == screenw / 2 && event.motion.y == screenh / 2)
                    return false;  // ignore any motion events generated by SDL_WarpMouse
                #ifdef __APPLE__
                if(event.motion.y == 0)
                    return false;  // let mac users drag windows via the title bar
                #endif
            }
            break;
    }
    return true;
}

static inline bool pollevent(SDL_Event &event)
{
    while(SDL_PollEvent(&event))
    {
        if(filterevent(event)) return true;
    }
    return false;
}

bool interceptkey(int sym)
{
    static int lastintercept = SDLK_UNKNOWN;
    int len = lastintercept == sym ? events.length() : 0;
    SDL_Event event;
    while(pollevent(event))
    {
        switch(event.type)
        {
            case SDL_MOUSEMOTION: break;
            default: pushevent(event); break;
        }
    }
    lastintercept = sym;
    if(sym != SDLK_UNKNOWN) for(int i = len; i < events.length(); i++)
    {
        if(events[i].type == SDL_KEYDOWN && events[i].key.keysym.sym == sym) { events.remove(i); return true; }
    }
    return false;
}

static void ignoremousemotion()
{
    SDL_Event e;
    SDL_PumpEvents();
    while(SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_MOUSEMOTION, SDL_MOUSEMOTION));
}

static void resetmousemotion()
{
    if(grabinput && !relativemouse && !(SDL_GetWindowFlags(screen) & SDL_WINDOW_FULLSCREEN))
    {
        SDL_WarpMouseInWindow(screen, screenw / 2, screenh / 2);
    }
}

static void checkmousemotion(int &dx, int &dy)
{
    loopv(events)
    {
        SDL_Event &event = events[i];
        if(event.type != SDL_MOUSEMOTION)
        {
            if(i > 0) events.remove(0, i);
            return;
        }
        dx += event.motion.xrel;
        dy += event.motion.yrel;
    }
    events.setsize(0);
    SDL_Event event;
    while(pollevent(event))
    {
        if(event.type != SDL_MOUSEMOTION)
        {
            events.add(event);
            return;
        }
        dx += event.motion.xrel;
        dy += event.motion.yrel;
    }
}

void checkinput()
{
    SDL_Event event;
    //int lasttype = 0, lastbut = 0;
    bool mousemoved = false;
    while(events.length() || pollevent(event))
    {
        if(events.length()) event = events.remove(0);

        switch(event.type)
        {
            case SDL_QUIT:
                quit();
                return;

            case SDL_TEXTINPUT:
            {
                uchar buf[SDL_TEXTINPUTEVENT_TEXT_SIZE+1];
                size_t len = decodeutf8(buf, sizeof(buf)-1, (const uchar *)event.text.text, strlen(event.text.text));
                if(len > 0) { buf[len] = '\0'; processtextinput((const char *)buf, len); }
                break;
            }

            case SDL_KEYDOWN:
            case SDL_KEYUP:
                if(keyrepeatmask || !event.key.repeat)
                    processkey(event.key.keysym.sym, event.key.state==SDL_PRESSED);
                break;

            case SDL_WINDOWEVENT:
                switch(event.window.event)
                {
                    case SDL_WINDOWEVENT_CLOSE:
                        quit();
                        break;

                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        shouldgrab = true;
                        break;
                    case SDL_WINDOWEVENT_ENTER:
                        inputgrab(grabinput = true);
                        break;

                    case SDL_WINDOWEVENT_LEAVE:
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                        inputgrab(grabinput = false);
                        break;

                    case SDL_WINDOWEVENT_MINIMIZED:
                        minimized = true;
                        break;

                    case SDL_WINDOWEVENT_MAXIMIZED:
                    case SDL_WINDOWEVENT_RESTORED:
                        minimized = false;
                        break;

                    case SDL_WINDOWEVENT_RESIZED:
                        break;

                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        SDL_GetWindowSize(screen, &screenw, &screenh);
                        if(!(SDL_GetWindowFlags(screen) & SDL_WINDOW_FULLSCREEN))
                        {
                            scr_w = clamp(screenw, SCR_MINW, SCR_MAXW);
                            scr_h = clamp(screenh, SCR_MINH, SCR_MAXH);
                        }
                        gl_resize();
                        break;
                }
                break;

            case SDL_MOUSEMOTION:
                if(grabinput)
                {
                    int dx = event.motion.xrel, dy = event.motion.yrel;
                    checkmousemotion(dx, dy);
                    if(!UI::movecursor(dx, dy)) mousemove(dx, dy);
                    mousemoved = true;
                }
                else if(shouldgrab) inputgrab(grabinput = true);
                break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                //if(lasttype==event.type && lastbut==event.button.button) break; // why?? get event twice without it
                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT: processkey(-1, event.button.state==SDL_PRESSED); break;
                    case SDL_BUTTON_MIDDLE: processkey(-2, event.button.state==SDL_PRESSED); break;
                    case SDL_BUTTON_RIGHT: processkey(-3, event.button.state==SDL_PRESSED); break;
                    case SDL_BUTTON_X1: processkey(-6, event.button.state==SDL_PRESSED); break;
                    case SDL_BUTTON_X2: processkey(-7, event.button.state==SDL_PRESSED); break;
                }
                //lasttype = event.type;
                //lastbut = event.button.button;
                break;

            case SDL_MOUSEWHEEL:
                if(event.wheel.y > 0) { processkey(-4, true); processkey(-4, false); }
                else if(event.wheel.y < 0) { processkey(-5, true); processkey(-5, false); }
                break;
        }
    }
    if(mousemoved) resetmousemotion();
}

void swapbuffers(bool overlay)
{
    recorder::capture(overlay);
    SDL_GL_SwapWindow(screen);
}

VAR(menufps, 0, 60, 1000);
VARP(maxfps, 0, 125, 1000);

void limitfps(int &millis, int curmillis)
{
    int limit = (mainmenu || minimized) && menufps ? (maxfps ? min(maxfps, menufps) : menufps) : maxfps;
    if(!limit) return;
    static int fpserror = 0;
    int delay = 1000/limit - (millis-curmillis);
    if(delay < 0) fpserror = 0;
    else
    {
        fpserror += 1000%limit;
        if(fpserror >= limit)
        {
            ++delay;
            fpserror -= limit;
        }
        if(delay > 0)
        {
            SDL_Delay(delay);
            millis += delay;
        }
    }
}

#if defined(WIN32) && !defined(_DEBUG) && !defined(__GNUC__)
void stackdumper(unsigned int type, EXCEPTION_POINTERS *ep)
{
    if(!ep) fatal("unknown type");
    EXCEPTION_RECORD *er = ep->ExceptionRecord;
    CONTEXT *context = ep->ContextRecord;
    char out[512];
    formatstring(out, "Tesseract Win32 Exception: 0x%x [0x%x]\n\n", er->ExceptionCode, er->ExceptionCode==EXCEPTION_ACCESS_VIOLATION ? er->ExceptionInformation[1] : -1);
    SymInitialize(GetCurrentProcess(), NULL, TRUE);
#ifdef _AMD64_
    STACKFRAME64 sf = {{context->Rip, 0, AddrModeFlat}, {}, {context->Rbp, 0, AddrModeFlat}, {context->Rsp, 0, AddrModeFlat}, 0};
    while(::StackWalk64(IMAGE_FILE_MACHINE_AMD64, GetCurrentProcess(), GetCurrentThread(), &sf, context, NULL, ::SymFunctionTableAccess, ::SymGetModuleBase, NULL))
    {
        union { IMAGEHLP_SYMBOL64 sym; char symext[sizeof(IMAGEHLP_SYMBOL64) + sizeof(string)]; };
        sym.SizeOfStruct = sizeof(sym);
        sym.MaxNameLength = sizeof(symext) - sizeof(sym);
        IMAGEHLP_LINE64 line;
        line.SizeOfStruct = sizeof(line);
        DWORD64 symoff;
        DWORD lineoff;
        if(SymGetSymFromAddr64(GetCurrentProcess(), sf.AddrPC.Offset, &symoff, &sym) && SymGetLineFromAddr64(GetCurrentProcess(), sf.AddrPC.Offset, &lineoff, &line))
#else
    STACKFRAME sf = {{context->Eip, 0, AddrModeFlat}, {}, {context->Ebp, 0, AddrModeFlat}, {context->Esp, 0, AddrModeFlat}, 0};
    while(::StackWalk(IMAGE_FILE_MACHINE_I386, GetCurrentProcess(), GetCurrentThread(), &sf, context, NULL, ::SymFunctionTableAccess, ::SymGetModuleBase, NULL))
    {
        union { IMAGEHLP_SYMBOL sym; char symext[sizeof(IMAGEHLP_SYMBOL) + sizeof(string)]; };
        sym.SizeOfStruct = sizeof(sym);
        sym.MaxNameLength = sizeof(symext) - sizeof(sym);
        IMAGEHLP_LINE line;
        line.SizeOfStruct = sizeof(line);
        DWORD symoff, lineoff;
        if(SymGetSymFromAddr(GetCurrentProcess(), sf.AddrPC.Offset, &symoff, &sym) && SymGetLineFromAddr(GetCurrentProcess(), sf.AddrPC.Offset, &lineoff, &line))
#endif
        {
            char *del = strrchr(line.FileName, '\\');
            concformatstring(out, "%s - %s [%d]\n", sym.Name, del ? del + 1 : line.FileName, line.LineNumber);
        }
    }
    fatal(out);
}
#endif

#define MAXFPSHISTORY 60

int fpspos = 0, fpshistory[MAXFPSHISTORY];

void resetfpshistory()
{
    loopi(MAXFPSHISTORY) fpshistory[i] = 1;
    fpspos = 0;
}

void updatefpshistory(int millis)
{
    fpshistory[fpspos++] = max(1, min(1000, millis));
    if(fpspos>=MAXFPSHISTORY) fpspos = 0;
}

void getframemillis(float &avg, float &bestdiff, float &worstdiff)
{
    int total = fpshistory[MAXFPSHISTORY-1], best = total, worst = total;
    loopi(MAXFPSHISTORY-1)
    {
        int millis = fpshistory[i];
        total += millis;
        if(millis < best) best = millis;
        if(millis > worst) worst = millis;
    }

    avg = total/float(MAXFPSHISTORY);
    best = best - avg;
    worstdiff = avg - worst;
}

void getfps(int &fps, int &bestdiff, int &worstdiff)
{
    int total = fpshistory[MAXFPSHISTORY-1], best = total, worst = total;
    loopi(MAXFPSHISTORY-1)
    {
        int millis = fpshistory[i];
        total += millis;
        if(millis < best) best = millis;
        if(millis > worst) worst = millis;
    }

    fps = (1000*MAXFPSHISTORY)/total;
    bestdiff = 1000/best-fps;
    worstdiff = fps-1000/worst;
}

void getfps_(int *raw)
{
    if(*raw) floatret(1000.0f/fpshistory[(fpspos+MAXFPSHISTORY-1)%MAXFPSHISTORY]);
    else
    {
        int fps, bestdiff, worstdiff;
        getfps(fps, bestdiff, worstdiff);
        intret(fps);
    }
}

COMMANDN(getfps, getfps_, "i");

bool inbetweenframes = false, renderedframe = true;

static bool findarg(int argc, char **argv, const char *str)
{
    for(int i = 1; i<argc; i++) if(strstr(argv[i], str)==argv[i]) return true;
    return false;
}

static int clockrealbase = 0, clockvirtbase = 0;
static void clockreset() { clockrealbase = SDL_GetTicks(); clockvirtbase = totalmillis; }
VARFP(clockerror, 990000, 1000000, 1010000, clockreset());
VARFP(clockfix, 0, 0, 1, clockreset());

int getclockmillis()
{
    int millis = SDL_GetTicks() - clockrealbase;
    if(clockfix) millis = int(millis*(double(clockerror)/1000000));
    millis += clockvirtbase;
    return max(millis, totalmillis);
}

VAR(numcpus, 1, 1, 16);

int main(int argc, char **argv)
{
    #ifdef WIN32
    //atexit((void (__cdecl *)(void))_CrtDumpMemoryLeaks);
    #ifndef _DEBUG
    #ifndef __GNUC__
    __try {
    #endif
    #endif
    #endif

    setlogfile(NULL);

    int dedicated = 0;
    char *load = NULL, *initscript = NULL;

    initing = INIT_RESET;
    for(int i = 1; i<argc; i++)
    {
        if(argv[i][0]=='-') switch(argv[i][1])
        {
            case 'u':
            {
                const char *dir = sethomedir(&argv[i][2]);
                if(dir) logoutf("Using home directory: %s", dir);
                break;
            }
        }
    }
    execfile("config/init.cfg", false);
    for(int i = 1; i<argc; i++)
    {
        if(argv[i][0]=='-') switch(argv[i][1])
        {
            case 'u': /* parsed first */ break;
            case 'k':
            {
                const char *dir = addpackagedir(&argv[i][2]);
                if(dir) logoutf("Adding package directory: %s", dir);
                break;
            }
            case 'g': logoutf("Setting log file: %s", &argv[i][2]); setlogfile(&argv[i][2]); break;
            case 'd': dedicated = atoi(&argv[i][2]); if(dedicated<=0) dedicated = 2; break;
            case 'w': scr_w = clamp(atoi(&argv[i][2]), SCR_MINW, SCR_MAXW); if(!findarg(argc, argv, "-h")) scr_h = -1; break;
            case 'h': scr_h = clamp(atoi(&argv[i][2]), SCR_MINH, SCR_MAXH); if(!findarg(argc, argv, "-w")) scr_w = -1; break;
            case 'v': vsync = atoi(&argv[i][2]); if(vsync < 0) { vsynctear = 1; vsync = 1; } else vsynctear = 0; break;
            case 'f': fullscreen = atoi(&argv[i][2]); break;
            case 'l':
            {
                char pkgdir[] = "media/";
                load = strstr(path(&argv[i][2]), path(pkgdir));
                if(load) load += sizeof(pkgdir)-1;
                else load = &argv[i][2];
                break;
            }
            case 'x': initscript = &argv[i][2]; break;
            default: if(!serveroption(argv[i])) gameargs.add(argv[i]); break;
        }
        else gameargs.add(argv[i]);
    }

    numcpus = clamp(SDL_GetCPUCount(), 1, 16);

    if(dedicated <= 1)
    {
        logoutf("init: sdl");

        int par = 0;
        #ifdef _DEBUG
        par = SDL_INIT_NOPARACHUTE;
        #endif

        if(SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO|SDL_INIT_AUDIO|par)<0) fatal("Unable to initialize SDL: %s", SDL_GetError());
    }

    logoutf("init: net");
    if(enet_initialize()<0) fatal("Unable to initialise network module");
    atexit(enet_deinitialize);
    enet_time_set(0);

    logoutf("init: game");
    game::parseoptions(gameargs);
    initserver(dedicated>0, dedicated>1);  // never returns if dedicated
    ASSERT(dedicated <= 1);
    game::initclient();

    logoutf("init: video");
    SDL_SetHint(SDL_HINT_GRAB_KEYBOARD, "0");
    #if !defined(WIN32) && !defined(__APPLE__)
    SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
    #endif
    setupscreen();
    SDL_ShowCursor(SDL_FALSE);
    SDL_StopTextInput(); // workaround for spurious text-input events getting sent on first text input toggle?

    logoutf("init: gl");
    gl_checkextensions();
    gl_init();
    notexture = textureload("media/texture/game/notexture.png");
    if(!notexture) fatal("could not find core textures");

    logoutf("init: console");
    if(!execfile("config/stdlib.cfg", false)) fatal("cannot find data files (you are running from the wrong folder, try .bat file in the main folder)");   // this is the first file we load.
    if(!execfile("config/font.cfg", false)) fatal("cannot find font definitions");
    if(!setfont("default")) fatal("no default font specified");

    UI::setup();

    inbetweenframes = true;
    renderbackground("initializing...");

    logoutf("init: world");
    camera1 = player = game::iterdynents(0);
    emptymap(0, true, NULL, false);

    logoutf("init: sound");
    initsound();

    logoutf("init: cfg");
    initing = INIT_LOAD;
    execfile("config/keymap.cfg");
    execfile("config/stdedit.cfg");
    execfile(game::gameconfig());
    execfile("config/sound.cfg");
    execfile("config/ui.cfg");
    execfile("config/heightmap.cfg");
    execfile("config/blendbrush.cfg");
    if(game::savedservers()) execfile(game::savedservers(), false);

    identflags |= IDF_PERSIST;

    if(!execfile(game::savedconfig(), false))
    {
        execfile(game::defaultconfig());
        writecfg(game::restoreconfig());
    }
    execfile(game::autoexec(), false);

    identflags &= ~IDF_PERSIST;

    initing = INIT_GAME;
    game::loadconfigs();

    initing = NOT_INITING;

    logoutf("init: render");
    restoregamma();
    restorevsync();
    initgbuffer();
    loadshaders();
    initparticles();
    initstains();

    identflags |= IDF_PERSIST;

    logoutf("init: mainloop");

    if(execfile("once.cfg", false)) remove(findfile("once.cfg", "rb"));

    if(load)
    {
        logoutf("init: localconnect");
        //localconnect();
        game::changemap(load);
    }

    if(initscript) execute(initscript);

    initmumble();
    resetfpshistory();

    inputgrab(grabinput = true);
    ignoremousemotion();

    for(;;)
    {
        static int frames = 0;
        int millis = getclockmillis();
        limitfps(millis, totalmillis);
        elapsedtime = millis - totalmillis;
        static int timeerr = 0;
        int scaledtime = game::scaletime(elapsedtime) + timeerr;
        curtime = scaledtime/100;
        timeerr = scaledtime%100;
        if(!multiplayer(false) && curtime>200) curtime = 200;
        if(game::ispaused()) curtime = 0;
        lastmillis += curtime;
        totalmillis = millis;
        updatetime();

        checkinput();
        ovr::update();
        UI::update();
        menuprocess();
        tryedit();

        if(lastmillis) game::updateworld();

        checksleep(lastmillis);

        serverslice(false, 0);

        if(frames) updatefpshistory(elapsedtime);
        frames++;

        // miscellaneous general game effects
        recomputecamera();
        updateparticles();
        updatesounds();

        if(minimized) continue;

        gl_setupframe(!mainmenu);

        inbetweenframes = false;
        gl_drawframe();
        swapbuffers();
        renderedframe = inbetweenframes = true;
    }

    ASSERT(0);
    return EXIT_FAILURE;

    #if defined(WIN32) && !defined(_DEBUG) && !defined(__GNUC__)
    } __except(stackdumper(0, GetExceptionInformation()), EXCEPTION_CONTINUE_SEARCH) { return 0; }
    #endif
}
