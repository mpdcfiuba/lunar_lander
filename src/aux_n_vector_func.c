#include "aux_n_vector_func.h"

#include "config.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static double computar_velocidad(double vi, double a) { return a * DT + vi; }

static double computar_posicion(double pi, double vi) { return vi * DT + pi; }

void compute_n_fix_all(nave_parameters_t *the_nave) {
  (the_nave->vel_x) = computar_velocidad(
      (the_nave->vel_x), -(the_nave->my_power) * sin(the_nave->my_angle));
  (the_nave->vel_y) = computar_velocidad(
      (the_nave->vel_y), -G + ((the_nave->my_power) * cos(the_nave->my_angle)));

  (the_nave->pos_x) = computar_posicion((the_nave->pos_x), (the_nave->vel_x));
  (the_nave->pos_y) = computar_posicion((the_nave->pos_y), (the_nave->vel_y));

  if ((the_nave->pos_x) >= (VENTANA_ANCHO)) {
    (the_nave->pos_x) -= (VENTANA_ANCHO - 10);
  }

  if ((the_nave->pos_x) <= 0) {
    (the_nave->pos_x) += (VENTANA_ANCHO - 10);
  }

  if ((the_nave->my_angle) < (NAVE_ROTACION_PASO / 4.0) &&
      (the_nave->my_angle) > (-NAVE_ROTACION_PASO / 4.0)) {
    the_nave->my_angle = 0;
  }

  if ((the_nave->my_power) > 0) {
    (the_nave->my_fuel) -=
        JUEGO_COMBUSTIBLE_POT_X_SEG * (the_nave->my_power) / NAVE_MAX_POTENCIA;
  }
}

bool vector_esta_arriba(float **v, size_t n, float x, float y) {

  for (size_t i = 0; i < (n - 1); i++) {
    if (x >= v[i][0] && x <= v[i + 1][0]) {
      float pend;
      float finaly;
      float y0 = v[i][1];
      float y1 = v[i + 1][1];
      float x0 = v[i][0];
      float x1 = v[i + 1][0];
      pend = (y1 - y0) / (x1 - x0);
      finaly = pend * (x - x0) + y0;
      return finaly < y;
    }
  }
  return false;
}

void vector_trasladar(float **v, size_t n, float dx, float dy) {
  for (size_t i = 0; i < n; i++) {
    v[i][0] += dx;
    v[i][1] += dy;
  }
}

void vector_rotar(float **v, size_t n, double rad) {
  double crad = cos(rad);
  double srad = sin(rad);

  for (size_t i = 0; i < n; i++) {
    double prevx = v[i][0];
    double prevy = v[i][1];

    v[i][0] = prevx * crad - prevy * srad;
    v[i][1] = prevx * srad + prevy * crad;
  }
}

float **vector_create(size_t n) {
  float **v;
  if (!(v = (malloc(sizeof(float *) * n)))) {
    return NULL;
  }

  for (size_t i = 0; i < n; i++) {
    if (!(v[i] = ((float *)malloc(sizeof(float) * 2)))) {
      vector_destruir(v, i);
      return NULL;
    }
  }
  return v;
}

static void matrix_copy(const float om[][2], float **nm, size_t n) {
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < 2; j++) {
      nm[i][j] = om[i][j];
    }
  }
}

float **matriz_a_vector(const float m[][2], size_t n) {
  float **v;
  v = vector_create(n);
  if (!v) {
    return NULL;
  }
  matrix_copy(m, v, n);
  return v;
}

void vector_destruir(float **v, size_t n) {
  for (size_t i = 0; i < n; i++) {
    free(v[i]);
  }
  free(v);
  v = NULL;
}

// Devuelve el valor de y de un vector ordenado dado un x
float request_y_from_x(float **v, size_t n, float x_req) {
  for (size_t i = 0; i < (n - 1); i++) {
    if ((v[i][0] <= x_req) && (v[i + 1][0] > x_req)) {
      float slope = (v[i + 1][1] - v[i][1]) / (v[i + 1][0] - v[i][0]);
      float var_x = x_req - (v[i][0]);
      return ((v[i][1]) + (var_x * slope));
    }
  }
  return 0;
}

// A partir de este lugar comienzan las funciones hechas para densificar vector y la que lo implementa en el terreno
int float_cmp(const void *a, const void *b) {
  const float *A = a, *B = b;
  return (*A > *B) - (*A < *B);
}

static float calc_rand_between(float a, float b) {
  if (a == b)
    return a;

  if (a > b) {
    float to_return = b + ((a - b) * (rand() / (RAND_MAX * 1.0)));
    if (!to_return)
      return calc_rand_between(a, b);
    return to_return;
  }

  float to_return = a + ((b - a) * (rand() / (RAND_MAX * 1.0)));
  if (!to_return)
    return calc_rand_between(a, b);
  return to_return;
}

static float **vector_densificar(float **v, size_t nv, size_t nn,
                                 float margen) {
  float **v_final;

  if (!(v_final = vector_create(nn))) {
    return NULL;
  }

  size_t count = 0;
  size_t old_vector_count = 0;
  size_t aux_vector_count = 0;
  int quantity_add = nn - nv;
  float x_vector_final = v[nv - 1][0];
  float x_vector_inicial = v[0][0];

  float v_coord_x[quantity_add];

  for (size_t i = 0; i < quantity_add; i++) {
    v_coord_x[i] = calc_rand_between(x_vector_inicial, x_vector_final);
  }

  qsort(v_coord_x, quantity_add, sizeof(float), float_cmp);

  while (count < nn) {
    if ((v[old_vector_count][0]) <= (v_coord_x[aux_vector_count]) ||
        (count == (nn - 1)) || (aux_vector_count == (quantity_add))) {
      v_final[count][0] = v[old_vector_count][0];
      v_final[count][1] = v[old_vector_count][1];
      if (old_vector_count < (nv - 1))
        old_vector_count++;
    } else {
      v_final[count][0] = v_coord_x[aux_vector_count];
      float new_y = request_y_from_x(v, nv, v_coord_x[aux_vector_count]);
      v_final[count][1] = calc_rand_between(new_y - margen, new_y + margen);
      if (aux_vector_count < (quantity_add))
        aux_vector_count++;
    }
    count++;
  }
  return v_final;
}

float **crear_terreno(size_t *n) {
  *n = 0;

  const float terreno_estatico[][2] = {
      {0, 100}, {-1, VENTANA_ALTO * 2.0 / 3}, {VENTANA_ANCHO, 100}};

  size_t nt = 3;
  float **terreno = matriz_a_vector(terreno_estatico, nt);
  if (terreno == NULL)
    return NULL;

  // Asignamos la coordenada del medio aleatoriamente
  terreno[1][0] = rand() % VENTANA_ANCHO;

  // Iterativamente densificamos 30 veces achicando el margen cada vez
  for (size_t i = 1; i < 30; i++) {
    float **aux = vector_densificar(terreno, nt, 2 * (i + 5), 100 / i);
    vector_destruir(terreno, nt);
    if (aux == NULL)
      return NULL;
    terreno = aux;
    nt = 2 * (i + 5);
  }

  *n = nt;
  return terreno;
}
