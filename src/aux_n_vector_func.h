#ifndef _AUX_N_VECTOR_FUNC_H_
#define _AUX_N_VECTOR_FUNC_H_

#include <stdbool.h>
#include <stdlib.h>

#define DT 1.0/JUEGO_FPS

typedef struct
{
	float pos_x;
	float pos_y;
	double my_angle;
	float vel_x;
	float vel_y;
	float my_power;
	float my_fuel;
}
nave_parameters_t;

void compute_n_fix_all(nave_parameters_t* the_nave);

bool vector_esta_arriba(float **v,size_t n,float x,float y);
void vector_trasladar(float **v,size_t n,float dx,float dy);
void vector_rotar(float **v,size_t n,double rad);
float** vector_create(size_t n);
float **matriz_a_vector(const float m[][2], size_t n);
void vector_destruir(float** v,size_t n);
float **crear_terreno(size_t *n);
float request_y_from_x(float** v,size_t n,float x_req);

#endif // _VECTOR_FUNC_H_
