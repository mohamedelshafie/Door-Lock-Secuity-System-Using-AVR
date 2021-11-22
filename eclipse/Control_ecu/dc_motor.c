 /******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: dc_motor.c
 *
 * Description: Source file for the ATmega16 DC motor driver
 *
 * Author: Mohamed Elshafie
 *
 *******************************************************************************/

#include "dc_motor.h"
#include "gpio.h"
#include "pwm.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for setup the direction for the two motor pins
 * through the GPIO driver and Stop the DC-Motor at the beginning through the GPIO driver
 */
void DcMotor_Init(void){
	/*
	 * set input1 and input2 pins as output pins and initialize by zero (motor is not rotating)
	 */
	GPIO_setupPinDirection(INPUT1_PORT_NUM, INPUT1_PIN_NUM, PIN_OUTPUT);
	GPIO_setupPinDirection(INPUT2_PORT_NUM, INPUT2_PIN_NUM, PIN_OUTPUT);

	/* make motor stop initially*/
	GPIO_writePin(INPUT1_PORT_NUM, INPUT1_PIN_NUM, LOGIC_LOW);
	GPIO_writePin(INPUT2_PORT_NUM, INPUT2_PIN_NUM, LOGIC_LOW);
}

/*
 * Description :
 * Function responsible for rotate the DC Motor CW/ or A-CW or stop the motor based
 * on the state input state value and Send the required duty cycle to the PWM driver based
 * on the required speed value
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed){
	/*
	 * configure input1 & input2 to make motor rotate CW or ACW or to stop according to state
	 */
	GPIO_writePin(INPUT1_PORT_NUM, INPUT1_PIN_NUM, (state>>1));
	GPIO_writePin(INPUT2_PORT_NUM, INPUT2_PIN_NUM, (state & 0x01));
	PWM_Timer0_Start(speed); /* pass the speed to the timer function */
}

