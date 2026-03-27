// Auteur : Émile Mayer
// Date de création 20 mars
// Date de modfication 20 mars
// PI4

#include "main.h"
#include "gpio.h"
#include "usart.h"
//Macros
#define XBEE_RF_UART_HANDLE huart1 // must be the same as in usart.c
#define XBEE_RF_UART_RATE 230400 // only information see usart.c to check if the rate is correct
#define XBEE_RF_UART_DMA_SIZE 20

typedef struct{
	uint32_t key;
	uint8_t pressure_psi[4];
	uint8_t valve1_status;
	uint8_t valve2_status;
}Info;

static Info info;

//Prototypes
void XBEE_INIT(void);
void XBEE_send_pressure(void);
void XBEE_send_valves_status(void);
