/**********************************************************************
 * File Name: Control_main.c
 * Author: Mohamed Elshafie
 * Date Created: Oct 25, 2021
 * Description: Control ECU code in Door lock security system project
 **********************************************************************/

#include <avr/io.h> /* to access SREG */
#include "external_eeprom.h"
#include "dc_motor.h"
#include "buzzer.h"
#include "std_types.h"
#include "twi.h"
#include "uart.h"
#include "util/delay.h"
#include "timer.h"

uint8 pass[5]={0};        /* to store entered password */
uint8 pass_reenter[5]={0};    /* to store re-entered password */

uint8 flag=0;         /*flag to constantly ask user to create password until match occurs*/
uint8 i;       /*counter for the for loops*/
uint8 check=0;    /* variable to store data read from EEPROM */

uint8 count=0;   /*counter to count number of the 3 mismatches*/
uint8 g_option;     /*variable to choose whether open door, change password or giva an error*/

uint8 g_key;      /* variable to get value of pressed key in the keypad */


uint16 g_tick = 0;   /*counter for the first callback function of the timer*/
uint8 g_timer_flag = 1;   /*flag to stop everything until timer finishes*/
uint16 g_tick2 = 0;   /*counter for the second callback function of the timer*/
uint8 g_mismatch_flag =0;     /*flag to indicate whether password entered at first is right or wrong*/

void timer_function(void){   /*the first callback function for the timer*/

	g_tick++;       /*increment counter each interrupt*/

	/*
	 * at the first 15 seconds (from first interrupt to int. no. 468) rotate motor Clock-wise
	 * at the next 3 seconds (from int. no. 468 to int. no. 562) stop the motor
	 * at the next 15 seconds (from int. no. 562 to int. no. 1031) rotate motor Anti Clock-wise
	 * at the end (when we reach int. no. 1031) stop the motor and de-initialize the timer and clear flags
	 */
	if(g_tick == 1){
		DcMotor_Rotate(CW, 100);
	}
	else if(g_tick == 468){
		DcMotor_Rotate(STOP, 0);
	}
	else if(g_tick == 562){
		DcMotor_Rotate(A_CW, 100);
	}
	else if(g_tick == 1031){
		DcMotor_Rotate(STOP, 0);
		g_tick = 0;
		g_timer_flag = 0;
		Timer_DeInit(2);
	}
}

void timer_function2(void){    /*the second callback function for the timer*/

	g_tick2++;    /*increment counter each interrupt*/

	/*
	 * at the first minute (from first interrupt to int. no. 1875) open the buzzer
	 * at the end of the minute (at int. no. 1875) turn off the buzzer and de-initialize the timer and clear flags
	 */
	if(g_tick2 == 1){
		buzzer_ON();
	}
	else if(g_tick2 == 1875){
		buzzer_OFF();
		g_tick2 = 0;
		Timer_DeInit(2);
	}
}
void setting_password2(void){           /*function to set new password*/

	for(i=0;i<5;i++){           /*receive first input*/
			pass[i] = UART_recieveByte();
		}

		for(i=0;i<5;i++){        /*receive second input*/
			pass_reenter[i] = UART_recieveByte();
		}

		for(i=0;i<5;i++){       /* compare two inputs*/
			if(pass[i]!=pass_reenter[i]){
				UART_sendByte(0x31);      /* send flag to indicate that the two inputs doesn't matches */

				flag=1;
				break;
			}
			else{
				flag=0;
			}
		}
		if(flag==0){
		UART_sendByte(0x30);  /* send flag to indicate that the two inputs matches */
		}

}
void check_password2(void){         /* function to compare the input to the stored data in the EEPROM */
	do{

	for(i=0;i<5;i++){    /*receive input*/
		pass[i] = UART_recieveByte();

	}

	for(i=0;i<5;i++){     /* read data from EEPROM and compare it with input */

		EEPROM_readByte(i, &check);


		if(pass[i] != check){

			g_mismatch_flag=0;
			count++;
			break;
		}
		else{

			g_mismatch_flag=1;
		}
	}
	UART_sendByte(count);         /* receive count flag */
	UART_sendByte(g_mismatch_flag);   /* send g_mismatch_flag */

	}while(count<3 && g_mismatch_flag == 0);     /*repeat for maximum 3 times or if input was right before 3rd time*/
}

int main(void){
	SREG |=(1<<7);    /*enable global interrupts*/

	/*
	 * configure and initialize UART
	 * work with 8 bits data
	 * no parity bit
	 * one bit for stop bit
	 * baud rate = 9600
	*/
	UART_ConfigType myConfig ={eight_bits,No_parity,one_bit,9600};
	UART_init(&myConfig);

	buzzer_init();  /*initialize buzzer*/

	DcMotor_Init();   /* initialize motor*/

	/*
	 * configure and initialize I2C(TWI)
	 * set bit rate and the address
	*/
	TWI_Config myConfig_TWI = {0x02,0x01};
	TWI_init(&myConfig_TWI);

	/*
	 * configuration for the timer:
	 * using timer2
	 * prescalar: 1024
	 * mode: Overflow
	 * initial value: 0
	 * compare value (in case using compare mode): 0
	 */
	Timer_ConfigType my_timer = {timer2,Overflow,timer2_1024,0,0};

	do{
		setting_password2();    /* setting password for first time */
	}while(flag==1);
	for(i=0;i<5;i++){
		EEPROM_writeByte(i, pass[i]);    /*store the password in EEPROM*/
		_delay_ms(10);   /*write time delay for EEPROM*/
	}
	while(1){
		g_key = UART_recieveByte();    /*receive chosen option*/

		if(g_key == '+'){
			check_password2();       /*first ask for the password and check it*/
			if(count < 3){
				UART_sendByte('D');    /*secondly send the action to be taken which is open door */

				Timer_init(&my_timer);   /*start timer*/
				Timer2_setCallBack(timer_function);  /*set the callback function*/
			}
			else{
				UART_sendByte('E');     /*or send the action to be taken which is error */
				Timer_init(&my_timer);   /*start timer*/
				Timer2_setCallBack(timer_function2);   /*set the callback function*/
			}
		}
		else if(g_key == '-'){
			check_password2();      /*first ask for the password and check it*/
			if(count < 3){
				UART_sendByte('C');     /*secondly send the action to be taken which is change password */
				do{

					setting_password2();    /*set new password*/
					}while(flag==1);
					for(i=0;i<5;i++){
						EEPROM_writeByte(i, pass[i]);    /*store new password*/
						_delay_ms(10);
					}

			}
			else{
				UART_sendByte('E');     /*or send the action to be taken which is error */
				Timer_init(&my_timer);     /*start timer*/
				Timer2_setCallBack(timer_function2);   /*set the callback function*/
			}


		}
	}
}
