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

#if defined(SDL_VIDEO_VULKAN)

#include "../SDL_vulkan_internal.h"

#include "SDL_webrogue_video.h"

#include "SDL_webrogue_vulkan.h"

PFN_vkVoidFunction vk_icdGetInstanceProcAddr(
    VkInstance                                  instance,
    const char*                                 pName
);

bool Webrogue_Vulkan_LoadLibrary(SDL_VideoDevice *_this, const char *path)
{
    VkExtensionProperties *extensions = NULL;
    Uint32 i, extensionCount = 0;
    bool hasSurfaceExtension = false;
    bool hasWebrogueSurfaceExtension = false;
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = vk_icdGetInstanceProcAddr;

    _this->vulkan_config.vkGetInstanceProcAddr = (void *)vkGetInstanceProcAddr;
    _this->vulkan_config.vkEnumerateInstanceExtensionProperties =
        (void *)((PFN_vkGetInstanceProcAddr)_this->vulkan_config.vkGetInstanceProcAddr)(
            VK_NULL_HANDLE, "vkEnumerateInstanceExtensionProperties");
    if (!_this->vulkan_config.vkEnumerateInstanceExtensionProperties) {
        goto fail;
    }
    extensions = SDL_Vulkan_CreateInstanceExtensionsList(
        (PFN_vkEnumerateInstanceExtensionProperties)
            _this->vulkan_config.vkEnumerateInstanceExtensionProperties,
        &extensionCount);
    if (!extensions) {
        goto fail;
    }
    for (i = 0; i < extensionCount; i++) {
        if (SDL_strcmp(VK_KHR_SURFACE_EXTENSION_NAME, extensions[i].extensionName) == 0) {
            hasSurfaceExtension = true;
        } else if (SDL_strcmp(VK_WEBROGUE_SURFACE_EXTENSION_NAME, extensions[i].extensionName) == 0) {
            hasWebrogueSurfaceExtension = true;
        }
    }
    SDL_free(extensions);
    if (!hasSurfaceExtension) {
        SDL_SetError("Installed Vulkan doesn't implement the " VK_KHR_SURFACE_EXTENSION_NAME " extension");
        goto fail;
    } else if (!hasWebrogueSurfaceExtension) {
        SDL_SetError("Installed Vulkan doesn't implement the " VK_WEBROGUE_SURFACE_EXTENSION_NAME "extension");
        goto fail;
    }
    return true;

fail:
    return false;
}

void Webrogue_Vulkan_UnloadLibrary(SDL_VideoDevice *_this)
{
}

char const* const* Webrogue_Vulkan_GetInstanceExtensions(SDL_VideoDevice *_this, Uint32 *count)
{
    static const char *const extensionsForWebrogue[] = {
        VK_KHR_SURFACE_EXTENSION_NAME, VK_WEBROGUE_SURFACE_EXTENSION_NAME
    };

    if (count) {
        *count = SDL_arraysize(extensionsForWebrogue);
    }

    return extensionsForWebrogue;
}

bool Webrogue_Vulkan_CreateSurface(SDL_VideoDevice *_this,
                                 SDL_Window *window,
                                 VkInstance instance,
                                 const struct VkAllocationCallbacks *allocator,
                                 VkSurfaceKHR *surface)
{
    SDL_WindowData *windowData = window->internal;
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr =
        (PFN_vkGetInstanceProcAddr)_this->vulkan_config.vkGetInstanceProcAddr;
    PFN_vkCreateSurfaceWEBROGUE vkCreateSurfaceWEBROGUE =
        (PFN_vkCreateSurfaceWEBROGUE)vkGetInstanceProcAddr(
            instance,
            "vkCreateSurfaceWEBROGUE");
    VkSurfaceCreateInfoWEBROGUE createInfo;
    VkResult result;

    if (!vkCreateSurfaceWEBROGUE) {
        return SDL_SetError(VK_WEBROGUE_SURFACE_EXTENSION_NAME
                            " extension is not enabled in the Vulkan instance.");
    }
    SDL_zero(createInfo);
    createInfo.sType = VK_STRUCTURE_TYPE_SURFACE_CREATE_INFO_WEBROGUE;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.window = windowData->wr_window;
    result = vkCreateSurfaceWEBROGUE(instance, &createInfo, allocator, surface);
    if (result != VK_SUCCESS) {
        return SDL_SetError("vkCreateSurfaceWEBROGUE failed: %s", SDL_Vulkan_GetResultString(result));
    }
    return true;
}

void Webrogue_Vulkan_DestroySurface(SDL_VideoDevice *_this,
                                   VkInstance instance,
                                   VkSurfaceKHR surface,
                                   const struct VkAllocationCallbacks *allocator)
{
    SDL_Vulkan_DestroySurface_Internal(_this->vulkan_config.vkGetInstanceProcAddr, instance, surface, allocator);
}

#endif
