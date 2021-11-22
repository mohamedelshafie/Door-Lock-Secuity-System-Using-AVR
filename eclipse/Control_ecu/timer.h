/********************************************************************
 * Module: Timer
 *
 * file name: timer.c
 *
 * Description: header file for timer driver
 *
 * Author: Mohamed Elshafie
 ********************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"

/*******************************************************************
 *                          Types Declaration                      *
 *******************************************************************/

typedef enum{
	timer0,timer1,timer2
}timer_Id;

typedef enum{
	Overflow,Compare
}timer_mode;

typedef enum{
	No_clock,No_prescale,timer0_1_2_8,timer0_1_64,timer2_32=3,timer0_1_256,timer2_64=4,timer0_1_1024,timer2_128=5,\
	timer0_1_Ext_falling,timer2_256=6,timer0_1_Ext_rising,timer2_1024=7
}timer_prescalar;

typedef struct{
	timer_Id ID;
	timer_mode mode;
	timer_prescalar prescalar;
	uint16 initial_value;
	uint16 compare_value;
}Timer_ConfigType;

/*********************************************************************
 *                     functions prototypes                          *
 *********************************************************************/

/*
 * Description:
 * Initialization function for the timer module by the following configurations:
 * Timer ID
 * Mode of opertion
 * initial value of timer
 * compare value for compare mode
 * prescalar
 */
void Timer_init(const Timer_ConfigType * Config_ptr);


/*
 * Description: Function to set the Call Back function address of timer 0
 */
void Timer0_setCallBack(void(*a_ptr)(void));


/*
 * Description: Function to set the Call Back function address of timer 1
 */
void Timer1_setCallBack(void(*a_ptr)(void));


/*
 * Description: Function to set the Call Back function address of timer 2
 */
void Timer2_setCallBack(void(*a_ptr)(void));


#endif /* TIMER_H_ */
