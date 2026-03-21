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
#define ADC_INIT_MESSAGE 0100001010000000b
/*
 * [15] : 0
 * [14:12] : Canal de lecture, 100 est la mesure entre AIN0 et GND (diviseur de tension)
 * [11:9] : FSR, 001 donne un FSR de ±4.096V
 * [8] : 0
 * [7:5] : 100
 * [4] : 0
 * [3] : 0
 * [2] : 0
 * [1:0] : 00
 */

int ADS1015_init(void);
void ADS1015_routine(void);



#endif /* __ASD1015_H__*/
