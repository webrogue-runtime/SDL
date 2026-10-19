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

#if defined(SDL_VIDEO_DRIVER_WEBROGUE) && defined(SDL_VIDEO_OPENGL_EGL)

#include <GLES2/gl2.h>

#include "SDL_webrogue_opengles.h"
#include "SDL_webrogue_unimplemented.h"
#include "SDL_webrogue_video.h"
#include <libwr4c/wr4c.h>

static void * static_eglGetProcAddress(const char *procname) __attribute__((weakref, alias("eglGetProcAddress")));

#define LOAD_FUNC(TYPE, NAME) \
    _this->egl_data->NAME = (TYPE)_this->egl_data->eglGetProcAddress(#NAME);

bool Webrogue_GLES_LoadLibrary(SDL_VideoDevice *_this, const char *path)
{
    if(!wr4c_vulkan_check()) {
        return SDL_SetError("WebrogueGFX-Vulkan API is unavailable");
    }
    if(!static_eglGetProcAddress) {
        return SDL_SetError("Could not get eglGetProcAddress entrypoint. You probably forgot -Wl,--export=eglGetProcAddress or -lEGL link flag.");
    }

    _this->egl_data = (struct SDL_EGL_VideoData *)SDL_calloc(1, sizeof(SDL_EGL_VideoData));
    if (!_this->egl_data) {
        return false;
    }
    _this->egl_data->eglGetProcAddress = (PFNEGLGETPROCADDRESSPROC)static_eglGetProcAddress;
    
    // Load new function pointers
    LOAD_FUNC(PFNEGLGETDISPLAYPROC, eglGetDisplay);
    LOAD_FUNC(PFNEGLINITIALIZEPROC, eglInitialize);
    LOAD_FUNC(PFNEGLTERMINATEPROC, eglTerminate);
    LOAD_FUNC(PFNEGLCHOOSECONFIGPROC, eglChooseConfig);
    LOAD_FUNC(PFNEGLCREATECONTEXTPROC, eglCreateContext);
    LOAD_FUNC(PFNEGLDESTROYCONTEXTPROC, eglDestroyContext);
    LOAD_FUNC(PFNEGLCREATEPBUFFERSURFACEPROC, eglCreatePbufferSurface);
    LOAD_FUNC(PFNEGLCREATEWINDOWSURFACEPROC, eglCreateWindowSurface);
    LOAD_FUNC(PFNEGLDESTROYSURFACEPROC, eglDestroySurface);
    LOAD_FUNC(PFNEGLMAKECURRENTPROC, eglMakeCurrent);
    LOAD_FUNC(PFNEGLSWAPBUFFERSPROC, eglSwapBuffers);
    LOAD_FUNC(PFNEGLSWAPINTERVALPROC, eglSwapInterval);
    LOAD_FUNC(PFNEGLQUERYSTRINGPROC, eglQueryString);
    LOAD_FUNC(PFNEGLGETCONFIGATTRIBPROC, eglGetConfigAttrib);
    LOAD_FUNC(PFNEGLWAITNATIVEPROC, eglWaitNative);
    LOAD_FUNC(PFNEGLWAITGLPROC, eglWaitGL);
    LOAD_FUNC(PFNEGLBINDAPIPROC, eglBindAPI);
    LOAD_FUNC(PFNEGLGETERRORPROC, eglGetError);
    LOAD_FUNC(PFNEGLQUERYDEVICESEXTPROC, eglQueryDevicesEXT);
    LOAD_FUNC(PFNEGLGETPLATFORMDISPLAYEXTPROC, eglGetPlatformDisplayEXT);
    // Atomic functions
    LOAD_FUNC(PFNEGLCREATESYNCKHRPROC, eglCreateSyncKHR);
    LOAD_FUNC(PFNEGLDESTROYSYNCKHRPROC, eglDestroySyncKHR);
    LOAD_FUNC(PFNEGLDUPNATIVEFENCEFDANDROIDPROC, eglDupNativeFenceFDANDROID);
    LOAD_FUNC(PFNEGLWAITSYNCKHRPROC, eglWaitSyncKHR);
    LOAD_FUNC(PFNEGLCLIENTWAITSYNCKHRPROC, eglClientWaitSyncKHR);
    // Atomic functions end

    _this->egl_data->egl_display = _this->egl_data->eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if (_this->egl_data->eglInitialize(_this->egl_data->egl_display, NULL, NULL) != EGL_TRUE) {
        _this->gl_config.driver_loaded = 0;
        *_this->gl_config.driver_path = '\0';
        return SDL_SetError("Could not initialize EGL");
    }

    *_this->gl_config.driver_path = '\0';
    _this->egl_data->is_offscreen = false;

    return true;
}

typedef struct Webrogue_GLContext
{
} Webrogue_GLContext;

SDL_GLContext Webrogue_GLES_CreateContext(SDL_VideoDevice *_this, SDL_Window *window)
{
    SDL_WindowData *window_data = (SDL_WindowData *)window->internal;
    SDL_GLContext context = SDL_EGL_CreateContext(_this, window_data->egl_surface);
    if (context) {
        window_data->egl_context = (EGLContext)context;
    }
    return context;
}

bool Webrogue_GLES_SwapWindow(SDL_VideoDevice *_this, SDL_Window *window)
{
    SDL_WindowData *window_data = (SDL_WindowData *)window->internal;
    if (window_data->egl_surface == EGL_NO_SURFACE) {
        return true;
    }
    return SDL_EGL_SwapBuffers(_this, window_data->egl_surface);
}

bool Webrogue_GLES_MakeCurrent(SDL_VideoDevice *_this, SDL_Window *window, SDL_GLContext context)
{
    EGLSurface egl_surface = EGL_NO_SURFACE;
    if (window) {
        SDL_WindowData *window_data = (SDL_WindowData *)window->internal;
        egl_surface = window_data->egl_surface;
    }

    if (!SDL_EGL_MakeCurrent(_this, egl_surface, context)) {
        return false;
    }
    if (window && context) {
        for (SDL_Window *other = _this->windows; other; other = other->next) {
            SDL_WindowData *other_data = (SDL_WindowData *)other->internal;
            if (other != window && other_data &&
                other_data->egl_context == (EGLContext)context) {
                other_data->egl_context = EGL_NO_CONTEXT;
            }
        }
        SDL_WindowData *window_data = (SDL_WindowData *)window->internal;
        window_data->egl_context = (EGLContext)context;
    }
    return true;
}

bool Webrogue_GLES_DestroyContext(SDL_VideoDevice *_this, SDL_GLContext context)
{
    for (SDL_Window *window = _this->windows; window; window = window->next) {
        SDL_WindowData *window_data = (SDL_WindowData *)window->internal;
        if (window_data && window_data->egl_context == (EGLContext)context) {
            window_data->egl_context = EGL_NO_CONTEXT;
        }
    }
    return SDL_EGL_DestroyContext(_this, context);
}
#endif
