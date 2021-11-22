 /******************************************************************************
 *
 * Module: PWM mode in Timer module
 *
 * File Name: pwm.h
 *
 * Description: header file for the ATmega16 PWM mode in Timer module driver
 *
 * Author: Mohamed Elshafie
 *
 *******************************************************************************/

#ifndef PWM_H_
#define PWM_H_

#include "std_types.h"



/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for start timer 0 in PWM mode.
 */

void PWM_Timer0_Start(uint8 duty_cycle);



#endif /* PWM_H_ */
