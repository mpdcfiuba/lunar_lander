#ifndef _DRAW_FUNC_H_
#define _DRAW_FUNC_H_

#include <SDL3/SDL.h>
#include <stdlib.h>

#include "dictionary.h"
#include "aux_n_vector_func.h"

#define NAVE_SIZE 28
#define CHORRO_SIZE 3

#define SCALE_FACTOR	1.0
#define LETTERS_SCALE_FACTOR	VENTANA_ALTO/800.0
#define MSJ_SCALE_FACTOR	(VENTANA_ALTO*1.0/CARACTER_ALTO)/20

// El escalado de los parametros se hizo segun el escalado de los caracteres. Si se cambia el escalado de la nave y terreno no deberian agrandarse las letras

#define FIRST_ROW	VENTANA_ALTO*24/25.0
#define ROW_SIZE	2*CARACTER_ALTO*LETTERS_SCALE_FACTOR
#define FIRST_COLUMN	VENTANA_ANCHO/8.0
#define SECOND_COLUMN	VENTANA_ANCHO*5/8.0
#define MID_COLUMN_SIZE	18*CARACTER_ANCHO*LETTERS_SCALE_FACTOR

void draw_complete_nave_n_terrain(nave_parameters_t* the_nave,float** my_terrain,size_t terrain_size,float scale_factor,SDL_Renderer *renderer);
void draw_phrase(char* phrase,float init_pos_x,float init_pos_y,float scale_factor,SDL_Renderer *renderer);
void draw_all_phrases(const nave_parameters_t* the_nave,int my_score,int my_time,float** terrain,size_t terr_size,float letters_scale_factor,SDL_Renderer *renderer);

#endif //_DRAW_FUNC_H_
