/*
 * game_fsm.h
 *
 *  Created on: Oct 30, 2026
 *      Author: javier
 */

#ifndef GAME_FSM_H_
#define GAME_FSM_H_

#include "main.h"

// Estados posibles de la máquina de estados del juego
typedef enum{
	ESPERA, // Espera un intervalo aleatorio antes de encender el LED
	CONTEO, // Mide el tiempo de reacción hasta detectar la pulsación
	MUESTRA // Muestra el tiempo de reacción mediante UART
}GameState;

void GameFSM_Init(void);
void GameFSM_Run(void);

#endif /* GAME_FSM_H_ */
