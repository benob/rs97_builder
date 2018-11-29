#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL/SDL.h>
#include "font.h"

int cursor_x = 0, cursor_y = 0;

void SDL_clear(SDL_Surface* buffer) {
  SDL_Rect r = {0, 0, buffer->w, buffer->h};
  SDL_FillRect(buffer, &r, 0);
  cursor_x = cursor_y = 0;
}

void SDL_shift_up(SDL_Surface* surface) {
  SDL_LockSurface(surface);
  memmove(surface->pixels, surface->pixels + 8 * surface->pitch, surface->pitch * (surface->h - 8));
  memset(surface->pixels + (surface->h - 8) * surface->pitch, 0, surface->pitch * 8);
  SDL_UnlockSurface(surface);
}

void SDL_printf(SDL_Surface* surface, const char* format, ...) {
  static char* result = NULL;
  va_list arglist;
  va_start(arglist, format);
  vasprintf(&result, format, arglist);
  va_end(arglist);

  unsigned short color = SDL_MapRGB(surface->format, 255, 255, 255); 
  while(*result != '\0') {
    if(*result == '\n') {
      cursor_x = 0;
      if(cursor_y >= surface->h - 8) {
        SDL_shift_up(surface);
      } else {
        cursor_y += 8;
      }
    } else {
      draw_char(surface, *result, cursor_x, cursor_y, color);
      cursor_x += 6;
      if(cursor_x > surface->w) {
        cursor_x = 0;
        if(cursor_y >= surface->h - 8) {
          SDL_shift_up(surface);
        } else {
          cursor_y += 8;
        }
      }
    }
    result++;
  }
}

int main() {
  SDL_Init( SDL_INIT_EVERYTHING );
  SDL_Surface* screen = SDL_SetVideoMode(320, 480, 16, SDL_SWSURFACE );
  SDL_Surface* buffer = SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 240, 16, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);

  int quit = 0;
  while(!quit) {
    SDL_Event event;
    while( SDL_PollEvent( &event ) ) {
      if( event.type == SDL_QUIT ) { 
        quit= 1;
        break;
      } else {
        switch(event.type) {
          case SDL_KEYDOWN:
            SDL_printf(buffer, "KEYDOWN: state=%d scancode=%d sym=%d mod=%d unicode=%d\n", event.key.state, event.key.keysym.scancode, event.key.keysym.sym, event.key.keysym.mod, event.key.keysym.unicode);
            if(event.key.keysym.sym == SDLK_ESCAPE) {
              quit = 1;
              break;
            }
            break;
          case SDL_KEYUP:
            SDL_printf(buffer, "KEYUP: state=%d scancode=%d sym=%d mod=%d unicode=%d\n", event.key.state, event.key.keysym.scancode, event.key.keysym.sym, event.key.keysym.mod, event.key.keysym.unicode);
            break;
          case SDL_VIDEORESIZE:
            SDL_printf(buffer, "VIDEORESIZE\n");
            break;
          case SDL_VIDEOEXPOSE:
            SDL_printf(buffer, "VIDEOEXPOSE\n");
            break;
            break;
        }
      }
    }
    SDL_LockSurface(buffer);
    SDL_LockSurface(screen);
    for(int j = 0; j < buffer->h; j++) {
      memcpy(screen->pixels + j * 2 * screen->pitch, buffer->pixels + j * buffer->pitch, buffer->w * 2);
      memcpy(screen->pixels + (j * 2 + 1) * screen->pitch, buffer->pixels + j * buffer->pitch, buffer->w * 2);
    }
    SDL_UnlockSurface(buffer);
    SDL_UnlockSurface(screen);

    SDL_UnlockSurface(buffer);
    SDL_Flip(screen);
    SDL_Delay(1000 / 30);
  }
  SDL_Quit();
}

