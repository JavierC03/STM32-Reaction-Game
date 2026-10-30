/*
 * game_fsm.c
 *
 *  Created on: Oct 30, 2026
 *      Author: javier
 */

#include "game_fsm.h"
#include "led.h"
#include "reaction_timer.h"
#include "random_delay.h"
#include "uart_interface.h"

// Almacena el estado actual de la máquina de estados
static GameState estado;
// Almacena el estado actual de la máquina de estados
static uint8_t espera_inicializada;

// Inicializa la máquina de estados
void GameFSM_Init(void){

	// Establecer ESPERA como estado inicial
	estado = ESPERA;
	// Indicar que el estado ESPERA todavía no ha sido inicializado
	espera_inicializada = 0;
}

// Ejecuta un ciclo de la máquina de estados y comprueba las transiciones
void GameFSM_Run(void) {
	// Almacenar el tiempo de espera aleatorio entre llamadas a la función
	static uint8_t espera;
	// Almacenar el tiempo de reacción obtenido durante la partida
	static float tiempo;
	switch (estado) {
		// Estado de espera antes de encender el LED
		case ESPERA:
			// Ejecutar las acciones de entrada al estado una sola vez
			if(espera_inicializada == 0){
				// Reiniciar el contador hardware de TIM3
				__HAL_TIM_SET_COUNTER(&htim3,0);
				// Reiniciar el contador de interrupciones de TIM3
				cont_tim3 = 0;
				// Generar un tiempo de espera aleatorio entre 1 y 5 segundos
				espera = Generate_Number();
				// Arrancar TIM3 para controlar el tiempo de espera
				HAL_TIM_Base_Start_IT(&htim3);
				// Indicar que las acciones de entrada ya se han ejecutado
				espera_inicializada = 1;
			}
			// Comprobar si se ha alcanzado el tiempo de espera aleatorio
			if(espera == cont_tim3){
				// Detener TIM3 al finalizar la espera
				HAL_TIM_Base_Stop_IT(&htim3);
				// Limpiar la flag del botón para ignorar pulsaciones anteriores
				flag_boton = 0;
				// Encender el LED para indicar que el usuario debe reaccionar
				Encender();
				// Iniciar TIM2 para medir el tiempo de reacción
				Arrancar();
				// Preparar la inicialización del estado ESPERA para la siguiente ronda
				espera_inicializada = 0;
				// Cambiar al estado CONTEO
				estado = CONTEO;
			}
		break;
		// Estado de medición del tiempo de reacción
		case CONTEO:
			// Comprobar si se ha detectado una pulsación del botón
			if(flag_boton == 1){
				// Limpiar la flag del botón
				flag_boton = 0;
				// Apagar el LED al detectar la reacción del usuario
				Apagar();
				// Obtener el tiempo de reacción y detener TIM2
				tiempo = Obtener_Cuenta();
				// Cambiar al estado MUESTRA
				estado = MUESTRA;
			}
		break;
		// Estado de visualización del resultado
		case MUESTRA:
			// Enviar el tiempo de reacción al ordenador mediante UART
			Mensaje(tiempo);
			// Volver al estado ESPERA para comenzar una nueva ronda
			estado = ESPERA;
		break;
	}
}
