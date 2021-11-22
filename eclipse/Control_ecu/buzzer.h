/*******************************************************************************
 * Module : Buzzer
 *
 * File name: buzzer.c
 *
 * Description: header file for buzzer driver
 *
 * Author: Mohamed Elshafie
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"

/**************************************************************************
 *                       Definitions                                      *
 **************************************************************************/
#define BUZZER_PORT_ID            PORTC_ID

#define BUZZER_PIN_ID             PIN7_ID

/**************************************************************************
 *                       Functions prototypes                             *
 **************************************************************************/

/*
 * Description:
 * Initialization function for Buzzer
 */
void buzzer_init(void);


/*
 * Description:
 * Function to start the buzzer
 */
void buzzer_ON(void);


/*
 * Description:
 * Function to stop the buzzer
 */
void buzzer_OFF(void);


#endif /* BUZZER_H_ */
