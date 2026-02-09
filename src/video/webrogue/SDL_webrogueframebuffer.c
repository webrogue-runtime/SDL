/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2024 Sam Lantinga <slouken@libsdl.org>

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
#include "../../SDL_internal.h"

#ifdef SDL_VIDEO_DRIVER_WEBROGUE

#include "SDL_webrogue_unimplemented.h"

#include "../SDL_sysvideo.h"
#include "SDL_webrogueframebuffer.h"
#include "SDL_webroguevideo.h"
#include <webroguegfx/webroguegfx.h>

int SDL_Webrogue_CreateWindowFramebuffer(_THIS, SDL_Window *window, Uint32 *format, void **pixels, int *pitch)
{
    SDL_Surface *surface;
    const Uint32 surface_format = SDL_PIXELFORMAT_ARGB8888;
    int w, h;

    SDL_WindowData *data = (SDL_WindowData *)window->driverdata;
    surface = data->surface;
    SDL_FreeSurface(surface);

    SDL_GetWindowSizeInPixels(window, &w, &h);
    surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 0, surface_format);

    if (!surface) {
        return SDL_OutOfMemory();
    }
    data->surface = surface;
    *format = surface_format;
    *pixels = surface->pixels;
    *pitch = surface->pitch;
    return 0;
}

int SDL_Webrogue_UpdateWindowFramebuffer(_THIS, SDL_Window *window, const SDL_Rect *rects, int numrects)
{
    SDL_WindowData *data = (SDL_WindowData *)window->driverdata;
    SDL_Surface *surface = data->surface;
    int w = surface->w;
    int h = surface->h;

    if (!surface) {
        return SDL_SetError("%s: Unable to get the window surface.", __func__);
    }
    webroguegfx_present_pixels(data->wr_window, surface->pixels, 4 * w * h);

    return 0;
}

void SDL_Webrogue_DestroyWindowFramebuffer(_THIS, SDL_Window *window)
{
    SDL_WindowData *data = (SDL_WindowData *)window->driverdata;

    SDL_FreeSurface(data->surface);
    data->surface = NULL;
}

#endif /* SDL_VIDEO_DRIVER_WEBROGUE */

// /* vi: set sts=4 ts=4 sw=4 expandtab: */
