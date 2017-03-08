/****************************************************************************
 *   $Id:: PWM16_32test.c 3635 2010-06-02 00:31:46Z usb00423                $
 *   Project: NXP LPC11xx 16-bit/32-bit PWM example
 *
 *   Description:
 *     This file contains PWM test modules, main entry, to test PWM APIs.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/

#include "driver_config.h"
#include "target_config.h"

#include "timer32.h"
#include "timer16.h"
#include "gpio.h"

#define CONFIG_ENABLE_DRIVER_SMALLGPIO 1


#include "small_gpio.h"
extern volatile uint32_t timer32_0_counter;

volatile uint32_t period = 1000;  //100  48Khz PWM frequency
                                //200  2x48Khz PWM frequency


/* Main Program */

int main (void) {
	/* Initialize 32-bits timer 0 */
	//init_timer32(0, TIME_INTERVAL);
	//enable_timer32(0);
	
	/* Initialize the PWM in timer32_1 enabling match0 output */
	init_timer32PWM(1, period, MATCH0);
	setMatch_timer32PWM (1, 0, period*9/10);
	enable_timer32(1);

	/* Initialize the PWM in timer16_1 enabling match1 output */
	//init_timer16PWM(1, period, MATCH1, 0);
	//setMatch_timer16PWM (1, 1, period*3/4);
	init_timer32PWM(0, period, MATCH1);
	setMatch_timer32PWM (0, 1, period*9/10);
	enable_timer32(0);



	/* Enable AHB clock to the GPIO domain. */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);
	
	/* Set port 2_0 to output */
	GPIOSetDir( 0, 7, 1 );
	
	GPIOSetDir( 1, 5, 1 );
	GPIOSetDir( 0, 6, 1 );
	GPIOSetDir( 2, 1, 1 );
	GPIOSetDir( 1, 8, 1 );
	GPIOSetDir( 2, 2, 1 );


	  //STBY to '1'
	  GPIOSetValue( 1, 5, 1 );

	  //BIN2 AIN2 to '1'
	  GPIOSetValue( 0, 6, 1 );
	  GPIOSetValue( 2, 1, 1 );

	  //BIN1 AIN1 to '0'
	  GPIOSetValue( 1, 8, 0 );
	  GPIOSetValue( 2, 2, 0 );



	  GPIOSetDir( 3, 2, 1 );
	  GPIOSetDir( 1, 9, 1 );
	  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);
	  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<8);
	  int val;
	while (1)                              /* Loop forever */
	{
		GPIOSetDir(   3, 2, 1 ); //DIR M0 TO sensor
		GPIOSetValue( 1, 9, 0 ); //DIR M0 TO sensor


		  GPIOSetValue( 3, 2, 0 );// Send low pulse
		  delayMs(0,2);   // Wait for 2 microseconds
		  GPIOSetValue( 3, 2, 1 );// Send high pulse
		  delayMs(0,10); // Wait for 5 microseconds
		  GPIOSetValue( 3, 2, 0 );// Send low pulse

		  GPIOSetDir(  3, 2, 0 ); //Send low pulse
		  GPIOSetValue(1,9, 1 );  //DIR sensor TO M0

		  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);
		  LPC_IOCON->PIO0_2           &= ~0x07;	/*  Timer0_16 I/O config */
		  LPC_IOCON->PIO0_2           |= 0x02;		/* Timer0_16 CAP0 */
		  LPC_TMR16B0->TCR = 0x02;		/* reset timer */
		  LPC_TMR16B0->PR  = MHZ_PRESCALE; /* set prescaler to get 1 M counts/sec */
		  LPC_TMR16B0->MR0 = TIME_INTERVALmS * 50; /* Set up 10 mS interval */
		  /* Capture 0 on rising edge, interrupt enable. */
		  LPC_TMR16B0->CCR = 0x02;
		  LPC_TMR16B0->TCR = 0x01;		/* start timer */

		  /* wait until delay time has elapsed */
		  while (LPC_TMR16B0->TCR & 0x01);
		  val = LPC_TMR16B0->CR0;

		  int i;
		  for ( i=0; i<20;i++)
		    delayMs(0,1000); // Wait for 5 microseconds

	}
}
