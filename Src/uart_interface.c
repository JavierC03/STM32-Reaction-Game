/*
 * uart_interface.c
 *
 *  Created on: Oct 28, 2026
 *      Author: javier
 */

#include "uart_interface.h"
#include <stdio.h>

// Muestra por UART el tiempo de reacción en segundos
void Mensaje(float tiempo){
	printf("Has Tardado: %f segundos\r\n",tiempo);
}

// Permite que printf envíe sus datos a través de la UART
int _write(int file, char *ptr, int len){
	// Transmitir los caracteres por USART2
	HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, HAL_MAX_DELAY);
	// Devolver el número de caracteres transmitidos
	return len;
}
