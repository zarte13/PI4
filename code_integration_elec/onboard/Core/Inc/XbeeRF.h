// Auteur : Émile Mayer
// Date de création 20 mars
// Date de modfication 20 mars
// PI4


//Macros
// xbee.h
#ifndef XBEE_H
#define XBEE_H

#include "main.h"
#include "gpio.h"
#include "usart.h"
#include <stdint.h>
#include <stdbool.h>

#define XBEE_RF_UART_HANDLE       huart1
#define XBEE_RESET_PORT GPIOC
#define XBEE_RESET_PIN  GPIO_PIN_10  // XBEE_N_RESET → PC10
#define XBEE_RF_UART_DMA_SIZE 20
#define XBEE_RF_BAUD_RATE 9600

// Taille des tempons de lecture et d'écriture
#define XBEE_RF_TX_BUFFER_LENGHT 100
#define XBEE_RF_RX_BUFFER_LENGTH 100

typedef struct{
	uint32_t start_char;
	float pression;
	uint8_t valves;
	uint32_t end_char;
}Xbee_Message;

typedef struct{
	uint32_t start_receive;
	uint32_t message_receive_valve1;
	uint32_t message_receive_valve2;
	uint32_t fin_receive;
}Xbee_message_receive;

extern Xbee_Message xbee_msg;

void XBeeRF_Init(void);
void XBeeRF_Task(void);


#endif
