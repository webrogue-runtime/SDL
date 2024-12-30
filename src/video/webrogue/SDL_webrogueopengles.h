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

#ifndef SDL_webrogueopengles_h_
#define SDL_webrogueopengles_h_

#ifdef SDL_VIDEO_DRIVER_WEBROGUE

#include "../SDL_sysvideo.h"
#include "SDL_webroguevideo.h"

typedef struct SDL_PrivateGLESData
{
    /* 1401 If the struct-declaration-list contains no named members, the behavior is undefined. */
    /* warning: empty struct has size 0 in C, size 1 in C++ [-Wc++-compat] */
    int dummy;
} SDL_PrivateGLESData;

/* OpenGLES functions */

extern int WEBROGUE_GLES_LoadLibrary(_THIS, const char *path);
extern void *WEBROGUE_GLES_GetProcAddress(_THIS, const char *proc);
extern void WEBROGUE_GLES_UnloadLibrary(_THIS);
extern int WEBROGUE_GLES_SetSwapInterval(_THIS, int interval);
extern int WEBROGUE_GLES_GetSwapInterval(_THIS);
extern SDL_GLContext WEBROGUE_GLES_CreateContext(_THIS, SDL_Window * window);
extern int WEBROGUE_GLES_SwapWindow(_THIS, SDL_Window * window);
extern int WEBROGUE_GLES_MakeCurrent(_THIS, SDL_Window * window, SDL_GLContext context);
extern void WEBROGUE_GLES_DeleteContext(_THIS, SDL_GLContext context);

#endif /* SDL_VIDEO_DRIVER_WEBROGUE */

#endif /* SDL_webrogueopengles_h_ */

/* vi: set ts=4 sw=4 expandtab: */
