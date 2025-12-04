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

#ifdef SDL_VIDEO_DRIVER_WEBROGUE

#include "SDL_webrogue_unimplemented.h"

#include "../../events/SDL_events_c.h"
#include "SDL_webrogueevents_c.h"
#include "SDL_webroguevideo.h"
#include <webroguegfx/webroguegfx.h>

#define MAP_KEY(wr_key, sdl_key) case WEBROGUE_PHYSICAL_KEY_ ## wr_key: return SDL_SCANCODE_ ## sdl_key;

static SDL_Scancode PhysicalKeyToScancode(webrogue_physical_key key)
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
        MAP_KEY(EJECT, EJECT)
        MAP_KEY(LAUNCH_APP_1, APP1)
        MAP_KEY(LAUNCH_APP_2, APP2)
        MAP_KEY(LAUNCH_MAIL, MAIL)
        MAP_KEY(MEDIA_PLAY_PAUSE, AUDIOPLAY)
        MAP_KEY(MEDIA_SELECT, MEDIASELECT)
        MAP_KEY(MEDIA_STOP, AUDIOSTOP)
        MAP_KEY(MEDIA_TRACK_NEXT, AUDIONEXT)
        MAP_KEY(MEDIA_TRACK_PREVIOUS, AUDIOPREV)
        MAP_KEY(POWER, POWER)
        MAP_KEY(SLEEP, SLEEP)
        MAP_KEY(AUDIO_VOLUME_DOWN, VOLUMEDOWN)
        MAP_KEY(AUDIO_VOLUME_MUTE, MUTE)
        MAP_KEY(AUDIO_VOLUME_UP, VOLUMEUP)
        MAP_KEY(WAKE_UP, UNKNOWN)
        MAP_KEY(HYPER, UNKNOWN)
        MAP_KEY(SUPER, UNKNOWN)
        MAP_KEY(TURBO, UNKNOWN)
        MAP_KEY(ABORT, CANCEL)
        MAP_KEY(RESUME, UNKNOWN)
        MAP_KEY(SUSPEND, UNKNOWN)
        MAP_KEY(AGAIN, AGAIN)
        MAP_KEY(COPY, COPY)
        MAP_KEY(CUT, CUT)
        MAP_KEY(FIND, FIND)
        MAP_KEY(OPEN, UNKNOWN)
        MAP_KEY(PASTE, PASTE)
        MAP_KEY(PROPS, UNKNOWN)
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
        MAP_KEY(BRIGHTNESS_DOWN, BRIGHTNESSDOWN)
        MAP_KEY(BRIGHTNESS_UP, BRIGHTNESSUP)
        MAP_KEY(DISPLAY_TOGGLE_INT_EXT, DISPLAYSWITCH)
        MAP_KEY(KEYBOARD_LAYOUT_SELECT, UNKNOWN)
        MAP_KEY(LAUNCH_ASSISTANT, UNKNOWN)
        MAP_KEY(LAUNCH_CONTROL_PANEL, COMPUTER)
        MAP_KEY(LAUNCH_SCREEN_SAVER, UNKNOWN)
        MAP_KEY(MAIL_FORWARD, UNKNOWN)
        MAP_KEY(MAIL_REPLY, UNKNOWN)
        MAP_KEY(MAIL_SEND, UNKNOWN)
        MAP_KEY(MEDIA_FAST_FORWARD, AUDIOFASTFORWARD)
        MAP_KEY(MEDIA_PAUSE, AUDIOPLAY)
        MAP_KEY(MEDIA_PLAY, AUDIOPLAY)
        MAP_KEY(MEDIA_RECORD, UNKNOWN)
        MAP_KEY(MEDIA_REWIND, AUDIOREWIND)
        MAP_KEY(MICROPHONE_MUTE_TOGGLE, UNKNOWN)
        MAP_KEY(PRIVACY_SCREEN_TOGGLE, UNKNOWN)
        MAP_KEY(KEYBOARD_BACKLIGHT_TOGGLE, KBDILLUMTOGGLE)
        MAP_KEY(SELECT_TASK, UNKNOWN)
        MAP_KEY(SHOW_ALL_WINDOWS, UNKNOWN)
        MAP_KEY(ZOOM_TOGGLE, UNKNOWN)
        default: return SDL_SCANCODE_UNKNOWN;
    }
}

void WEBROGUE_PumpEvents(_THIS)
{
    SDL_VideoData *data = (SDL_VideoData *)_this->driverdata;
    webrogue_event event;
    while (1) {
        event = webroguegfx_poll();
        switch (event.type) {
        case WEBROGUE_EVENT_TYPE_MOUSE_BUTTON:
        {
            Uint8 button = 0;
            switch (event.inner.mouse_button.button) {
                case WEBROGUE_MOUSE_BUTTON_LEFT:
                    button = SDL_BUTTON_LEFT;
                    break;
                case WEBROGUE_MOUSE_BUTTON_RIGHT:
                    button = SDL_BUTTON_RIGHT;
                    break;
                case WEBROGUE_MOUSE_BUTTON_MIDDLE:
                    button = SDL_BUTTON_MIDDLE;
                    break;
            }
            if (button) {
                SDL_SendMouseButton(
                    data->latest_window, 
                    0, 
                    event.inner.mouse_button.down ? SDL_PRESSED : SDL_RELEASED, 
                    button
                );
            }
        } break;
        case WEBROGUE_EVENT_TYPE_MOUSE_MOTION:
        {
            SDL_SendMouseMotion(
                data->latest_window,
                0,
                0,
                event.inner.mouse_motion.x,
                event.inner.mouse_motion.y
            );
        } break;
        case WEBROGUE_EVENT_TYPE_KEY:
        {
            SDL_SendKeyboardKey(
                event.inner.key.down ? SDL_PRESSED : SDL_RELEASED,
                PhysicalKeyToScancode(event.inner.key.physical_key)
            );
            if(event.inner.key.text) {
                SDL_SendKeyboardText(event.inner.key.text);
            }
        } break;
        case WEBROGUE_EVENT_TYPE_QUIT:
        {
            SDL_Event ev;
            ev.type = SDL_QUIT;
            SDL_PushEvent(&ev);
        } break;
        case WEBROGUE_EVENT_TYPE_WINDOW_RESIZED:
        {
            int width, height;
            webroguegfx_window_size(
                ((SDL_WindowData*)data->latest_window->driverdata)->wr_window,
                &width,
                &height
            );
            SDL_SendWindowEvent(
                data->latest_window,
                SDL_WINDOWEVENT_RESIZED,
                width,
                height
            );
        } break;
        // case WEBROGUE_EVENT_TYPE_GL_RESIZED:
        // {
        //     int width, height;
        //     webroguegfx_gl_size(
        //         ((SDL_WindowData*)data->latest_window->driverdata)->wr_window,
        //         &width,
        //         &height
        //     );
        //     SDL_SendWindowEvent(
        //         data->latest_window,
        //         SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED,
        //         width,
        //         height
        //     );
        // } break;
        case WEBROGUE_EVENT_TYPE_INVALID:
        {
            return;
        }
        }
    }
    WR_NOT_IMPLEMENTED;
}

#endif /* SDL_VIDEO_DRIVER_WEBROGUE */

/* vi: set sts=4 ts=4 sw=4 expandtab: */
