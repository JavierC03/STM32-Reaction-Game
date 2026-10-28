/*
 * led.h
 *
 *  Created on: Oct 28, 2026
 *      Author: javier
 */
#ifndef LED_H
#define LED_H

#include "main.h"

typedef enum{
	LED_OFF,
	LED_ON
}LED_Status;

void Encender(void); // Turn on LED
void Apagar(void); // Turn off LED
LED_Status Estado(void); // Check led status

#endif /* LED_H */
