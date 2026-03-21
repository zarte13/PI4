// Auteur : Émile Mayer
// Date de création 20 mars
// Date de modfication 20 mars
// PI4

#include "ads1015.h"

int ADS1015_init(){
	//todo : écriture dans les registres de config pour les bonnes configurations
	//todo : s'assurer qu'il n'y ait pas de problème avec le HAL
	//todo : confirmer le fonctionnement du transfert I2C avec le DMA
	uint8_t tx_buffer[2];
	tx_buffer[0] = (uint8_t)(ADC_INIT_MESSAGE >> 8);
	tx_buffer[1] = (uint8_t)(ADC_INIT_MESSAGE & 0xFF);
	if(HAL_I2C_Master_Transmit_DMA(&hi2c1,ADC_ADDRESS << 1,tx_buffer, 2)!= HAL_OK){
		return -1;
	}
	return 0;
}

void ADS1015_get_sample(){
	//todo : appeler cette fonction lors d'un interrupt de l'ADC (signifie conversion faite)
	//todo : écrire dans un buffer l'information (utiliser le DMA)
	//todo : écrire la valeur numérique de la pression
}
