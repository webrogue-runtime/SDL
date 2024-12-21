// /*
//   Simple DirectMedia Layer
//   Copyright (C) 1997-2024 Sam Lantinga <slouken@libsdl.org>

//   This software is provided 'as-is', without any express or implied
//   warranty.  In no event will the authors be held liable for any damages
//   arising from the use of this software.

//   Permission is granted to anyone to use this software for any purpose,
//   including commercial applications, and to alter it and redistribute it
//   freely, subject to the following restrictions:

//   1. The origin of this software must not be misrepresented; you must not
//      claim that you wrote the original software. If you use this software
//      in a product, an acknowledgment in the product documentation would be
//      appreciated but is not required.
//   2. Altered source versions must be plainly marked as such, and must not be
//      misrepresented as being the original software.
//   3. This notice may not be removed or altered from any source distribution.
// */
#include "../../SDL_internal.h"

#ifdef SDL_VIDEO_DRIVER_WEBROGUE

#include <webrogue_gfx/webrogue_gfx.h>
#include "SDL_webrogue_unimplemented.h"

#include "../SDL_sysvideo.h"
#include "SDL_webrogueopengles.h"
#include "SDL_webrogueevents_c.h"
#include "SDL_webrogueframebuffer_c.h"
#include "SDL_webrogueswkb.h"
// #include "SDL_n3dstouch.h"
#include "SDL_webroguevideo.h"

#define WEBROGUEVID_DRIVER_NAME "webrogue"

SDL_FORCE_INLINE int AddWEBROGUEDisplay();

static int WEBROGUE_VideoInit(_THIS);
static void WEBROGUE_VideoQuit(_THIS);
static void WEBROGUE_GetDisplayModes(_THIS, SDL_VideoDisplay *display);
static int WEBROGUE_SetDisplayMode(_THIS, SDL_VideoDisplay *display, SDL_DisplayMode *mode);
static int WEBROGUE_GetDisplayBounds(_THIS, SDL_VideoDisplay *display, SDL_Rect *rect);
static int WEBROGUE_CreateWindow(_THIS, SDL_Window *window);
static void WEBROGUE_DestroyWindow(_THIS, SDL_Window *window);

typedef struct
{
} DisplayDriverData;

typedef struct
{
} ModeDriverData;

// static const struct
// {
//     SDL_PixelFormatEnum pixfmt;
//     GSPGPU_FramebufferFormat gspfmt;
// } format_map[] = {
//     { SDL_PIXELFORMAT_RGBA8888, GSP_RGBA8_OES },
//     { SDL_PIXELFORMAT_BGR24, GSP_BGR8_OES },
//     { SDL_PIXELFORMAT_RGB565, GSP_RGB565_OES },
//     { SDL_PIXELFORMAT_RGBA5551, GSP_RGB5_A1_OES },
//     { SDL_PIXELFORMAT_RGBA4444, GSP_RGBA4_OES }
// };

// /* N3DS driver bootstrap functions */

static void WEBROGUE_DeleteDevice(SDL_VideoDevice *device)
{
    SDL_free(device->displays);
    SDL_free(device->driverdata);
    SDL_free(device);
}

static SDL_VideoDevice *WEBROGUE_CreateDevice(void)
{
    SDL_VideoDevice *device;
    SDL_VideoData *phdata;

    /* Initialize all variables that we clean on shutdown */
    device = (SDL_VideoDevice *)SDL_calloc(1, sizeof(SDL_VideoDevice));
    if (!device) {
        SDL_OutOfMemory();
        return 0;
    }

    /* Initialize internal data */
    phdata = (SDL_VideoData *)SDL_calloc(1, sizeof(SDL_VideoData));
    if (!phdata) {
        SDL_OutOfMemory();
        SDL_free(device);
        return NULL;
    }

    device->driverdata = phdata;

    device->VideoInit = WEBROGUE_VideoInit;
    device->VideoQuit = WEBROGUE_VideoQuit;

    device->GetDisplayModes = WEBROGUE_GetDisplayModes;
    device->SetDisplayMode = WEBROGUE_SetDisplayMode;
    device->GetDisplayBounds = WEBROGUE_GetDisplayBounds;

    device->CreateSDLWindow = WEBROGUE_CreateWindow;
    device->DestroyWindow = WEBROGUE_DestroyWindow;

    device->HasScreenKeyboardSupport = WEBROGUE_HasScreenKeyboardSupport;
    device->StartTextInput = WEBROGUE_StartTextInput;
    device->StopTextInput = WEBROGUE_StopTextInput;

    device->PumpEvents = WEBROGUE_PumpEvents;

    device->CreateWindowFramebuffer = SDL_WEBROGUE_CreateWindowFramebuffer;
    device->UpdateWindowFramebuffer = SDL_WEBROGUE_UpdateWindowFramebuffer;
    device->DestroyWindowFramebuffer = SDL_WEBROGUE_DestroyWindowFramebuffer;


    device->GL_LoadLibrary = WEBROGUE_GLES_LoadLibrary;
    device->GL_GetProcAddress = WEBROGUE_GLES_GetProcAddress;
    device->GL_UnloadLibrary = WEBROGUE_GLES_UnloadLibrary;
    device->GL_CreateContext = WEBROGUE_GLES_CreateContext;
    device->GL_MakeCurrent = WEBROGUE_GLES_MakeCurrent;
    device->GL_SetSwapInterval = WEBROGUE_GLES_SetSwapInterval;
    device->GL_GetSwapInterval = WEBROGUE_GLES_GetSwapInterval;
    device->GL_SwapWindow = WEBROGUE_GLES_SwapWindow;
    device->GL_DeleteContext = WEBROGUE_GLES_DeleteContext;

    device->free = WEBROGUE_DeleteDevice;

    device->quirk_flags = VIDEO_DEVICE_QUIRK_FULLSCREEN_ONLY;

    return device;
}

VideoBootStrap WEBROGUE_bootstrap = { WEBROGUEVID_DRIVER_NAME, "Webrogue Video Driver", WEBROGUE_CreateDevice, NULL /* no ShowMessageBox implementation */ };

static int WEBROGUE_VideoInit(_THIS)
{
    SDL_VideoData *driverdata = (SDL_VideoData *)_this->driverdata;


    // gfxInit(GSP_RGBA8_OES, GSP_RGBA8_OES, false);
    // hidInit();

    AddWEBROGUEDisplay();
    // driverdata->top_display = AddN3DSDisplay(GFX_TOP);
    // driverdata->touch_display = AddN3DSDisplay(GFX_BOTTOM);

    // N3DS_InitTouch();
    // N3DS_SwkbInit();

    // FIXME

    return 0;
}

SDL_FORCE_INLINE int
AddWEBROGUEDisplay()
{
    SDL_DisplayMode mode;
    ModeDriverData *modedata;
    SDL_VideoDisplay display;
    DisplayDriverData *display_driver_data = SDL_calloc(1, sizeof(DisplayDriverData));
    if (!display_driver_data) {
        return SDL_OutOfMemory();
    }

    SDL_zero(mode);
    SDL_zero(display);

    // display_driver_data->screen = screen;

    modedata = SDL_malloc(sizeof(ModeDriverData));
    if (!modedata) {
        return SDL_OutOfMemory();
    }

    // FIXME
    mode.w = 2048;
    mode.h = 2048;
    mode.refresh_rate = 60;
    mode.format = SDL_PIXELFORMAT_RGBA8888;
    mode.driverdata = modedata;

    display.name = "webrogue screen";
    display.desktop_mode = mode;
    display.current_mode = mode;
    display.driverdata = display_driver_data;

    return SDL_AddVideoDisplay(&display, SDL_FALSE);
}

static void WEBROGUE_VideoQuit(_THIS)
{
    // N3DS_SwkbQuit();
    // N3DS_QuitTouch();

    // hidExit();
    // gfxExit();
}

static void WEBROGUE_GetDisplayModes(_THIS, SDL_VideoDisplay *display)
{
    DisplayDriverData *displaydata = display->driverdata;
    ModeDriverData *modedata;
    SDL_DisplayMode mode;
    // int i;

    modedata = SDL_malloc(sizeof(ModeDriverData));
    if (!modedata)
        return;

    SDL_zero(mode);
    mode.w = 2048;
    mode.h = 2048;
    mode.refresh_rate = 60;
    mode.format = SDL_PIXELFORMAT_RGBA8888;
    mode.driverdata = modedata;
    // modedata->fmt = format_map[i].gspfmt;

    if (!SDL_AddDisplayMode(display, &mode)) {
        SDL_free(modedata);
    }
}

static int WEBROGUE_SetDisplayMode(_THIS, SDL_VideoDisplay *display, SDL_DisplayMode *mode)
{
    // FIXME implement
    DisplayDriverData *displaydata = display->driverdata;
    ModeDriverData *modedata = mode->driverdata;

    // gfxSetScreenFormat(displaydata->screen, modedata->fmt);
    return 0;
}

static int WEBROGUE_GetDisplayBounds(_THIS, SDL_VideoDisplay *display, SDL_Rect *rect)
{
    DisplayDriverData *driver_data = (DisplayDriverData *)display->driverdata;
    if (!driver_data) {
        WR_NOT_IMPLEMENTED;
        return -1;
    }
    int width, height;
    webrogue_gfx_window_size(&width, &height);
    // FIXME use real data
    rect->x = 0;
    rect->y = 0;
    rect->w = width;
    rect->h = height;

    return 0;
}

static int WEBROGUE_CreateWindow(_THIS, SDL_Window *window)
{
    DisplayDriverData *display_data;
    SDL_WindowData *window_data = (SDL_WindowData *)SDL_calloc(1, sizeof(SDL_WindowData));
    if (!window_data) {
        return SDL_OutOfMemory();
    }
    webrogue_gfx_make_window();
    display_data = (DisplayDriverData *)SDL_GetDisplayDriverData(window->display_index);
    window->driverdata = window_data;

    SDL_SetKeyboardFocus(window);
    return 0;
}

static void WEBROGUE_DestroyWindow(_THIS, SDL_Window *window)
{
    WR_NOT_IMPLEMENTED;
    return;
    // if (!window) {
    //     return;
    // }
    // SDL_free(window->driverdata);
}

#endif /* SDL_VIDEO_DRIVER_WEBROGUE */

// /* vi: set sts=4 ts=4 sw=4 expandtab: */
