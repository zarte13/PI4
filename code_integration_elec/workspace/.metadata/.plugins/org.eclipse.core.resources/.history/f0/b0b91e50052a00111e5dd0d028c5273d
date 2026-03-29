// Auteur : Émile Mayer
// Date de création 20 mars
// Date de modfication 20 mars
// PI4

#include "main.h"
#include "gpio.h"
#include "usart.h"
//Macros
// xbee.h
#ifndef XBEE_H
#define XBEE_H

#define XBEE_UART       &huart1
#define XBEE_RESET_PORT GPIOC
#define XBEE_RESET_PIN  GPIO_PIN_10  // XBEE_N_RESET → PC10

void XBee_Reset(void);
void XBee_Send(uint8_t *data, uint16_t len);

#endif
