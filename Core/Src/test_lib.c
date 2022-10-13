/*
 * test_lib.c
 *
 *  Created on: 25 wrz 2022
 *      Author: Adam Polus
 */

#include "test_lib.h"

void update_val(char *tab,  uint16_t sound_probe, int i){
//		  tab[i] = sound_probe & 0xFF;
		  tab[i+1] = sound_probe & 0xFF;
	  tab[i] = sound_probe >> 8;
//	  tab[i+1] = sound_probe >> 8;
//	  tab[i] = 'a';
//	  tab[i+1] = 'a';
}
