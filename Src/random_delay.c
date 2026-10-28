/*
 * random_delay.c
 *
 *  Created on: Oct 28, 2026
 *      Author: javier
 */
#include "random_delay.h"
#include <stdlib.h>

int Generate_Number(void){
	int numero = (rand() % 5) + 1;
	return numero;
}
