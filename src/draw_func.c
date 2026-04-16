#include "draw_func.h"
#include "aux_n_vector_func.h"
#include "dictionary.h"

#include <SDL3/SDL.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "config.h"
#include "naves.h"
#include "caracteres.h"

static const float (*nave)[2] = nave_grande;

static const float chorro[CHORRO_SIZE][2] =
{
	{-NAVE_GRANDE_TOBERA_X, NAVE_GRANDE_TOBERA_Y},
	{0, NAVE_GRANDE_TOBERA_Y},
	{NAVE_GRANDE_TOBERA_X, NAVE_GRANDE_TOBERA_Y}
};

// Dibuja un vector partiendo de la posicion de pantalla abajo a la izquierda
static void draw_vector(float **v,size_t vector_size,float pos_x,float pos_y,float scale_factor,SDL_Renderer *renderer)
{
	for(size_t i=0; i < (vector_size - 1); i++)
			SDL_RenderLine(
				renderer,
				v[i][0] * scale_factor + pos_x,
				VENTANA_ALTO - (v[i][1] * scale_factor + pos_y),
				v[i+1][0] * scale_factor + pos_x,
				VENTANA_ALTO - (v[i+1][1] * scale_factor + pos_y)
			);
}

// Idem draw_vector pero recibe un arreglo estatico en vez de dinamico
static void draw_vector_letra(character_t v,size_t vector_size,float pos_x,float pos_y,float scale_factor,SDL_Renderer *renderer)
{
	for(size_t i=0; i < (vector_size - 1); i++)
			SDL_RenderLine(
				renderer,
				v[i][0] * scale_factor + pos_x,
				VENTANA_ALTO - (v[i][1] * scale_factor + pos_y),
				v[i+1][0] * scale_factor + pos_x,
				VENTANA_ALTO - (v[i+1][1] * scale_factor + pos_y)
			);
}

// Cargo en un vector dinamico la nave y el chorro, le doy la longitud al chorro segun la potencia, los roto segun my_angle, los traslado según la nueva posicion computada, previa revision si deben reaparecer del otro lado de la pantalla, y por ultimo dibujo los vectores finales antes de destruirlos
void draw_complete_nave_n_terrain(nave_parameters_t* the_nave,float** my_terrain,size_t terrain_size,float scale_factor,SDL_Renderer *renderer)
{
		float** my_nave;
		float** my_chorro;
		my_nave=matriz_a_vector(nave,NAVE_SIZE);
		my_chorro=matriz_a_vector(chorro,CHORRO_SIZE);

		my_chorro[1][1]= ((the_nave->my_power)/NAVE_MAX_POTENCIA)*(-30) + NAVE_GRANDE_TOBERA_Y;

		vector_rotar(my_nave,NAVE_SIZE,(the_nave->my_angle));
		vector_rotar(my_chorro,CHORRO_SIZE,(the_nave->my_angle));

		vector_trasladar(my_nave,NAVE_SIZE,(the_nave->pos_x),(the_nave->pos_y));
		vector_trasladar(my_chorro,CHORRO_SIZE,(the_nave->pos_x),(the_nave->pos_y));

		draw_vector(my_terrain,terrain_size,0,0,scale_factor,renderer);
		draw_vector(my_nave,NAVE_SIZE,0,0,scale_factor,renderer);
		draw_vector(my_chorro,CHORRO_SIZE,0,0,scale_factor,renderer);

		vector_destruir(my_nave,NAVE_SIZE);
		vector_destruir(my_chorro,CHORRO_SIZE);
}

// Dibujo una cadena de caracteres comenzando por init_pos_x e init_pos_y separada por CARACTER_ANCHO escalado por letters_scale_factor
void draw_phrase(char* phrase,float init_pos_x,float init_pos_y,float letters_scale_factor,SDL_Renderer *renderer)
{
	size_t phrase_size=strlen(phrase);
	dictionary_t* the_letter=NULL;

	for(size_t i=0;i<phrase_size;i++)
	{
		letter_to_func(phrase[i],&the_letter);
		draw_vector_letra(the_letter->my_character,the_letter->my_size,init_pos_x+(CARACTER_ANCHO*i*letters_scale_factor),init_pos_y,letters_scale_factor,renderer);
	}
}

// Dibujo los valores de los parametros introduciendolos en una cadena y pasandoselos a la funcion creada anteriormente para este fin
void draw_all_phrases(const nave_parameters_t* the_nave,int my_score,int my_time,float** terrain,size_t terr_size,float letters_scale_factor,SDL_Renderer *renderer)
{
	draw_phrase("SCORE",FIRST_COLUMN,FIRST_ROW,letters_scale_factor,renderer);
	draw_phrase("TIME",FIRST_COLUMN,FIRST_ROW-ROW_SIZE,letters_scale_factor,renderer);
	draw_phrase("FUEL",FIRST_COLUMN,FIRST_ROW-2*ROW_SIZE,letters_scale_factor,renderer);
	draw_phrase("ALTITUDE",SECOND_COLUMN,FIRST_ROW,letters_scale_factor,renderer);
	draw_phrase("HORIZONTAL SPEED",SECOND_COLUMN,FIRST_ROW-ROW_SIZE,letters_scale_factor,renderer);
	draw_phrase("VERTICAL SPEED",SECOND_COLUMN,FIRST_ROW-2*ROW_SIZE,letters_scale_factor,renderer);

	char my_string[12];
	sprintf(my_string,"%04d",my_score);
	draw_phrase(my_string,FIRST_COLUMN+(MID_COLUMN_SIZE/2.0),FIRST_ROW,letters_scale_factor,renderer);
	sprintf(my_string,"%04d",(int)((1.0*my_time)/JUEGO_FPS));
	draw_phrase(my_string,FIRST_COLUMN+(MID_COLUMN_SIZE/2.0),FIRST_ROW-ROW_SIZE,letters_scale_factor,renderer);
	sprintf(my_string,"%04d",(int)(the_nave->my_fuel));
	draw_phrase(my_string,FIRST_COLUMN+(MID_COLUMN_SIZE/2.0),FIRST_ROW-2*ROW_SIZE,letters_scale_factor,renderer);
  sprintf(my_string,"%4d",(int)abs((int)((the_nave->pos_y)-request_y_from_x(terrain,terr_size,(the_nave->pos_x)))));
	draw_phrase(my_string,SECOND_COLUMN+MID_COLUMN_SIZE,FIRST_ROW,letters_scale_factor,renderer);
	sprintf(my_string,"%4d",(int)fabs((the_nave->vel_x)));
	draw_phrase(my_string,SECOND_COLUMN+MID_COLUMN_SIZE,FIRST_ROW-ROW_SIZE,letters_scale_factor,renderer);

	if((the_nave->vel_x)>0)
	{
		draw_phrase(">",SECOND_COLUMN+MID_COLUMN_SIZE*(5/4.0),FIRST_ROW-ROW_SIZE,letters_scale_factor,renderer);
	}
	else if((the_nave->vel_x)<0)
	{
		draw_phrase("<",SECOND_COLUMN+MID_COLUMN_SIZE*(5/4.0),FIRST_ROW-ROW_SIZE,letters_scale_factor,renderer);
	}

	sprintf(my_string,"%4d",(int)fabs((the_nave->vel_y)));
	draw_phrase(my_string,SECOND_COLUMN+MID_COLUMN_SIZE,FIRST_ROW-2*ROW_SIZE,letters_scale_factor,renderer);

	if((the_nave->vel_y)>0)
	{
		draw_phrase("^",SECOND_COLUMN+MID_COLUMN_SIZE*(5/4.0),FIRST_ROW-2*ROW_SIZE,letters_scale_factor,renderer);
	}
	else if((the_nave->vel_y)<0)
	{
		draw_phrase("v",SECOND_COLUMN+MID_COLUMN_SIZE*(5/4.0),FIRST_ROW-2*ROW_SIZE,letters_scale_factor,renderer);
	}
}
