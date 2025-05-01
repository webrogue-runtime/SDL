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

#include <GLES2/gl2.h>

#include "SDL_webrogue_opengles.h"
#include "SDL_webrogue_unimplemented.h"
#include "SDL_webrogue_video.h"
#include <webroguegfx/webroguegfx.h>

bool WEBROGUE_GLES_LoadLibrary(SDL_VideoDevice *_this, const char *path)
{
    webroguegfx_init_gl();
    return true;
}

void WEBROGUE_GLES_UnloadLibrary(SDL_VideoDevice *_this)
{
}

SDL_FunctionPointer WEBROGUE_GLES_GetProcAddress(SDL_VideoDevice *_this, const char *proc)
{
    return webroguegfx_gl_loader(proc);
}

bool WEBROGUE_GLES_SetSwapInterval(SDL_VideoDevice *_this, int interval)
{
    return true;
}

bool WEBROGUE_GLES_GetSwapInterval(SDL_VideoDevice *_this, int *interval)
{
    webroguegfx_get_gl_swap_interval(interval);
    return true;
}

typedef struct WEBROGUE_GLContext
{
} WEBROGUE_GLContext;

SDL_GLContext WEBROGUE_GLES_CreateContext(SDL_VideoDevice *_this, SDL_Window *window)
{
    SDL_WindowData *window_data;

    window_data = window->internal;

    if (window_data->gl_context) {
        SDL_SetError("Cannot create multiple contexts per window");
        return NULL;
    }

    WEBROGUE_GLContext *context = SDL_malloc(sizeof(WEBROGUE_GLContext));

    if (!context) {
        SDL_SetError("Could not create context");
        return NULL;
    }

    window_data->gl_context = (SDL_GLContext)context;

    return (SDL_GLContext)context;
}

bool WEBROGUE_GLES_DestroyContext(SDL_VideoDevice *_this, SDL_GLContext context)
{
    return true;
}

bool WEBROGUE_GLES_SwapWindow(SDL_VideoDevice *_this, SDL_Window *window)
{
    webroguegfx_present();
    return true;
}

bool WEBROGUE_GLES_MakeCurrent(SDL_VideoDevice *_this, SDL_Window *window, SDL_GLContext context)
{
    // it isn't possible to reuse contexts across canvases
    if (window && context) {
        SDL_WindowData *window_data = window->internal;

        if (context != window_data->gl_context) {
            return SDL_SetError("Cannot make context current to another window");
        }
    }

    webroguegfx_init_gl();
    return true;
}

#endif // SDL_VIDEO_DRIVER_WEBROGUE
