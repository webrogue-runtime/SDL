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
#include <libwr4c/wr4c.h>

#define MAP_KEY(wr_key, sdl_key) case WR4C_PHYSICAL_KEY_ ## wr_key: return SDL_SCANCODE_ ## sdl_key;
#define MAP_NAMED_KEY(wr_key, sdl_key) case WR4C_NAMED_KEY_ ## wr_key: return SDLK_ ## sdl_key;

static SDL_Window *FindWebrogueWindow(SDL_VideoDevice *device, wr4c_window_t wr_window)
{
    SDL_Window *window;
    for (window = device->windows; window; window = window->next) {
        SDL_WindowData *window_data = (SDL_WindowData *)window->internal;
        if (window_data && window_data->wr_window == wr_window) {
            return window;
        }
    }
    return NULL;
}

#ifdef SDL_VIDEO_OPENGL_EGL
static void WEBROGUE_BackupEGLContext(SDL_VideoDevice *_this, SDL_Window *window)
{
    SDL_WindowData *window_data = (SDL_WindowData *)window->internal;

    if (SDL_GL_GetCurrentWindow() == window) {
        window_data->egl_context = (EGLContext)SDL_GL_GetCurrentContext();
        window_data->has_swap_interval =
            SDL_GL_GetSwapInterval(&window_data->swap_interval);
        SDL_GL_MakeCurrent(window, NULL);
    }

    if (window_data->egl_surface != EGL_NO_SURFACE) {
        SDL_EGL_DestroySurface(_this, window_data->egl_surface);
        window_data->egl_surface = EGL_NO_SURFACE;
    }
}

static void WEBROGUE_RestoreEGLContext(SDL_VideoDevice *_this, SDL_Window *window)
{
    SDL_WindowData *window_data = (SDL_WindowData *)window->internal;

    if (window_data->egl_surface == EGL_NO_SURFACE) {
        window_data->egl_surface =
            SDL_EGL_CreateSurface(_this, window, window_data->wr_window);
    }
    if (window_data->egl_surface == EGL_NO_SURFACE) {
        return;
    }

    if (window_data->egl_context) {
        SDL_GL_MakeCurrent(window, NULL);
        const bool restored =
            SDL_GL_MakeCurrent(window, (SDL_GLContext)window_data->egl_context);
        if (!restored) {
            // The context may no longer be valid, so create a replacement.
            window_data->egl_context = (EGLContext)SDL_GL_CreateContext(window);
            if (window_data->egl_context) {
                SDL_Event event;
                SDL_zero(event);
                event.type = SDL_EVENT_RENDER_DEVICE_RESET;
                event.render.windowID = SDL_GetWindowID(window);
                SDL_PushEvent(&event);
            }
        }
    }

    if (window_data->has_swap_interval) {
        SDL_GL_SetSwapInterval(window_data->swap_interval);
    }
}
#endif

static SDL_Keycode NamedKeyToKeycode(wr4c_named_key_t key)
{
    switch (key) {
        MAP_NAMED_KEY(UNKNOWN, UNKNOWN)
        MAP_NAMED_KEY(ALT, LALT)
        MAP_NAMED_KEY(ALT_GRAPH, RALT)
        MAP_NAMED_KEY(CAPS_LOCK, CAPSLOCK)
        MAP_NAMED_KEY(CONTROL, LCTRL)
        MAP_NAMED_KEY(NUM_LOCK, NUMLOCKCLEAR)
        MAP_NAMED_KEY(SCROLL_LOCK, SCROLLLOCK)
        MAP_NAMED_KEY(SHIFT, LSHIFT)
        MAP_NAMED_KEY(META, LGUI)
        MAP_NAMED_KEY(ENTER, RETURN)
        MAP_NAMED_KEY(TAB, TAB)
        MAP_NAMED_KEY(ARROW_DOWN, DOWN)
        MAP_NAMED_KEY(ARROW_LEFT, LEFT)
        MAP_NAMED_KEY(ARROW_RIGHT, RIGHT)
        MAP_NAMED_KEY(ARROW_UP, UP)
        MAP_NAMED_KEY(END, END)
        MAP_NAMED_KEY(HOME, HOME)
        MAP_NAMED_KEY(PAGE_DOWN, PAGEDOWN)
        MAP_NAMED_KEY(PAGE_UP, PAGEUP)
        MAP_NAMED_KEY(BACKSPACE, BACKSPACE)
        MAP_NAMED_KEY(CLEAR, CLEAR)
        MAP_NAMED_KEY(COPY, COPY)
        MAP_NAMED_KEY(CR_SEL, CRSEL)
        MAP_NAMED_KEY(CUT, CUT)
        MAP_NAMED_KEY(DELETE, DELETE)
        MAP_NAMED_KEY(INSERT, INSERT)
        MAP_NAMED_KEY(PASTE, PASTE)
        MAP_NAMED_KEY(REDO, AGAIN)
        MAP_NAMED_KEY(UNDO, UNDO)
        MAP_NAMED_KEY(AGAIN, AGAIN)
        MAP_NAMED_KEY(CANCEL, CANCEL)
        MAP_NAMED_KEY(CONTEXT_MENU, APPLICATION)
        MAP_NAMED_KEY(ESCAPE, ESCAPE)
        MAP_NAMED_KEY(EXECUTE, EXECUTE)
        MAP_NAMED_KEY(FIND, FIND)
        MAP_NAMED_KEY(HELP, HELP)
        MAP_NAMED_KEY(PAUSE, PAUSE)
        MAP_NAMED_KEY(SELECT, SELECT)
        MAP_NAMED_KEY(POWER, POWER)
        MAP_NAMED_KEY(PRINT_SCREEN, PRINTSCREEN)
        MAP_NAMED_KEY(BROWSER_BACK, AC_BACK)
        MAP_NAMED_KEY(BROWSER_FAVORITES, AC_BOOKMARKS)
        MAP_NAMED_KEY(BROWSER_FORWARD, AC_FORWARD)
        MAP_NAMED_KEY(BROWSER_HOME, AC_HOME)
        MAP_NAMED_KEY(BROWSER_REFRESH, AC_REFRESH)
        MAP_NAMED_KEY(BROWSER_SEARCH, AC_SEARCH)
        MAP_NAMED_KEY(BROWSER_STOP, AC_STOP)
        MAP_NAMED_KEY(MEDIA_PLAY_PAUSE, MEDIA_PLAY_PAUSE)
        MAP_NAMED_KEY(MEDIA_PLAY, MEDIA_PLAY)
        MAP_NAMED_KEY(MEDIA_PAUSE, MEDIA_PAUSE)
        MAP_NAMED_KEY(MEDIA_STOP, MEDIA_STOP)
        MAP_NAMED_KEY(MEDIA_TRACK_NEXT, MEDIA_NEXT_TRACK)
        MAP_NAMED_KEY(MEDIA_TRACK_PREVIOUS, MEDIA_PREVIOUS_TRACK)
        MAP_NAMED_KEY(MEDIA_FAST_FORWARD, MEDIA_FAST_FORWARD)
        MAP_NAMED_KEY(MEDIA_REWIND, MEDIA_REWIND)
        MAP_NAMED_KEY(AUDIO_VOLUME_DOWN, VOLUMEDOWN)
        MAP_NAMED_KEY(AUDIO_VOLUME_MUTE, MUTE)
        MAP_NAMED_KEY(AUDIO_VOLUME_UP, VOLUMEUP)
        MAP_NAMED_KEY(LAUNCH_MEDIA_PLAYER, MEDIA_SELECT)
        MAP_NAMED_KEY(F1, F1)
        MAP_NAMED_KEY(F2, F2)
        MAP_NAMED_KEY(F3, F3)
        MAP_NAMED_KEY(F4, F4)
        MAP_NAMED_KEY(F5, F5)
        MAP_NAMED_KEY(F6, F6)
        MAP_NAMED_KEY(F7, F7)
        MAP_NAMED_KEY(F8, F8)
        MAP_NAMED_KEY(F9, F9)
        MAP_NAMED_KEY(F10, F10)
        MAP_NAMED_KEY(F11, F11)
        MAP_NAMED_KEY(F12, F12)
        MAP_NAMED_KEY(F13, F13)
        MAP_NAMED_KEY(F14, F14)
        MAP_NAMED_KEY(F15, F15)
        MAP_NAMED_KEY(F16, F16)
        MAP_NAMED_KEY(F17, F17)
        MAP_NAMED_KEY(F18, F18)
        MAP_NAMED_KEY(F19, F19)
        MAP_NAMED_KEY(F20, F20)
        MAP_NAMED_KEY(F21, F21)
        MAP_NAMED_KEY(F22, F22)
        MAP_NAMED_KEY(F23, F23)
        MAP_NAMED_KEY(F24, F24)
        default: return SDLK_UNKNOWN;
    }
}

static SDL_Scancode PhysicalKeyToScancode(wr4c_physical_key_t key)
{
    switch (key) {
        MAP_KEY(UNKNOWN, UNKNOWN)
        MAP_KEY(BACKQUOTE, GRAVE)
        MAP_KEY(BACKSLASH, BACKSLASH)
        MAP_KEY(BRACKET_LEFT, LEFTBRACKET)
        MAP_KEY(BRACKET_RIGHT, RIGHTBRACKET)
        MAP_KEY(COMMA, COMMA)
        MAP_KEY(DIGIT_0, 0)
        MAP_KEY(DIGIT_1, 1)
        MAP_KEY(DIGIT_2, 2)
        MAP_KEY(DIGIT_3, 3)
        MAP_KEY(DIGIT_4, 4)
        MAP_KEY(DIGIT_5, 5)
        MAP_KEY(DIGIT_6, 6)
        MAP_KEY(DIGIT_7, 7)
        MAP_KEY(DIGIT_8, 8)
        MAP_KEY(DIGIT_9, 9)
        MAP_KEY(EQUAL, EQUALS)
        MAP_KEY(INTL_BACKSLASH, NONUSBACKSLASH)
        MAP_KEY(INTL_RO, UNKNOWN)
        MAP_KEY(INTL_YEN, INTERNATIONAL3)
        MAP_KEY(KEY_A, A)
        MAP_KEY(KEY_B, B)
        MAP_KEY(KEY_C, C)
        MAP_KEY(KEY_D, D)
        MAP_KEY(KEY_E, E)
        MAP_KEY(KEY_F, F)
        MAP_KEY(KEY_G, G)
        MAP_KEY(KEY_H, H)
        MAP_KEY(KEY_I, I)
        MAP_KEY(KEY_J, J)
        MAP_KEY(KEY_K, K)
        MAP_KEY(KEY_L, L)
        MAP_KEY(KEY_M, M)
        MAP_KEY(KEY_N, N)
        MAP_KEY(KEY_O, O)
        MAP_KEY(KEY_P, P)
        MAP_KEY(KEY_Q, Q)
        MAP_KEY(KEY_R, R)
        MAP_KEY(KEY_S, S)
        MAP_KEY(KEY_T, T)
        MAP_KEY(KEY_U, U)
        MAP_KEY(KEY_V, V)
        MAP_KEY(KEY_W, W)
        MAP_KEY(KEY_X, X)
        MAP_KEY(KEY_Y, Y)
        MAP_KEY(KEY_Z, Z)
        MAP_KEY(MINUS, MINUS)
        MAP_KEY(PERIOD, PERIOD)
        MAP_KEY(QUOTE, APOSTROPHE)
        MAP_KEY(SEMICOLON, SEMICOLON)
        MAP_KEY(SLASH, SLASH)
        MAP_KEY(ALT_LEFT, LALT)
        MAP_KEY(ALT_RIGHT, RALT)
        MAP_KEY(BACKSPACE, BACKSPACE)
        MAP_KEY(CAPS_LOCK, CAPSLOCK)
        MAP_KEY(CONTEXT_MENU, APPLICATION)
        MAP_KEY(CONTROL_LEFT, LCTRL)
        MAP_KEY(CONTROL_RIGHT, RCTRL)
        MAP_KEY(ENTER, RETURN)
        MAP_KEY(META_LEFT, LGUI)
        MAP_KEY(META_RIGHT, RGUI)
        MAP_KEY(SHIFT_LEFT, LSHIFT)
        MAP_KEY(SHIFT_RIGHT, RSHIFT)
        MAP_KEY(SPACE, SPACE)
        MAP_KEY(TAB, TAB)
        MAP_KEY(CONVERT, UNKNOWN)
        MAP_KEY(KANA_MODE, LANG3)
        MAP_KEY(LANG_1, LANG1)
        MAP_KEY(LANG_2, LANG2)
        MAP_KEY(LANG_3, LANG3)
        MAP_KEY(LANG_4, LANG4)
        MAP_KEY(LANG_5, LANG5)
        MAP_KEY(NON_CONVERT, UNKNOWN)
        MAP_KEY(DELETE, DELETE)
        MAP_KEY(END, END)
        MAP_KEY(HELP, HELP)
        MAP_KEY(HOME, HOME)
        MAP_KEY(INSERT, INSERT)
        MAP_KEY(PAGE_DOWN, PAGEDOWN)
        MAP_KEY(PAGE_UP, PAGEUP)
        MAP_KEY(ARROW_DOWN, DOWN)
        MAP_KEY(ARROW_LEFT, LEFT)
        MAP_KEY(ARROW_RIGHT, RIGHT)
        MAP_KEY(ARROW_UP, UP)
        MAP_KEY(NUM_LOCK, NUMLOCKCLEAR)
        MAP_KEY(NUMPAD_0, KP_0)
        MAP_KEY(NUMPAD_1, KP_1)
        MAP_KEY(NUMPAD_2, KP_2)
        MAP_KEY(NUMPAD_3, KP_3)
        MAP_KEY(NUMPAD_4, KP_4)
        MAP_KEY(NUMPAD_5, KP_5)
        MAP_KEY(NUMPAD_6, KP_6)
        MAP_KEY(NUMPAD_7, KP_7)
        MAP_KEY(NUMPAD_8, KP_8)
        MAP_KEY(NUMPAD_9, KP_9)
        MAP_KEY(NUMPAD_ADD, KP_PLUS)
        MAP_KEY(NUMPAD_BACKSPACE, KP_BACKSPACE)
        MAP_KEY(NUMPAD_CLEAR, KP_CLEAR)
        MAP_KEY(NUMPAD_CLEAR_ENTRY, KP_CLEARENTRY)
        MAP_KEY(NUMPAD_COMMA, KP_COMMA)
        MAP_KEY(NUMPAD_DECIMAL, KP_PERIOD)
        MAP_KEY(NUMPAD_DIVIDE, KP_DIVIDE)
        MAP_KEY(NUMPAD_ENTER, KP_ENTER)
        MAP_KEY(NUMPAD_EQUAL, KP_EQUALS)
        MAP_KEY(NUMPAD_HASH, KP_HASH)
        MAP_KEY(NUMPAD_MEMORY_ADD, KP_MEMADD)
        MAP_KEY(NUMPAD_MEMORY_CLEAR, KP_MEMCLEAR)
        MAP_KEY(NUMPAD_MEMORY_RECALL, KP_MEMRECALL)
        MAP_KEY(NUMPAD_MEMORY_STORE, KP_MEMSTORE)
        MAP_KEY(NUMPAD_MEMORY_SUBTRACT, KP_MEMSUBTRACT)
        MAP_KEY(NUMPAD_MULTIPLY, KP_MULTIPLY)
        MAP_KEY(NUMPAD_PAREN_LEFT, KP_LEFTPAREN)
        MAP_KEY(NUMPAD_PAREN_RIGHT, KP_RIGHTPAREN)
        MAP_KEY(NUMPAD_STAR, UNKNOWN)
        MAP_KEY(NUMPAD_SUBTRACT, KP_MINUS)
        MAP_KEY(ESCAPE, ESCAPE)
        MAP_KEY(FN, UNKNOWN)
        MAP_KEY(FN_LOCK, UNKNOWN)
        MAP_KEY(PRINT_SCREEN, PRINTSCREEN)
        MAP_KEY(SCROLL_LOCK, SCROLLLOCK)
        MAP_KEY(PAUSE, PAUSE)
        MAP_KEY(BROWSER_BACK, AC_BACK)
        MAP_KEY(BROWSER_FAVORITES, AC_BOOKMARKS)
        MAP_KEY(BROWSER_FORWARD, AC_FORWARD)
        MAP_KEY(BROWSER_HOME, AC_HOME)
        MAP_KEY(BROWSER_REFRESH, AC_REFRESH)
        MAP_KEY(BROWSER_SEARCH, AC_SEARCH)
        MAP_KEY(BROWSER_STOP, AC_STOP)
        MAP_KEY(EJECT, MEDIA_EJECT)
        MAP_KEY(LAUNCH_APP_1, UNKNOWN)
        MAP_KEY(LAUNCH_APP_2, UNKNOWN)
        MAP_KEY(LAUNCH_MAIL, UNKNOWN)
        MAP_KEY(MEDIA_PLAY_PAUSE, MEDIA_PLAY_PAUSE)
        MAP_KEY(MEDIA_SELECT, MEDIA_SELECT)
        MAP_KEY(MEDIA_STOP, MEDIA_STOP)
        MAP_KEY(MEDIA_TRACK_NEXT, MEDIA_NEXT_TRACK)
        MAP_KEY(MEDIA_TRACK_PREVIOUS, MEDIA_PREVIOUS_TRACK)
        MAP_KEY(POWER, POWER)
        MAP_KEY(SLEEP, SLEEP)
        MAP_KEY(AUDIO_VOLUME_DOWN, VOLUMEDOWN)
        MAP_KEY(AUDIO_VOLUME_MUTE, MUTE)
        MAP_KEY(AUDIO_VOLUME_UP, VOLUMEUP)
        MAP_KEY(WAKE_UP, UNKNOWN)
        MAP_KEY(ABORT, CANCEL)
        MAP_KEY(RESUME, UNKNOWN)
        MAP_KEY(SUSPEND, UNKNOWN)
        MAP_KEY(AGAIN, AGAIN)
        MAP_KEY(COPY, COPY)
        MAP_KEY(CUT, CUT)
        MAP_KEY(FIND, FIND)
        MAP_KEY(OPEN, AC_NEW)
        MAP_KEY(PASTE, PASTE)
        MAP_KEY(PROPS, AC_PROPERTIES)
        MAP_KEY(SELECT, SELECT)
        MAP_KEY(UNDO, UNDO)
        MAP_KEY(HIRAGANA, LANG4)
        MAP_KEY(KATAKANA, LANG3)
        MAP_KEY(UNIDENTIFIED, UNKNOWN)
        MAP_KEY(F1, F1)
        MAP_KEY(F2, F2)
        MAP_KEY(F3, F3)
        MAP_KEY(F4, F4)
        MAP_KEY(F5, F5)
        MAP_KEY(F6, F6)
        MAP_KEY(F7, F7)
        MAP_KEY(F8, F8)
        MAP_KEY(F9, F9)
        MAP_KEY(F10, F10)
        MAP_KEY(F11, F11)
        MAP_KEY(F12, F12)
        MAP_KEY(F13, F13)
        MAP_KEY(F14, F14)
        MAP_KEY(F15, F15)
        MAP_KEY(F16, F16)
        MAP_KEY(F17, F17)
        MAP_KEY(F18, F18)
        MAP_KEY(F19, F19)
        MAP_KEY(F20, F20)
        MAP_KEY(F21, F21)
        MAP_KEY(F22, F22)
        MAP_KEY(F23, F23)
        MAP_KEY(F24, F24)
        MAP_KEY(F25, UNKNOWN)
        MAP_KEY(F26, UNKNOWN)
        MAP_KEY(F27, UNKNOWN)
        MAP_KEY(F28, UNKNOWN)
        MAP_KEY(F29, UNKNOWN)
        MAP_KEY(F30, UNKNOWN)
        MAP_KEY(F31, UNKNOWN)
        MAP_KEY(F32, UNKNOWN)
        MAP_KEY(F33, UNKNOWN)
        MAP_KEY(F34, UNKNOWN)
        MAP_KEY(F35, UNKNOWN)
        MAP_KEY(BRIGHTNESS_DOWN, UNKNOWN)
        MAP_KEY(BRIGHTNESS_UP, UNKNOWN)
        MAP_KEY(DISPLAY_TOGGLE_INT_EXT, UNKNOWN)
        MAP_KEY(KEYBOARD_LAYOUT_SELECT, UNKNOWN)
        MAP_KEY(LAUNCH_ASSISTANT, UNKNOWN)
        MAP_KEY(LAUNCH_CONTROL_PANEL, UNKNOWN)
        MAP_KEY(LAUNCH_SCREEN_SAVER, UNKNOWN)
        MAP_KEY(MAIL_FORWARD, UNKNOWN)
        MAP_KEY(MAIL_REPLY, UNKNOWN)
        MAP_KEY(MAIL_SEND, UNKNOWN)
        MAP_KEY(MEDIA_FAST_FORWARD, MEDIA_FAST_FORWARD)
        MAP_KEY(MEDIA_PAUSE, MEDIA_PAUSE)
        MAP_KEY(MEDIA_PLAY, MEDIA_PLAY)
        MAP_KEY(MEDIA_RECORD, MEDIA_RECORD)
        MAP_KEY(MEDIA_REWIND, MEDIA_REWIND)
        MAP_KEY(MICROPHONE_MUTE_TOGGLE, UNKNOWN)
        MAP_KEY(PRIVACY_SCREEN_TOGGLE, UNKNOWN)
        MAP_KEY(KEYBOARD_BACKLIGHT_TOGGLE, UNKNOWN)
        MAP_KEY(SELECT_TASK, UNKNOWN)
        MAP_KEY(SHOW_ALL_WINDOWS, UNKNOWN)
        MAP_KEY(ZOOM_TOGGLE, UNKNOWN)
        default: return SDL_SCANCODE_UNKNOWN;
    }
}

void WEBROGUE_PumpEvents(SDL_VideoDevice *_this)
{
    SDL_VideoData *data = (SDL_VideoData *)_this->internal;
    wr4c_event_t event;
    while (1) {
        event = wr4c_poll();
        switch (event.tag) {
        case WR4C_EVENT_TAG_MOUSE_BUTTON:
        {
            SDL_Window *window = FindWebrogueWindow(_this, event.window);
            Uint8 button = 0;
            switch (event.inner.mouse_button.button) {
                case WR4C_MOUSE_BUTTON_LEFT:
                    button = SDL_BUTTON_LEFT;
                    break;
                case WR4C_MOUSE_BUTTON_RIGHT:
                    button = SDL_BUTTON_RIGHT;
                    break;
                case WR4C_MOUSE_BUTTON_MIDDLE:
                    button = SDL_BUTTON_MIDDLE;
                    break;
            }
            if (!window) {
                break;
            }
            SDL_FingerID finger_id = event.inner.mouse_button.touch_id;
            if (finger_id != 0) {
                SDL_TouchID deviceId = 1;
                if (SDL_AddTouch(deviceId, SDL_TOUCH_DEVICE_DIRECT, "") < 0) {
                    break;
                }

                SDL_SendTouch(
                    0,
                    deviceId,
                    finger_id,
                    window,
                    event.inner.mouse_button.down ? SDL_EVENT_FINGER_DOWN : SDL_EVENT_FINGER_UP,
                    (float)event.inner.mouse_button.x / window->w,
                    (float)event.inner.mouse_button.y / window->h,
                    1.0f
                );
            } else if (button) {
                SDL_SendMouseButton(
                    0,
                    window,
                    SDL_DEFAULT_MOUSE_ID,
                    event.inner.mouse_button.button,
                    event.inner.mouse_button.down
                );
            }
        } break;
        case WR4C_EVENT_TAG_MOUSE_MOTION:
        {
            SDL_Window *window = FindWebrogueWindow(_this, event.window);
            if (!window) {
                break;
            }
            SDL_FingerID finger_id = event.inner.mouse_motion.touch_id;
            if (finger_id != 0) {
                SDL_TouchID deviceId = 1;
                if (SDL_AddTouch(deviceId, SDL_TOUCH_DEVICE_DIRECT, "") < 0) {
                    break;
                }

                SDL_SendTouchMotion(
                    0,
                    deviceId, 
                    finger_id, 
                    window, 
                    (float)event.inner.mouse_motion.x / window->w,
                    (float)event.inner.mouse_motion.y / window->h,
                    1.0f
                );
            } else {
                SDL_SendMouseMotion(
                    0,
                    window,
                    SDL_DEFAULT_MOUSE_ID,
                    false,
                    event.inner.mouse_motion.x,
                    event.inner.mouse_motion.y
                );
            }
        } break;
        case WR4C_EVENT_TAG_KEY:
        {
            SDL_SendKeyboardKeyAndKeycode(
                0,
                SDL_GLOBAL_KEYBOARD_ID,
                0,
                PhysicalKeyToScancode(event.inner.key.physical_key),
                NamedKeyToKeycode(event.inner.key.named_key),
                event.inner.key.down
            );
            if(event.inner.key.text) {
                SDL_SendKeyboardText(event.inner.key.text);
            }
        } break;
        case WR4C_EVENT_TAG_QUIT:
        {
            SDL_Event ev;
            ev.type = SDL_EVENT_QUIT;
            SDL_PushEvent(&ev);
        } break;
        case WR4C_EVENT_TAG_RESIZED:
        {
            SDL_Window *window = FindWebrogueWindow(_this, event.window);
            if (!window) {
                break;
            }
            int logical_width, logical_height;
            int physical_width, physical_height;
            wr4c_logical_size(
                ((SDL_WindowData*)window->internal)->wr_window,
                &logical_width,
                &logical_height
            );
            SDL_SendWindowEvent(
                window,
                SDL_EVENT_WINDOW_RESIZED,
                logical_width,
                logical_height
            );
            wr4c_physical_size(
                ((SDL_WindowData*)window->internal)->wr_window,
                &physical_width,
                &physical_height
            );
            SDL_SendWindowEvent(
                window,
                SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED,
                physical_width,
                physical_height
            );
        } break;
        case WR4C_EVENT_TAG_SURFACE_LOST:
        {
            SDL_Window *window = FindWebrogueWindow(_this, event.window);
            if (window) {
#ifdef SDL_VIDEO_OPENGL_EGL
                if (window->flags & SDL_WINDOW_OPENGL) {
                    WEBROGUE_BackupEGLContext(_this, window);
                }
#endif
                SDL_SendWindowEvent(window, SDL_EVENT_WINDOW_MINIMIZED, 0, 0);
            }
            if (!data->app_backgrounded) {
                data->app_backgrounded = true;
                SDL_SendAppEvent(SDL_EVENT_WILL_ENTER_BACKGROUND);
                SDL_SendAppEvent(SDL_EVENT_DID_ENTER_BACKGROUND);
            }
        } break;
        case WR4C_EVENT_TAG_SURFACE_ATTACHED:
        {
            SDL_Window *window = FindWebrogueWindow(_this, event.window);
#ifdef SDL_VIDEO_OPENGL_EGL
            if (window && (window->flags & SDL_WINDOW_OPENGL)) {
                WEBROGUE_RestoreEGLContext(_this, window);
            }
#endif
            if (data->app_backgrounded) {
                data->app_backgrounded = false;
                SDL_SendAppEvent(SDL_EVENT_WILL_ENTER_FOREGROUND);
                SDL_SendAppEvent(SDL_EVENT_DID_ENTER_FOREGROUND);
            }
            if (window) {
                SDL_SendWindowEvent(window, SDL_EVENT_WINDOW_RESTORED, 0, 0);
            }
        } break;
        case WR4C_EVENT_TAG_INVALID:
        {
            return;
        }
        }
    }
    WR_NOT_IMPLEMENTED;
}

#endif // SDL_VIDEO_DRIVER_N3DS
