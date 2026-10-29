/*
 * reaction_timer.c
 *
 *  Created on: Oct 29, 2026
 *      Author: javier
 */
#include "reaction_timer.h"

// Declaración externa del temporizador TIM2, definido en main.c
extern TIM_HandleTypeDef htim2;

// Inicia la medición del tiempo de reacción
void Arrancar(void){
	// Poner el contador de TIM2 a cero
	__HAL_TIM_SET_COUNTER(&htim2,0);
	// Reiniciar el número de interrupciones contabilizadas
	cont_tim2 = 0;
	// Arrancar TIM2 para medir el tiempo
	HAL_TIM_Base_Start_IT(&htim2);
}

float Obtener_Cuenta(void){
	float tiempo;
	// Detener el temporizador
	HAL_TIM_Base_Stop_IT(&htim2);
	// Leer el valor actual del contador
	uint32_t cuenta =  __HAL_TIM_GET_COUNTER(&htim2);
	// Calcular el tiempo total:
	// Cada interrupción suma 0,5 segundos.
	// Cada incremento del contador suma 0,0005 segundos
	tiempo = (cont_tim2 * 0.5) + (cuenta * 0.0005);
	return tiempo;
}

