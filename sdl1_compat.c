#ifdef __EMSCRIPTEN__

#include <SDL/SDL.h>

static SDL_Window *compat_window = NULL;
static SDL_Surface *compat_screen = NULL;

SDL_Surface* SDL_SetVideoMode(int width, int height, int bpp, Uint32 flags)
{
    (void)bpp;
    if (compat_window) {
        SDL_DestroyWindow(compat_window);
        compat_window = NULL;
        compat_screen = NULL;
    }

    Uint32 wflags = 0;
    if (flags & 0x80000000) /* SDL 1.2 SDL_FULLSCREEN */
        wflags |= SDL_WINDOW_FULLSCREEN;

    compat_window = SDL_CreateWindow("Einstein",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, wflags);
    if (!compat_window) return NULL;

    compat_screen = SDL_GetWindowSurface(compat_window);
    return compat_screen;
}

SDL_Surface* SDL_DisplayFormat(SDL_Surface *surface)
{
    if (!compat_screen) return NULL;
    return SDL_ConvertSurface(surface, compat_screen->format, 0);
}

void SDL_UpdateRects(SDL_Surface *screen, int numrects, SDL_Rect *rects)
{
    (void)screen;
    if (compat_window) {
        SDL_UpdateWindowSurfaceRects(compat_window, rects, numrects);
    }
}

void SDL_WM_SetCaption(const char *title, const char *icon)
{
    (void)icon;
    if (compat_window)
        SDL_SetWindowTitle(compat_window, title);
}

int SDL_EnableKeyRepeat(int delay, int interval)
{
    (void)delay;
    (void)interval;
    return 0;
}

int SDL_EnableUNICODE(int enable)
{
    (void)enable;
    return 0;
}

#endif
