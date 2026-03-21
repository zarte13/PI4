// Auteur : Émile Mayer
// Date de création 20 mars
// Date de modfication 20 mars
// PI4


#ifndef __ADS1015_H__
#define __ADS1015_H__

#include "main.h"
#include "dma.h"
#include "i2c.h"

//Macro values defines
#define ADC_ADDRESS 0b1001000
#define ADC_ADDR_CONV_REG 0b0000000
#define ADC_ADDR_CONFIG_REG 0b00000001
#define ADC_INIT_MESSAGE 0x4280
/*
 * [15] : OS, 0
 * [14:12] : Canal de lecture, 100 est la mesure entre AIN0 et GND (diviseur de tension)
 * [11:9] : FSR, 001 donne un FSR de ±4.096V
 * [8] : MODE, 0 -> continuous conversion mode
 * [7:5] : 100 -> data rate, garde par défaut
 * [4] : 0 -> COMP_MODE
 * [3] : 0 -> COMP_POL
 * [2] : 0 -> COMP_LAT
 * [1:0] : 00 -> COMP_QUE
 */
#define ADC_INIT_MESSAGE 0x4280



int ADS1015_init(void);
void ADS1015_get_sample(void);



#endif /* __ASD1015_H__*/
