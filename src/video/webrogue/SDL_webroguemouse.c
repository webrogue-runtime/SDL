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

#include "../SDL_sysvideo.h"

#include "SDL_mouse.h"
#include "../../events/SDL_mouse_c.h"
#include "SDL_webroguevideo.h"
#include "../SDL_pixels_c.h"

#include "SDL_webroguemouse.h"

#include "SDL_hints.h"
#include "../../SDL_hints_c.h"

static int WEBROGUE_SetRelativeMouseMode(SDL_bool enabled) 
{
    SDL_VideoDevice *vd = SDL_GetVideoDevice();
    SDL_VideoData *d = vd->driverdata;
    SDL_Window *window;
    bool result = true;

    for (window = vd->windows; window; window = window->next) {
        SDL_WindowData *window_data = (SDL_WindowData *)window->driverdata;
        if(!wr4c_set_mouse_grab(window_data->wr_window, enabled)) {
            result = false;
        }
    }

    if(result) {
        d->is_mouse_grabbed = enabled;
    }
    return result;
}

void WEBROGUE_InitMouse(void)
{
    SDL_Mouse *mouse = SDL_GetMouse();
    SDL_VideoDevice *vd = SDL_GetVideoDevice();
    SDL_VideoData *d = vd->driverdata;

    mouse->SetRelativeMouseMode = WEBROGUE_SetRelativeMouseMode;
}

#endif /* SDL_VIDEO_DRIVER_WAYLAND */

/* vi: set ts=4 sw=4 expandtab: */
