/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2025 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "SDL_internal.h"

#ifdef SDL_VIDEO_DRIVER_WEBROGUE

#include "../SDL_sysvideo.h"
#include "SDL_webrogue_events_c.h"
#include "SDL_webrogue_framebuffer_c.h"
#include "SDL_webrogue_opengles.h"
#include "SDL_webrogue_swkb.h"
#include "SDL_webrogue_touch.h"
#include "SDL_webrogue_unimplemented.h"
#include "SDL_webrogue_video.h"
#include <webroguegfx/webroguegfx.h>

#define WEBROGUEVID_DRIVER_NAME "webrogue"

static bool AddWEBROGUEDisplay();

static bool WEBROGUE_VideoInit(SDL_VideoDevice *_this);
static void WEBROGUE_VideoQuit(SDL_VideoDevice *_this);
static bool WEBROGUE_GetDisplayModes(SDL_VideoDevice *_this, SDL_VideoDisplay *display);
static bool WEBROGUE_SetDisplayMode(SDL_VideoDevice *_this, SDL_VideoDisplay *display, SDL_DisplayMode *mode);
static bool WEBROGUE_GetDisplayBounds(SDL_VideoDevice *_this, SDL_VideoDisplay *display, SDL_Rect *rect);
static bool WEBROGUE_CreateWindow(SDL_VideoDevice *_this, SDL_Window *window, SDL_PropertiesID create_props);
static void WEBROGUE_DestroyWindow(SDL_VideoDevice *_this, SDL_Window *window);

struct SDL_DisplayData
{
};

struct SDL_DisplayModeData
{
};

// WEBROGUE driver bootstrap functions

static void WEBROGUE_DeleteDevice(SDL_VideoDevice *device)
{
    SDL_free(device->internal);
    SDL_free(device);
}

static SDL_VideoDevice *WEBROGUE_CreateDevice(void)
{
    SDL_VideoDevice *device;
    SDL_VideoData *phdata;

    // Initialize all variables that we clean on shutdown
    device = (SDL_VideoDevice *)SDL_calloc(1, sizeof(SDL_VideoDevice));
    if (!device) {
        return NULL;
    }

    // Initialize internal data
    phdata = (SDL_VideoData *)SDL_calloc(1, sizeof(SDL_VideoData));
    if (!phdata) {
        SDL_free(device);
        return NULL;
    }

    device->internal = phdata;

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
    device->GL_DestroyContext = WEBROGUE_GLES_DestroyContext;

    device->free = WEBROGUE_DeleteDevice;

    device->device_caps = VIDEO_DEVICE_CAPS_SENDS_FULLSCREEN_DIMENSIONS;

    return device;
}

VideoBootStrap WEBROGUE_bootstrap = { WEBROGUEVID_DRIVER_NAME, "WEBROGUE Video Driver", WEBROGUE_CreateDevice, NULL, /* no ShowMessageBox implementation */ false };

static bool WEBROGUE_VideoInit(SDL_VideoDevice *_this)
{
    SDL_VideoData *internal = (SDL_VideoData *)_this->internal;

    AddWEBROGUEDisplay();

    return true;
}

static bool AddWEBROGUEDisplay()
{
    SDL_DisplayMode mode;
    SDL_DisplayModeData *modedata;
    SDL_VideoDisplay display;
    SDL_DisplayData *display_driver_data = SDL_calloc(1, sizeof(SDL_DisplayData));
    if (!display_driver_data) {
        return false;
    }

    SDL_zero(mode);
    SDL_zero(display);

    modedata = SDL_malloc(sizeof(SDL_DisplayModeData));
    if (!modedata) {
        return false;
    }

    int width, height;
    webroguegfx_window_size(&width, &height);
    mode.w = width;
    mode.h = height;
    mode.refresh_rate = 60.0f;
    mode.format = SDL_PIXELFORMAT_RGBA32;
    mode.internal = modedata;

    display.name = "Webrogue screen";
    display.desktop_mode = mode;
    display.internal = display_driver_data;

    if (SDL_AddVideoDisplay(&display, false) == 0) {
        return false;
    }
    return true;
}

static void WEBROGUE_VideoQuit(SDL_VideoDevice *_this)
{
    WEBROGUE_SwkbQuit();
    WEBROGUE_QuitTouch();
}

static bool WEBROGUE_GetDisplayModes(SDL_VideoDevice *_this, SDL_VideoDisplay *display)
{
    SDL_DisplayData *displaydata = display->internal;
    SDL_DisplayModeData *modedata;
    SDL_DisplayMode mode;

    modedata = SDL_malloc(sizeof(SDL_DisplayModeData));

    SDL_zero(mode);

    int width, height;
    webroguegfx_window_size(&width, &height);
    mode.w = width;
    mode.h = height;
    mode.refresh_rate = 60.0f;
    mode.format = SDL_PIXELFORMAT_RGBA32;
    mode.internal = modedata;

    if (!SDL_AddFullscreenDisplayMode(display, &mode)) {
        SDL_free(modedata);
    }

    return true;
}

static bool WEBROGUE_SetDisplayMode(SDL_VideoDevice *_this, SDL_VideoDisplay *display, SDL_DisplayMode *mode)
{
    // FIXME implement
    SDL_DisplayData *displaydata = display->internal;
    SDL_DisplayModeData *modedata = mode->internal;

    return true;
}

static bool WEBROGUE_GetDisplayBounds(SDL_VideoDevice *_this, SDL_VideoDisplay *display, SDL_Rect *rect)
{
    SDL_DisplayData *driver_data = display->internal;

    if (!driver_data) {
        return false;
    }

    int width, height;
    webroguegfx_window_size(&width, &height);
    rect->x = 0;
    rect->y = 0;
    rect->w = 500;
    rect->h = 500;
    return true;
}

static bool WEBROGUE_CreateWindow(SDL_VideoDevice *_this, SDL_Window *window, SDL_PropertiesID create_props)
{
    SDL_DisplayData *display_data;
    SDL_VideoData *internal = (SDL_VideoData *)_this->internal;
    SDL_WindowData *window_data = (SDL_WindowData *)SDL_calloc(1, sizeof(SDL_WindowData));
    if (!window_data) {
        return false;
    }
    webroguegfx_make_window();
    int width, height;
    webroguegfx_window_size(&width, &height);
    window->w = width;
    window->h = height;
    display_data = SDL_GetDisplayDriverDataForWindow(window);
    window->internal = window_data;

    internal->latest_window = window;

    SDL_SetKeyboardFocus(window);
    return true;
}

static void WEBROGUE_DestroyWindow(SDL_VideoDevice *_this, SDL_Window *window)
{
    if (!window) {
        return;
    }
    SDL_free(window->internal);
}

#endif // SDL_VIDEO_DRIVER_WEBROGUE
