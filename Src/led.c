/*
 * led.c
 *
 *  Created on: Oct 28, 2026
 *      Author: javier
 */

#include "led.h"

// Enciende el LED
void Encender(void){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
}

// Apaga el LED
void Apagar(void){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
}

// Devuelve el estado actual del LED: encendido o apagado
LED_Status Estado(void){

	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_SET){
		return LED_ON;
	}else{
		return LED_OFF;
	}
}

