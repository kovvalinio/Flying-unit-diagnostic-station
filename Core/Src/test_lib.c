/*
 * test_lib.c
 *
 *  Created on: 25 wrz 2022
 *      Author: Adam Polus
 */

#include "test_lib.h"

void update_val(uint8_t tab[9], uint8_t V, uint16_t X, uint16_t Y, uint16_t Z, uint16_t sound_probe){

	  tab[0] = 1;
	  tab[1] = X >> 8;
	  tab[2] = X & 0xFF;
	  tab[3] = Y >> 8;
	  tab[4] = Y & 0xFF;
	  tab[5] = Z >> 8;
	  tab[6] = Z & 0xFF;
	  tab[7] = sound_probe >> 8;
	  tab[8] = sound_probe & 0xFF;
}
