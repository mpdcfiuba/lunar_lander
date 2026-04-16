#include "aux_n_vector_func.h"
#include "dictionary.h"
#include "draw_func.h"

#include <SDL3/SDL.h>

#include "caracteres.h"
#include "config.h"
#include "naves.h"

#include <math.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event event;

  SDL_CreateWindowAndRenderer("Lunar Lander", VENTANA_ANCHO, VENTANA_ALTO, 0,
                              &window, &renderer);

  int dormir = 0;

  srand(time(NULL));

  float f = SCALE_FACTOR;

  nave_parameters_t the_nave;
  (the_nave.my_fuel) = JUEGO_COMBUSTIBLE_INICIAL;
  int my_score = 0;
  int my_time = 0;

  bool starting_level = 1;
  size_t terrain_size = 0;
  float **my_terrain;

  unsigned int ticks = SDL_GetTicks();
  while (1) {
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT)
        break;
      if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
        case SDLK_UP:
          if ((the_nave.my_power) < NAVE_MAX_POTENCIA) {
            (the_nave.my_power) += ((float)NAVE_MAX_POTENCIA / 7);
          }
          if ((the_nave.my_power) > NAVE_MAX_POTENCIA) {
            (the_nave.my_power) = NAVE_MAX_POTENCIA;
          }
          break;
        case SDLK_DOWN:
          if ((the_nave.my_power) > 0) {
            (the_nave.my_power) -= ((float)NAVE_MAX_POTENCIA / 4);
          }
          if ((the_nave.my_power) < 0) {
            (the_nave.my_power) = 0;
          }
          break;
        case SDLK_RIGHT:
          if ((the_nave.my_angle) > (-PI / 2)) {
            (the_nave.my_angle) -= NAVE_ROTACION_PASO;
            (the_nave.my_fuel) -=
                JUEGO_COMBUSTIBLE_RADIANES * NAVE_ROTACION_PASO;
          }
          if ((the_nave.my_angle) < (-PI / 2))
            (the_nave.my_angle) = -PI / 2;
          break;
        case SDLK_LEFT:
          if ((the_nave.my_angle) < (PI / 2)) {
            (the_nave.my_angle) += NAVE_ROTACION_PASO;
            (the_nave.my_fuel) -=
                JUEGO_COMBUSTIBLE_RADIANES * NAVE_ROTACION_PASO;
          }
          if ((the_nave.my_angle) > (PI / 2))
            (the_nave.my_angle) = PI / 2;
          break;
        }
      }
      continue;
    }
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);

    if (starting_level) {
      (the_nave.pos_x) = NAVE_X_INICIAL;
      (the_nave.pos_y) = NAVE_Y_INICIAL;
      (the_nave.my_angle) = NAVE_ANGULO_INICIAL;
      (the_nave.vel_x) = NAVE_VX_INICIAL;
      (the_nave.vel_y) = NAVE_VY_INICIAL;
      (the_nave.my_power) = NAVE_POTENCIA_INICIAL;

      my_terrain = crear_terreno(&terrain_size);
      starting_level = 0;
    }

    if ((the_nave.my_fuel) <= 0) {
      vector_destruir(my_terrain, terrain_size);
      fprintf(stdout, "%d", my_score);
      break;
    }

    compute_n_fix_all(&the_nave);

    my_time++;

    draw_complete_nave_n_terrain(&the_nave, my_terrain, terrain_size, f,
                                 renderer);
    draw_all_phrases(&the_nave, my_score, my_time, my_terrain, terrain_size,
    LETTERS_SCALE_FACTOR, renderer);

    if (!vector_esta_arriba(my_terrain, terrain_size, (the_nave.pos_x),
                            (the_nave.pos_y) + NAVE_GRANDE_TOBERA_Y)) {
      if (fabs((the_nave.vel_x)) < 1 && fabs((the_nave.vel_y)) < 10 &&
          fabs((the_nave.my_angle)) < 0.01) {
        my_score += 50;
        draw_phrase("YOU HAVE LANDED",
                    (float)VENTANA_ANCHO / 2 -
                        7 * CARACTER_ANCHO * MSJ_SCALE_FACTOR,
                    (VENTANA_ALTO + CARACTER_ALTO * MSJ_SCALE_FACTOR) / 2,
                    MSJ_SCALE_FACTOR, renderer);
      } else if (fabs((the_nave.vel_x)) < 2 && fabs((the_nave.vel_y)) < 20 &&
                 fabs((the_nave.my_angle)) < 0.01) {
        my_score += 15;
        draw_phrase("YOU LANDED HARD",
                    (float)VENTANA_ANCHO / 2 -
                        7 * CARACTER_ANCHO * MSJ_SCALE_FACTOR,
                    (VENTANA_ALTO + CARACTER_ALTO * MSJ_SCALE_FACTOR) / 2,
                    MSJ_SCALE_FACTOR, renderer);
      } else {
        (the_nave.my_fuel) -= 250;
        draw_phrase("DESTROYED",
                    (float)VENTANA_ANCHO / 2 -
                        5 * CARACTER_ANCHO * MSJ_SCALE_FACTOR,
                    (VENTANA_ALTO + CARACTER_ALTO * MSJ_SCALE_FACTOR) / 2,
                    MSJ_SCALE_FACTOR, renderer);
      }

      vector_destruir(my_terrain, terrain_size);
      starting_level = 1;
      dormir = 2000;
    }

    SDL_RenderPresent(renderer);
    ticks = SDL_GetTicks() - ticks;
    if (dormir) {
      SDL_Delay(dormir);
      dormir = 0;
    } else if (ticks < 1000 / JUEGO_FPS)
      SDL_Delay(1000 / JUEGO_FPS - ticks);
    ticks = SDL_GetTicks();
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();
  return 0;
}
