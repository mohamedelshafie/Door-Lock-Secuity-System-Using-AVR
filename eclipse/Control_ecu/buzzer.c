/*******************************************************************************
 * Module : Buzzer
 *
 * File name: buzzer.c
 *
 * Description: source file for buzzer driver
 *
 * Author: Mohamed Elshafie
 *******************************************************************************/

#include"buzzer.h"

/******************************************************************************
 *                       Functions definitions                                *
 ******************************************************************************/

/*
 * Description:
 * Initialization function for Buzzer
 */
void buzzer_init(void){
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
}


/*
 * Description:
 * Function to start the buzzer
 */
void buzzer_ON(void){
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_HIGH);
}


/*
 * Description:
 * Function to stop the buzzer
 */
void buzzer_OFF(void){
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}
