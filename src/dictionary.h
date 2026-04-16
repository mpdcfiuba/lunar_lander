#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

#include <stdlib.h>

typedef const int (*character_t)[2];

typedef struct
{
	character_t my_character;
	const char my_ascii;
	size_t my_size;
}
dictionary_t;

void letter_to_func(char ascii,dictionary_t** letter);

#endif
