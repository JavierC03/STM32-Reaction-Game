/*
 * callbacks.c
 *
 *  Created on: Oct 29, 2026
 *      Author: javier
 */
#include "main.h"

// Guarda el instante de la última pulsación aceptada
static uint32_t ultima_pulsacion = 0;

// Se ejecuta cuando se detecta una interrupción del botón
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	// Comprobar que la interrupción corresponde al pin del botón
	if(GPIO_Pin == GPIO_PIN_13){
		// Obtener el tiempo actual en milisegundos
		uint32_t ahora = HAL_GetTick();
		// Aceptar la pulsación solo si han pasado al menos 50 ms
		if((ahora - ultima_pulsacion)>=50){
			// Avisar al programa principal de que se ha pulsado el botón
			flag_boton = 1;
			// Guardar el instante de esta pulsación
			ultima_pulsacion = ahora;
		}

	}
}

// Se ejecuta cuando TIM2 o TIM3 generan una interrupción
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	// Contabilizar las interrupciones de TIM2 para medir el tiempo de reacción
	if(htim->Instance == TIM2){
		cont_tim2++;
	}
	// Contabilizar las interrupciones de TIM3 para controlar la espera aleatoria
	if(htim->Instance == TIM3){
		cont_tim3++;
		}
}
