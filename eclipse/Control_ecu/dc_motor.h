 /******************************************************************************
 *
 * Module: DC motor
 *
 * File Name: dc_motor.h
 *
 * Description: header file for the ATmega16 DC Motor driver
 *
 * Author: Mohamed Elshafie
 *
 *******************************************************************************/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define INPUT1_PORT_NUM          PORTB_ID
#define INPUT2_PORT_NUM          PORTB_ID
#define ENABLE_PORT_NUM           PORTB_ID
#define INPUT1_PIN_NUM           PIN0_ID
#define INPUT2_PIN_NUM           PIN1_ID
#define ENABLE_PIN_NUM           PIN3_ID

typedef enum{
	STOP,A_CW,CW
}DcMotor_State;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for setup the direction for the two motor pins
 * through the GPIO driver and Stop the DC-Motor at the beginning through the GPIO driver
 */
void DcMotor_Init(void);

/*
 * Description :
 * Function responsible for rotate the DC Motor CW/ or A-CW or stop the motor based
 * on the state input state value and Send the required duty cycle to the PWM driver based
 * on the required speed value
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed);


#endif /* DC_MOTOR_H_ */
