/**********************************************************************
 * File Name: HMI_main.c
 * Author: Mohamed Elshafie
 * Date Created: Oct 24, 2021
 * Description: Human Machine Interface(HMI) ECU code in Door lock security system project
 **********************************************************************/

#include "std_types.h"
#include <avr/io.h> /* to access SREG */
#include <util/delay.h>
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer.h"

uint8 g_key;     /* variable to get value of pressed key in the keypad */
uint8 pass[5]={0}; /* to store entered password */
uint8 pass_reenter[5]={0};        /* to store re-entered password */

uint8 i=0;   /*counter for the for loops*/
uint8 g_option;    /*variable to choose whether open door, change password or give an error*/
uint16 g_tick = 0;  /*counter for the first callback function of the timer*/
uint8 count = 0;     /*counter to count number of the 3 mismatches*/

uint8 flag = 0;     /*flag to constantly ask user to create password until match occurs*/
uint8 g_timer_flag = 1;     /*flag to stop everything until timer finishes*/
uint8 g_mismatch_flag = 0;    /*flag to indicate whether password entered at first is right or wrong*/
uint16 g_tick2 = 0;        /*counter for the second callback function of the timer*/

void timer_function(void){     /*the first callback function for the timer*/
	g_tick++;  /*increment counter each interrupt*/
	/*
	 * at the first 15 seconds (from first interrupt to int. no. 468) print that door is opening
	 * at the next 3 seconds (from int. no. 468 to int. no. 562) print that door is open
	 * at the next 15 seconds (from int. no. 562 to int. no. 1031) print that door is closing
	 * at the end (when we reach int. no. 1031) clear screen and de-initialize the timer and clear flags
	 */

	if(g_tick == 1){
		LCD_displayString("Door is opening");
	}
	else if(g_tick == 468){
		LCD_clearScreen();
		LCD_displayString("door is open");
	}
	else if(g_tick == 562){
		LCD_clearScreen();
		LCD_displayString("Door is closing");
	}
	else if(g_tick == 1031){
		LCD_clearScreen();
		g_tick = 0;
		g_timer_flag = 0;
		Timer_DeInit(0);
	}
}
void timer_function2(void){           /*the second callback function for the timer*/

	g_tick2++;      /*increment counter each interrupt*/

	/*
	 * at the first minute (from first interrupt to int. no. 1875) print wrong!!!
	 * at the end of the minute (at int. no. 1875) clear screen and de-initialize the timer and clear flags
	 */
	if(g_tick2 == 1){
		LCD_displayString("wrong!!!");
	}
	else if(g_tick2 == 1875){
		LCD_clearScreen();
		g_tick2 = 0;
		g_timer_flag = 0;
		Timer_DeInit(0);
	}
}
void setting_password(void){           /*function to set new password*/
	/*set cursor position and print and set cursor to new line*/
	LCD_moveCursor(0, 0);
	LCD_displayString("Enter Password");
	LCD_moveCursor(1, 0);

	/* get the key and send it by UART */
	for(i=0;i<5;i++){
		pass[i] = KEYPAD_getPressedKey();
		UART_sendByte(pass[i]);
		LCD_displayCharacter('*');
		_delay_ms(500);
	}
	g_key = KEYPAD_getPressedKey();  /*enter key to get to the next stage*/

	_delay_ms(500);

	LCD_clearScreen();

	/*repeat the same steps*/
	LCD_moveCursor(0, 0);
	LCD_displayString("ReEnter Password");
	LCD_moveCursor(1, 0);

	for(i=0;i<5;i++){
		pass_reenter[i] = KEYPAD_getPressedKey();
		UART_sendByte(pass_reenter[i]);
		LCD_displayCharacter('*');
		_delay_ms(500);
	}
	g_key = KEYPAD_getPressedKey();  /*enter key to get to the next stage*/
	_delay_ms(500);

	LCD_clearScreen();
	flag = UART_recieveByte();   /* receive flag to check whether two inputs match or not */
}
void check_password(void){                /* function to compare the input to the stored data in the EEPROM */
	do{
		/*set cursor position and print and set cursor to new line*/
		LCD_moveCursor(0, 0);
		LCD_displayString("Enter Password");
		LCD_moveCursor(1, 0);

		/* get the key and send it by UART */
		for(i=0;i<5;i++){
			pass[i] = KEYPAD_getPressedKey();

			LCD_displayCharacter('*');
			_delay_ms(500);
		}
		g_key = KEYPAD_getPressedKey();  /*enter key to get to the next stage*/

		_delay_ms(500);
		for(i=0;i<5;i++){
			UART_sendByte(pass[i]);
		}
		LCD_clearScreen();

		count = UART_recieveByte();    /* receive count flag */
		g_mismatch_flag =UART_recieveByte();   /* receive g_mismatch_flag */

	}while(count<3 && g_mismatch_flag == 0);   /*repeat for maximum 3 times or if input was right before 3rd time*/

}
int main(void){
	SREG |=(1<<7);    /*enable global interrupts*/

	/*
	 * configuration for the timer:
	 * using timer0
	 * prescalar: 1024
	 * mode: Overflow
	 * initial value: 0
	 * compare value (in case using compare mode): 0
	 */
	Timer_ConfigType my_timer = {timer0,Overflow,timer0_1_1024,0,0};

	/*
	 * configure and initialize UART
	 * work with 8 bits data
	 * no parity bit
	 * one bit for stop bit
	 * baud rate = 9600
	 */
	UART_ConfigType myConfig ={eight_bits,No_parity,one_bit,9600};
	UART_init(&myConfig);

	LCD_init();  /*initialize LCD*/

	do{
		setting_password();     /*set password for the first time*/
	}while(flag == 0x31);

	while(1){

		/*print main menu*/
		LCD_moveCursor(0, 0);
		LCD_displayString("+: Open Door");
		LCD_moveCursor(1, 0);
		LCD_displayString("-: change pass");
		LCD_moveCursor(2, 0);

		g_key = KEYPAD_getPressedKey();
		UART_sendByte(g_key);    /*send chosen option*/
		_delay_ms(500);

		LCD_clearScreen();
		if(g_key == '+'){
			check_password();   /*first ask for the password*/

		}
		else if(g_key == '-'){
			check_password();    /*first ask for the password*/
		}
		LCD_clearScreen();


		g_option = UART_recieveByte();   /*receive the action to be taken whether open door, change password or print error*/
		if(g_option == 'D'){    /* secondly start timer and set the callback function */

			Timer_init(&my_timer);
			Timer0_setCallBack(timer_function);

			while(g_timer_flag==1);   /*polling for the timer to stop everything until timer finishes*/
			g_timer_flag=1;   /*set the flag back again*/
		}
		else if(g_option == 'E'){  /* secondly start timer and set the callback function */

			Timer0_setCallBack(timer_function2);
			Timer_init(&my_timer);

			while(g_timer_flag==1);   /*polling for the timer to stop everything until timer finishes*/
			g_timer_flag=1;           /*set the flag back again*/
		}
		else if(g_option == 'C'){
			do{

				setting_password();    /*create new password*/
			}while(flag == 0x31);
		}

	}
}
