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

#include "../../events/SDL_events_c.h"
#include "SDL_webrogue_events_c.h"
#include "SDL_webrogue_touch.h"
#include "SDL_webrogue_unimplemented.h"
#include "SDL_webrogue_video.h"
#include <webroguegfx/webroguegfx.h>

void WEBROGUE_PumpEvents(SDL_VideoDevice *_this)
{
    SDL_VideoData *data = (SDL_VideoData *)_this->internal;
    webrogue_event event;
    while (1) {
        event = webroguegfx_poll();
        switch (event.type) {
        case WEBROGUE_EVENT_TYPE_MOUSE_BUTTON:
        {
            SDL_SendMouseButton(0, data->latest_window, SDL_DEFAULT_MOUSE_ID, event.inner.mouse_button.button, event.inner.mouse_button.down);
        } break;
        case WEBROGUE_EVENT_TYPE_MOUSE_MOTION:
        {
            SDL_SendMouseMotion(0, data->latest_window, SDL_DEFAULT_MOUSE_ID, false, event.inner.mouse_motion.x, event.inner.mouse_motion.y);
        } break;
        case WEBROGUE_EVENT_TYPE_KEY:
        {
            SDL_SendKeyboardKey(0, SDL_GLOBAL_KEYBOARD_ID, 0, event.inner.key.scancode, event.inner.key.down);
        } break;
        case WEBROGUE_EVENT_TYPE_QUIT:
        {
            SDL_Event ev;
            ev.type = SDL_EVENT_QUIT;
            SDL_PushEvent(&ev);
        } break;
        case WEBROGUE_EVENT_TYPE_WINDOW_RESIZED:
        {
            int width, height;
            webroguegfx_window_size(&width, &height);
            SDL_SendWindowEvent(data->latest_window, SDL_EVENT_WINDOW_RESIZED, width, height);
        } break;
        case WEBROGUE_EVENT_TYPE_GL_RESIZED:
        {
            int width, height;
            webroguegfx_gl_size(&width, &height);
            SDL_SendWindowEvent(data->latest_window, SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED, width, height);
        } break;
        case WEBROGUE_EVENT_TYPE_INVALID:
        {
            return;
        }
        }
    }
    WR_NOT_IMPLEMENTED;
}

#endif // SDL_VIDEO_DRIVER_N3DS
