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

#if defined(SDL_VIDEO_DRIVER_WEBROGUE)

#include <webrogue_gfx/webrogue_gfx.h>
#include "SDL_webrogue_unimplemented.h"

#include "SDL_hints.h"
// #include "SDL_x11video.h"
#include "SDL_webrogueopengles.h"
// #include "SDL_x11opengl.h"

// /* EGL implementation of SDL OpenGL support */

int WEBROGUE_GLES_LoadLibrary(_THIS, const char *path)
{
    return 0;
//     SDL_VideoData *data = (SDL_VideoData *)_this->driverdata;
}

void *WEBROGUE_GLES_GetProcAddress(_THIS, const char *proc)
{
    WR_NOT_IMPLEMENTED;
    return 0;
// #ifdef HAVE_DLOPEN
//     return dlsym( 0 /* RTLD_DEFAULT */, proc);
// #else
//     return NULL;
// #endif
}

void WEBROGUE_GLES_UnloadLibrary(_THIS)
{
    WR_NOT_IMPLEMENTED;
    return;
    // /* FIXME: Support dynamic linking when PNACL supports it */
    // glTerminatePPAPI();
}

int WEBROGUE_GLES_SetSwapInterval(_THIS, int interval)
{
    WR_NOT_IMPLEMENTED;
    return 0;
    // /* STUB */
    // return SDL_Unsupported();
}

int WEBROGUE_GLES_GetSwapInterval(_THIS)
{
    WR_NOT_IMPLEMENTED;
    return 0;
    // /* STUB */
    // return 0;
}



// XVisualInfo *X11_GLES_GetVisual(_THIS, Display *display, int screen)
// {

//     XVisualInfo *egl_visualinfo = NULL;
//     EGLint visual_id;
//     XVisualInfo vi_in;
//     int out_count;

//     if (!_this->egl_data) {
//         /* The EGL library wasn't loaded, SDL_GetError() should have info */
//         return NULL;
//     }

//     if (_this->egl_data->eglGetConfigAttrib(_this->egl_data->egl_display,
//                                             _this->egl_data->egl_config,
//                                             EGL_NATIVE_VISUAL_ID,
//                                             &visual_id) == EGL_FALSE ||
//         !visual_id) {
//         /* Use the default visual when all else fails */
//         vi_in.screen = screen;
//         egl_visualinfo = X11_XGetVisualInfo(display,
//                                             VisualScreenMask,
//                                             &vi_in, &out_count);
//     } else {
//         vi_in.screen = screen;
//         vi_in.visualid = visual_id;
//         egl_visualinfo = X11_XGetVisualInfo(display, VisualScreenMask | VisualIDMask, &vi_in, &out_count);
//     }

//     return egl_visualinfo;
// }

typedef struct WEBROGUE_GLContext {
} WEBROGUE_GLContext;

SDL_GLContext WEBROGUE_GLES_CreateContext(_THIS, SDL_Window *window)
{
    WEBROGUE_GLContext *context = 0;
    // WEBROGUE_GLContext *share_context = 0;
    SDL_VideoData *driverdata = (SDL_VideoData *) _this->driverdata;
    int i = 0;

    // if (_this->gl_config.share_with_current_context) {
    //     share_context = (WEBROGUE_GLContext*) SDL_GL_GetCurrentContext();
    // }

    // attribs[i++] = PP_GRAPHICS3DATTRIB_WIDTH;
    // attribs[i++] = window->w;
    // attribs[i++] = PP_GRAPHICS3DATTRIB_HEIGHT;
    // attribs[i++] = window->h;
    // attribs[i++] = PP_GRAPHICS3DATTRIB_RED_SIZE;
    // attribs[i++] = _this->gl_config.red_size;
    // attribs[i++] = PP_GRAPHICS3DATTRIB_GREEN_SIZE;
    // attribs[i++] = _this->gl_config.green_size;
    // attribs[i++] = PP_GRAPHICS3DATTRIB_BLUE_SIZE;
    // attribs[i++] = _this->gl_config.blue_size;

    // if (_this->gl_config.alpha_size) {
    //     attribs[i++] = PP_GRAPHICS3DATTRIB_ALPHA_SIZE;
    //     attribs[i++] = _this->gl_config.alpha_size;
    // }

    /*if (_this->gl_config.buffer_size) {
        attribs[i++] = EGL_BUFFER_SIZE;
        attribs[i++] = _this->gl_config.buffer_size;
    }*/

    // attribs[i++] = PP_GRAPHICS3DATTRIB_DEPTH_SIZE;
    // attribs[i++] = _this->gl_config.depth_size;

    // if (_this->gl_config.stencil_size) {
    //     attribs[i++] = PP_GRAPHICS3DATTRIB_STENCIL_SIZE;
    //     attribs[i++] = _this->gl_config.stencil_size;
    // }

    // if (_this->gl_config.multisamplebuffers) {
    //     attribs[i++] = PP_GRAPHICS3DATTRIB_SAMPLE_BUFFERS;
    //     attribs[i++] = _this->gl_config.multisamplebuffers;
    // }

    // if (_this->gl_config.multisamplesamples) {
    //     attribs[i++] = PP_GRAPHICS3DATTRIB_SAMPLES;
    //     attribs[i++] = _this->gl_config.multisamplesamples;
    // }

    // attribs[i++] = PP_GRAPHICS3DATTRIB_NONE;

    context = SDL_malloc(sizeof(WEBROGUE_GLContext));

    if (context) {
        /* We need to make the context current, otherwise nothing works */
        SDL_GL_MakeCurrent(window, (SDL_GLContext) context);
    }

    return (SDL_GLContext) context;



    return context;
}

int WEBROGUE_GLES_SwapWindow(_THIS, SDL_Window * window)
{
    SDL_VideoData *driverdata = (SDL_VideoData *) _this->driverdata;
    webrogue_gfx_present();
    return 0;
}

int WEBROGUE_GLES_MakeCurrent(_THIS, SDL_Window * window, SDL_GLContext sdl_context)
{
    SDL_VideoData *driverdata = (SDL_VideoData *) _this->driverdata;
    WEBROGUE_GLContext* context = sdl_context;
    webrogue_gfx_init_ptrs();
    return 0;
}

void WEBROGUE_GLES_DeleteContext(_THIS, SDL_GLContext context)
{
    WR_NOT_IMPLEMENTED;
    return;
    // SDL_VideoData *driverdata = (SDL_VideoData *) _this->driverdata;
    // driverdata->ppb_core->ReleaseResource((PP_Resource) context);
}

#endif /* SDL_VIDEO_DRIVER_WEBROGUE */

    /* vi: set ts=4 sw=4 expandtab: */
