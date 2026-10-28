/*
 * uart_interface.c
 *
 *  Created on: Oct 28, 2026
 *      Author: javier
 */

#include "uart_interface.h"
#include <stdio.h>

void Mensaje(float tiempo){
	printf("Has Tardado: %f segundos\r\n",tiempo);
}

int _write(int file, char *ptr, int len){
	HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, HAL_MAX_DELAY);
	return len;
}

