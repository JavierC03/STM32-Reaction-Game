/*
 * led.h
 *
 *  Created on: Oct 28, 2026
 *      Author: javier
 */
#ifndef LED_H
#define LED_H

#include "main.h"

// Estados posibles del LED
typedef enum{
	LED_OFF, // LED apagado
	LED_ON // LED encendido
}LED_Status;

void Encender(void); // Turn on LED
void Apagar(void); // Turn off LED
LED_Status Estado(void); // Check led status

#endif /* LED_H */
