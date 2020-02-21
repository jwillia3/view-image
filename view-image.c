#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

SDL_Surface *bitmap;
double zoom = 1;
int     tx;
int     ty;
int     oldtx;
int     oldty;
int     click_x;
int     click_y;
bool    captured;

void draw(SDL_Window *window) {
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    // SDL_LockSurface(surface);

    SDL_BlitScaled(
        bitmap,
        &(SDL_Rect){tx, ty, bitmap->w * zoom, bitmap->h * zoom},
        surface,
        &(SDL_Rect){0, 0, surface->w, surface->h});

    // SDL_UnlockSurface(surface);
    SDL_UpdateWindowSurface(window);
}

int main(int argc, char **argv) {
    (void) argc;
    ++argv;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    bitmap = SDL_LoadBMP(*argv);
    if (!bitmap) {
        printf("Could not open %s\n", *argv);
        exit(1);
    }

    SDL_Window *window =
        SDL_CreateWindow(
            "Demo",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            bitmap->w,
            bitmap->h,
            SDL_WINDOW_RESIZABLE);

    for (SDL_Event e; SDL_WaitEvent(&e); )
        switch (e.type) {
        case SDL_QUIT:
            exit(0);
            break;
        case SDL_WINDOWEVENT:
            switch (e.window.event) {
            case SDL_WINDOWEVENT_EXPOSED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                draw(window);
                break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            SDL_CaptureMouse(false);
            captured = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            SDL_CaptureMouse(true);
            click_x = e.button.x;
            click_y = e.button.y;
            oldtx = tx;
            oldty = ty;
            captured = true;
            break;
        case SDL_MOUSEMOTION:
            if (captured) {
                tx = oldtx - (e.motion.x - click_x);
                ty = oldty - (e.motion.y - click_y);

                tx = tx < 0? 0: tx > bitmap->w? bitmap->w: tx;
                ty = ty < 0? 0: ty > bitmap->h? bitmap->h: ty;
                draw(window);
            }
            break;
        case SDL_MOUSEWHEEL:
            if (e.wheel.y < 0)
                zoom += 0.1;
            else if (e.wheel.y > 0)
                zoom -= 0.1;
            zoom = zoom < 0? 0: zoom > 1? 1: zoom;
            draw(window);
            break;
        case SDL_KEYDOWN:
            if (e.key.keysym.sym == 27)
                SDL_Quit();
            break;
        }
}
