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

#if defined(SDL_VIDEO_DRIVER_WEBROGUE) && defined(SDL_VIDEO_OPENGL_EGL)

#include "SDL_webrogue_unimplemented.h"
#include <libwr4c/wr4c.h>

#include "SDL_hints.h"
// #include "SDL_x11video.h"
#include "SDL_webrogueopengles.h"
// #include "SDL_x11opengl.h"

// /* EGL implementation of SDL OpenGL support */

static void * static_eglGetProcAddress(const char *procname) __attribute__((weakref, alias("eglGetProcAddress")));

extern int Webrogue_GLES_IsLibraryLoadable(void) {
    return static_eglGetProcAddress != NULL && wr4c_vulkan_check();
}

#define LOAD_FUNC(NAME)                                        \
    _this->egl_data->NAME = _this->egl_data->eglGetProcAddress (#NAME);   \
    if (!_this->egl_data->NAME) {                              \
        return SDL_SetError("Could not get a EGL entrypoint"); \
    }

int Webrogue_GLES_LoadLibrary(_THIS, const char *path)
{
    if(!wr4c_vulkan_check()) {
        return SDL_SetError("WebrogueGFX-Vulkan API is unavailable");
    }
    if(!static_eglGetProcAddress) {
        return SDL_SetError("Could not get eglGetProcAddress entrypoint. You probably forgot -Wl,--export=eglGetProcAddress or -lEGL link flag.");
    }

    _this->egl_data = (struct SDL_EGL_VideoData *)SDL_calloc(1, sizeof(SDL_EGL_VideoData));
    if (!_this->egl_data) {
        return SDL_OutOfMemory();
    }

    _this->egl_data->eglGetProcAddress = (void *(EGLAPIENTRY *)(const char *))static_eglGetProcAddress;

    LOAD_FUNC(eglGetDisplay);
    LOAD_FUNC(eglInitialize);
    LOAD_FUNC(eglTerminate);
    LOAD_FUNC(eglChooseConfig);
    LOAD_FUNC(eglGetConfigAttrib);
    LOAD_FUNC(eglCreateContext);
    LOAD_FUNC(eglDestroyContext);
    LOAD_FUNC(eglCreateWindowSurface);
    LOAD_FUNC(eglDestroySurface);
    LOAD_FUNC(eglMakeCurrent);
    LOAD_FUNC(eglSwapBuffers);
    LOAD_FUNC(eglSwapInterval);
    LOAD_FUNC(eglWaitNative);
    LOAD_FUNC(eglWaitGL);
    LOAD_FUNC(eglBindAPI);
    LOAD_FUNC(eglQueryString);
    LOAD_FUNC(eglGetError);

    _this->egl_data->egl_display = _this->egl_data->eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (!_this->egl_data->egl_display) {
        return SDL_SetError("Could not get EGL display");
    }

    if (_this->egl_data->eglInitialize(_this->egl_data->egl_display, NULL, NULL) != EGL_TRUE) {
        return SDL_SetError("Could not initialize EGL");
    }

    *_this->gl_config.driver_path = '\0';

    return 0;
    //     SDL_VideoData *data = (SDL_VideoData *)_this->driverdata;
}

typedef struct Webrogue_GLContext
{
} Webrogue_GLContext;

SDL_GLContext Webrogue_GLES_CreateContext(_THIS, SDL_Window *window)
{
    SDL_WindowData *window_data = (SDL_WindowData *)window->driverdata;
    SDL_GLContext context = SDL_EGL_CreateContext(_this, window_data->egl_surface);
    if (context) {
        window_data->egl_context = (EGLContext)context;
    }
    return context;
}

int Webrogue_GLES_SwapWindow(_THIS, SDL_Window *window)
{
    SDL_WindowData *window_data = (SDL_WindowData *)window->driverdata;
    if (window_data->egl_surface == EGL_NO_SURFACE) {
        return 0;
    }
    return SDL_EGL_SwapBuffers(_this, window_data->egl_surface);
}

int Webrogue_GLES_MakeCurrent(_THIS, SDL_Window *window, SDL_GLContext context)
{
    EGLSurface egl_surface = EGL_NO_SURFACE;
    if (window) {
        SDL_WindowData *window_data = (SDL_WindowData *)window->driverdata;
        egl_surface = window_data->egl_surface;
    }

    if (SDL_EGL_MakeCurrent(_this, egl_surface, context) < 0) {
        return -1;
    }
    if (window && context) {
        for (SDL_Window *other = _this->windows; other; other = other->next) {
            SDL_WindowData *other_data = (SDL_WindowData *)other->driverdata;
            if (other != window && other_data &&
                other_data->egl_context == (EGLContext)context) {
                other_data->egl_context = EGL_NO_CONTEXT;
            }
        }
        SDL_WindowData *window_data = (SDL_WindowData *)window->driverdata;
        window_data->egl_context = (EGLContext)context;
    }
    return 0;
}

void Webrogue_GLES_DeleteContext(_THIS, SDL_GLContext context)
{
    for (SDL_Window *window = _this->windows; window; window = window->next) {
        SDL_WindowData *window_data = (SDL_WindowData *)window->driverdata;
        if (window_data && window_data->egl_context == (EGLContext)context) {
            window_data->egl_context = EGL_NO_CONTEXT;
        }
    }
    SDL_EGL_DeleteContext(_this, context);
}
#endif /* SDL_VIDEO_DRIVER_WEBROGUE */

    /* vi: set ts=4 sw=4 expandtab: */
