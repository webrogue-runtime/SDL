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

#include "../../SDL_properties_c.h"
#include "../SDL_sysvideo.h"
#include "SDL_webrogue_framebuffer_c.h"
#include "SDL_webrogue_unimplemented.h"
#include "SDL_webrogue_video.h"

bool SDL_Webrogue_CreateWindowFramebuffer(SDL_VideoDevice *_this, SDL_Window *window, SDL_PixelFormat *format, void **pixels, int *pitch)
{
    WR_NOT_IMPLEMENTED;
}

bool SDL_Webrogue_UpdateWindowFramebuffer(SDL_VideoDevice *_this, SDL_Window *window, const SDL_Rect *rects, int numrects)
{
    WR_NOT_IMPLEMENTED;
}

void SDL_Webrogue_DestroyWindowFramebuffer(SDL_VideoDevice *_this, SDL_Window *window)
{
    WR_NOT_IMPLEMENTED;
}

#endif // SDL_VIDEO_DRIVER_WEBROGUE
