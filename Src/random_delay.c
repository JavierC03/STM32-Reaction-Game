/*
 * random_delay.c
 *
 *  Created on: Oct 28, 2026
 *      Author: javier
 */
#include "random_delay.h"
#include <stdlib.h>

// Genera un número aleatorio entre 1 y 5 segundos
uint8_t Generate_Number(void){
	// Obtener un número entre 1 y 5
	uint8_t numero = (rand() % 5) + 1;
	return numero;
}
