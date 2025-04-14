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

#include "../../events/SDL_events_c.h"
#include "SDL_webrogueevents_c.h"
#include "SDL_webroguevideo.h"
#include <webroguegfx/webroguegfx.h>

void WEBROGUE_PumpEvents(_THIS)
{
    SDL_VideoData *data = (SDL_VideoData *)_this->driverdata;
    webrogue_event event;
    while (1) {
        event = webroguegfx_poll();
        switch (event.type) {
        case WEBROGUE_EVENT_TYPE_MOUSE_DOWN:
        {
            SDL_SendMouseButton(data->latest_window, 0, SDL_PRESSED, event.inner.mouse_down.button);
        } break;
        case WEBROGUE_EVENT_TYPE_MOUSE_UP:
        {
            SDL_SendMouseButton(data->latest_window, 0, SDL_RELEASED, event.inner.mouse_down.button);
        } break;
        case WEBROGUE_EVENT_TYPE_MOUSE_MOTION:
        {
            SDL_SendMouseMotion(data->latest_window, 0, 0, event.inner.mouse_motion.x, event.inner.mouse_motion.y);
        } break;
        case WEBROGUE_EVENT_TYPE_QUIT:
        {
            SDL_Event ev;
            ev.type = SDL_QUIT;
            SDL_PushEvent(&ev);
        } break;
        case WEBROGUE_EVENT_TYPE_INVALID:
        {
            return;
        }
        }
    }
    WR_NOT_IMPLEMENTED;
    // hidScanInput();
    // N3DS_PollTouch(_this);

    // if (!aptMainLoop()) {
    //     SDL_Event ev;
    //     ev.type = SDL_QUIT;
    //     SDL_PushEvent(&ev);
    //     return;
    // }
}

#endif /* SDL_VIDEO_DRIVER_WEBROGUE */

/* vi: set sts=4 ts=4 sw=4 expandtab: */
