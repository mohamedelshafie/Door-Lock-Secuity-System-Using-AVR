/********************************************************************
 * Module: Timer
 *
 * file name: timer.c
 *
 * Description: source file for timer driver
 *
 * Author: Mohamed Elshafie
 ********************************************************************/

#include "timer.h"
#include <avr/io.h> /* To use timers Registers */
#include <avr/interrupt.h> /* For timer ISR */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr_Timer0)(void) = NULL_PTR;

static volatile void (*g_callBackPtr_Timer1)(void) = NULL_PTR;

static volatile void (*g_callBackPtr_Timer2)(void) = NULL_PTR;


/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect){
	if(g_callBackPtr_Timer0 != NULL_PTR){
			/* Call the Call Back function in the application after timer finishes counting in overflow mode */
			(*g_callBackPtr_Timer0)();
		}
}
ISR(TIMER0_COMP_vect){
	if(g_callBackPtr_Timer0 != NULL_PTR){
		/* Call the Call Back function in the application after timer finishes counting in compare mode*/
			(*g_callBackPtr_Timer0)();
		}
}
ISR(TIMER1_OVF_vect){
	if(g_callBackPtr_Timer1 != NULL_PTR){
		/* Call the Call Back function in the application after timer finishes counting in overflow mode */
				(*g_callBackPtr_Timer1)();
			}
}
ISR(TIMER1_COMPA_vect){
	if(g_callBackPtr_Timer1 != NULL_PTR){
		/* Call the Call Back function in the application after timer finishes counting in compare mode*/
				(*g_callBackPtr_Timer1)();
			}
}
ISR(TIMER2_OVF_vect){
	if(g_callBackPtr_Timer2 != NULL_PTR){
		/* Call the Call Back function in the application after timer finishes counting in overflow mode */
				(*g_callBackPtr_Timer2)();
			}
}
ISR(TIMER2_COMP_vect){
	if(g_callBackPtr_Timer2 != NULL_PTR){
		/* Call the Call Back function in the application after timer finishes counting in compare mode*/
				(*g_callBackPtr_Timer2)();
			}
}


/********************************************************************
 *                     Function definitions                         *
 ********************************************************************/

/*
 * Description:
 * Initialization function for the timer module by the following configurations:
 * Timer ID  (timer0, timer1, timer2)
 * Mode of opertion  (Overflow, Compare)
 * initial value of timer
 * compare value for compare mode
 * prescalar
 */
void Timer_init(const Timer_ConfigType * Config_ptr){
	if(Config_ptr->ID ==0){    /*In case using timer 0*/

		/*Insert 0 or 1 in bit 3 in the register to choose the mode*/
		TCCR0 = (TCCR0 & 0xF7) | ((Config_ptr->mode)<<3);

		/*Insert the required prescale in bit 0,1,2 in the register*/
		TCCR0 = (TCCR0 & 0xF8) | (Config_ptr->prescalar);

		/*Insert initial value*/
		TCNT0 = Config_ptr->initial_value;

		/*Check which mode is chosen to enable its interrupt*/
		if(Config_ptr->mode == 0){
			TIMSK |= (1<<TOIE0);         /*Enable Timer0 Overflow Interrupt*/
		}
		else if(Config_ptr->mode == 1){
			OCR0 = Config_ptr->compare_value;      /*set compare value*/
			TIMSK |= (1<<OCIE0);                 /*Enable Timer0 Compare Interrupt*/

		}
	}
	else if(Config_ptr->ID == 1){      /*In case using timer 1*/

		/*the bit is set to 1 in case using non pwm mode */
		TCCR1A = (1<<FOC1A);

		/*Insert 0 or 1 in bit 3 in the register to choose the mode*/
		TCCR1B = (TCCR1B & 0xF7) | ((Config_ptr->mode)<<3);

		/*Insert the required prescale in bit 0,1,2 in the register*/
		TCCR1B = (TCCR1B & 0xF8) | (Config_ptr->prescalar);

		/*Insert initial value*/
		TCNT1 = Config_ptr->initial_value;

		/*Check which mode is chosen to enable its interrupt*/
		if(Config_ptr->mode == 0){
			TIMSK |= (1<<TOIE1);         /*Enable Timer1 Overflow Interrupt*/
		}
		else if(Config_ptr->mode == 1){
			OCR1A = Config_ptr->compare_value;      /*set compare value*/
			TIMSK |= (1<<OCIE1A);                 /*Enable Timer1 Compare Interrupt*/

		}
	}
	else if(Config_ptr->ID == 2){      /*In case using timer 2*/

		/*Insert 0 or 1 in bit 3 in the register to choose the mode*/
		TCCR2 = (TCCR2 & 0xF7) | ((Config_ptr->mode)<<3);

		/*Insert the required prescale in bit 0,1,2 in the register*/
		TCCR2 = (TCCR2 & 0xF8) | (Config_ptr->prescalar);

		/*Insert initial value*/
		TCNT2 = Config_ptr->initial_value;

		/*Check which mode is chosen to enable its interrupt*/
		if(Config_ptr->mode == 0){
			TIMSK |= (1<<TOIE2);         /*Enable Timer2 Overflow Interrupt*/
		}
		else if(Config_ptr->mode == 1){
			OCR2 = Config_ptr->compare_value;      /*set compare value*/
			TIMSK |= (1<<OCIE2);                 /*Enable Timer2 Compare Interrupt*/

		}
	}

}

/*
 * Description: Function to set the Call Back function address of timer 0
 */
void Timer0_setCallBack(void(*a_ptr)(void)){
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr_Timer0 = a_ptr;
}


/*
 * Description: Function to set the Call Back function address of timer 1
 */
void Timer1_setCallBack(void(*a_ptr)(void)){
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr_Timer1 = a_ptr;
}


/*
 * Description: Function to set the Call Back function address of timer 2
 */
void Timer2_setCallBack(void(*a_ptr)(void)){
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr_Timer2 = a_ptr;
}

void Timer_DeInit(uint8 id){
	if(id == 0){
		TCCR0 = 0;
		TCNT0 = 0;
		OCR0 = 0;
		TIMSK &= ~(1<<TOIE0);         /*disable Timer0 Overflow Interrupt*/
		TIMSK &= ~(1<<OCIE0);         /*disable Timer0 Compare Interrupt*/
	}
	else if(id == 1){
			TCCR1A = 0;
			TCCR1B = 0;
			TCNT1 = 0;
			OCR1A = 0;
			TIMSK &= ~(1<<TOIE1);         /*disable Timer1 Overflow Interrupt*/
			TIMSK &= ~(1<<OCIE1A);         /*disable Timer1 Compare Interrupt*/
		}
	else if(id == 2){
		TCCR2 = 0;
		TCNT2 = 0;
		OCR2 = 0;
		TIMSK &= ~(1<<TOIE2);         /*disable Timer2 Overflow Interrupt*/
		TIMSK &= ~(1<<OCIE2);         /*disable Timer2 Compare Interrupt*/
	}
}
