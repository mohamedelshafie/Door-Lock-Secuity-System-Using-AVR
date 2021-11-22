 /******************************************************************************
 *
 * Module: PWM mode in Timer module
 *
 * File Name: pwm.c
 *
 * Description: Source file for the ATmega16 PWM mode in Timer module driver
 *
 * Author: Mohamed Elshafie
 *
 *******************************************************************************/

#include "avr/io.h" /* To use the timer 0 Registers */
#include "pwm.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


/*
 * Description :
 * Function responsible for start timer 0 in PWM mode.
 */
void PWM_Timer0_Start(uint8 duty_cycle){

	TCNT0 = 0;            /*Set Timer Initial value*/
	OCR0 = ((duty_cycle*255)/100);
	DDRB  = DDRB | (1<<PB3); /*set PB3/OC0 as output pin*/

	/* Configure timer control register
	 *  Fast PWM Mode WGM01=1 & WGM00=1
	 *  Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 *  clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */

	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}
