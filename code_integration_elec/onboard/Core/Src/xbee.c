// Auteur : Émile Mayer
// Date de création 20 mars
// Date de modfication 20 mars
// PI4

#include "xbee.h"
#include "usart.h"




void XBEE_INIT(){
	info.key = 0xFEEDCAFE;
    HAL_GPIO_WritePin(XBEE_N_RESET_GPIO_Port,XBEE_N_RESET_Pin,GPIO_PIN_SET);
}

void XBEE_send_alive(){

}
void XBEE_send_pressure(){

}
void XBEE_send_valves_status(){

}
