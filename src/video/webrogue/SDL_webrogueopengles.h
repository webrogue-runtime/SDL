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

#if defined(SDL_VIDEO_DRIVER_WEBROGUE) && defined(SDL_VIDEO_OPENGL_EGL)

#include "../SDL_sysvideo.h"
#include "../SDL_egl_c.h"
#include "SDL_webroguevideo.h"

typedef struct SDL_PrivateGLESData
{
    /* 1401 If the struct-declaration-list contains no named members, the behavior is undefined. */
    /* warning: empty struct has size 0 in C, size 1 in C++ [-Wc++-compat] */
    int dummy;
} SDL_PrivateGLESData;

/* OpenGLES functions */

extern int Webrogue_GLES_IsLibraryLoadable(void);
extern int Webrogue_GLES_LoadLibrary(_THIS, const char *path);
#define Webrogue_GLES_GetProcAddress SDL_EGL_GetProcAddress
#define Webrogue_GLES_SetSwapInterval SDL_EGL_SetSwapInterval
#define Webrogue_GLES_GetSwapInterval SDL_EGL_GetSwapInterval
extern SDL_GLContext Webrogue_GLES_CreateContext(_THIS, SDL_Window * window);
extern int Webrogue_GLES_SwapWindow(_THIS, SDL_Window * window);
extern int Webrogue_GLES_MakeCurrent(_THIS, SDL_Window * window, SDL_GLContext context);
#define Webrogue_GLES_DeleteContext SDL_EGL_DeleteContext

#define Webrogue_GLES_GetAttribute SDL_EGL_GetAttribute
#define Webrogue_GLES_UnloadLibrary SDL_EGL_UnloadLibrary

#endif

#endif /* SDL_webrogueopengles_h_ */

/* vi: set ts=4 sw=4 expandtab: */
