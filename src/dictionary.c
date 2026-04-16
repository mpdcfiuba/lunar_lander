#include "dictionary.h"

#include "caracteres.h"

#include <stdio.h>
#include <stdlib.h>

#define DICTIONARY_SIZE 41

static dictionary_t my_dictionary[] = {{caracter_a,'A',7},
								{caracter_b,'B',12},
								{caracter_c,'C',4},
								{caracter_d,'D',7},
								{caracter_e,'E',7},
								{caracter_f,'F',6},
								{caracter_g,'G',7},
								{caracter_h,'H',6},
								{caracter_i,'I',6},
								{caracter_j,'J',4},
								{caracter_k,'K',6},
								{caracter_l,'L',3},
								{caracter_m,'M',5},
								{caracter_n,'N',4},
								{caracter_o,'O',5},
								{caracter_p,'P',5},
								{caracter_q,'Q',9},
								{caracter_r,'R',7},
								{caracter_s,'S',6},
								{caracter_t,'T',4},
								{caracter_u,'U',4},
								{caracter_v,'V',3},
								{caracter_w,'W',5},
								{caracter_x,'X',5},
								{caracter_y,'Y',5},
								{caracter_z,'Z',4},
								{caracter_0,'0',5},
								{caracter_1,'1',2},
								{caracter_2,'2',6},
								{caracter_3,'3',7},
								{caracter_4,'4',5},
								{caracter_5,'5',6},
								{caracter_6,'6',5},
								{caracter_7,'7',3},
								{caracter_8,'8',7},
								{caracter_9,'9',5},
								{caracter_izquierda,'<',5},
								{caracter_derecha,'>',5},
								{caracter_arriba,'^',5},
								{caracter_abajo,'v',5},
								{caracter_espacio,' ',1}
};

// Busca la posición en el diccionario de la letra y la guarda en dict_position. A la vez guarda el puntero al vector de la letra en letter
void letter_to_func(char ascii,dictionary_t** letter)
{
	for(size_t i = 0; i < DICTIONARY_SIZE; i++)
	{
		if(((my_dictionary[i]).my_ascii) == ascii)
		{
			*letter = (&(my_dictionary[i]));
		}
	}
}
