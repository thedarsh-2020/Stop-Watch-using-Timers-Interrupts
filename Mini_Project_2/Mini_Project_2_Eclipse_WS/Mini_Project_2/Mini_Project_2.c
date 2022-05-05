/**************************************************************************************************
 Name        : Mini Project 2
 Author      : Mostafa M A. Mottilib
 Description : Stop Watch Project
 **************************************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#include "DATA_TYPES_INIT.h"
#include "BITWISE_OPERATIONS.h"


/* Stop-Watch counter definition */
#define INTIAL_VALUE 0
#define TRUE 1
#define COUNT_ONES 9
#define COUNT_TENS 5


/* Stop-Watch Global Variables */
uint8 SEC_1 = INTIAL_VALUE;
uint8 SEC_1_FLAG = INTIAL_VALUE;

uint8 SEC_2 = INTIAL_VALUE;

uint8 MIN_1 = INTIAL_VALUE;
uint8 MIN_2 = INTIAL_VALUE;

uint8 HOUR_1 = INTIAL_VALUE;
uint8 HOUR_1_FLAG = INTIAL_VALUE;

uint8 HOUR_2 = INTIAL_VALUE;


/* External Interrupt INT0 to reset Stop Watch */
ISR(INT0_vect)
{
	SEC_1 = INTIAL_VALUE;
	SEC_2 = INTIAL_VALUE;
	MIN_1 = INTIAL_VALUE;
	MIN_2 = INTIAL_VALUE;
	HOUR_1 = INTIAL_VALUE;
	HOUR_2 = INTIAL_VALUE;
}

/* External Interrupt INT1 to Stop count */
ISR(INT1_vect)
{
	CLEAR_BIT(TIMSK, OCIE1A);			/* Timer1 CTC Interrupt Disable to stop count using External Interrupt INT1 using Bush Button*/
}


ISR(TIMER1_COMPA_vect)
{
	SET_BIT(SREG, 7);					/* Re-enable Global Interrupt (I-Bit) in to enable another interrupt*/
	SEC_1_FLAG = TRUE;					/* Raise a second flag to Count each one second */
}


ISR(INT2_vect)
{
	SET_BIT(TIMSK, OCIE1A);				/* Timer1 CTC Interrupt Enable to resume count using External Interrupt INT2 using Bush Button*/
	TCNT1 = INTIAL_VALUE;				/* Resume count from initial value */
}


void Timer1_CTC_Init(void)
{
	TCNT1 = INTIAL_VALUE;				/* Set initial value of Timer1 to start from zero */

	/*
	 * 		F_CPU = 1Mhz, N = 64 (Pre-Scaler)
	 * 		F_Timer = 1M / 64 = 15,625 Hz
	 * 		T_Timer = 1 / 15,625 = 64 Micro-Sec
	 * 		SEC per T_Timer = 1 Sec / 64 Micro-Sec = 15625
	 */
	OCR1A = 15625;						/* Set the Compare value to 15625 to count every one second*/

	SET_BIT(TIMSK, OCIE1A);				/* Enable Timer1 Compare A Interrupt */

	/* Configure timer control register TCCR1A
	 * 1. Disconnect OC1A and OC1B  COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
	 * 2. FOC1A=1 FOC1B=0
	 * 3. CTC Mode WGM10=0 WGM11=0 (Mode Number 4)
	 */
	TCCR1A |= (1 << FOC1A);

	/* Configure timer control register TCCR1B
	 * 1. CTC Mode WGM12=1 WGM13=0 (Mode Number 4)
	 * 2. Pre-scaler = F_CPU/64 CS10=1 CS11=1 CS12=0
	 */
	TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);
}


void INT0_Init(void)
{
	SET_BIT(MCUCR, ISC01);				/* Set INT0 to interrupt at Falling Edge */
	SET_BIT(GICR, INT0);				/* Enable External Interrupt 0 */
}


void INT1_Init(void)
{
	MCUCR |= (1 << ISC11) | (1 << ISC10);		/* Set INT1 to interrupt at Rising Edge */
	SET_BIT(GICR, INT1);						/* Enable External Interrupt 1 */
}


void INT2_Init(void)
{
	CLEAR_BIT(MCUCSR, ISC2);			/* Set INT2 to interrupt at Falling Edge */
	SET_BIT(GICR, INT2);				/* Enable External Interrupt 2 */
}


int main(void)
{
	DDRA |= 0X3F;						/* Set pins (0 -> 5) as O/P */
	PORTA |= 0X3F;						/* Initiate pins (0 -> 5) to ones */

	DDRC |= 0X0F;						/* Set pins (0 -> 3) as O/P */
	PORTC &= 0XF0;						/* Initiate pins (0 -> 3) to zeros */

	CLEAR_BIT(DDRB, PINB2);				/* Set pin as I/P */
	SET_BIT(PORTB, PINB2);				/* Active Internal Pull-Up Res. */

	CLEAR_BIT(DDRD, PINB2);				/* Set pin as I/P */
	SET_BIT(PORTD, PINB2);				/* Active Internal Pull-Up Res. */

	CLEAR_BIT(DDRD, PINB3);				/* Set pin as I/P */

	SET_BIT(SREG, 7);					/* Enable Global Interrupt (I-Bit) in M.C */

	/* Calling Timers & Interrupts Functions */
	INT0_Init();
	Timer1_CTC_Init();
	INT1_Init();
	INT2_Init();

	/* Super Loop of Stop-Watch */
	for(;;)
	{
		if(SEC_1_FLAG)
		{
			if((SEC_1 & 0x0F) == COUNT_ONES)
			{
				SEC_1 = INTIAL_VALUE;
				if((SEC_2 & 0x0F) == COUNT_TENS)
				{
					SEC_2 = INTIAL_VALUE;
					if((MIN_1 & 0x0F) == COUNT_ONES)
					{
						MIN_1 = INTIAL_VALUE;
						if((MIN_2 & 0x0F) == COUNT_TENS)
						{
							MIN_2 = INTIAL_VALUE;
							HOUR_1_FLAG = TRUE;
						}
						else
						{
							MIN_2++;
						}
					}
					else
					{
						MIN_1++;
					}
				}
				else
				{
					SEC_2++;
				}
			}
			else
			{
				SEC_1++;
			}
			SEC_1_FLAG = INTIAL_VALUE;				/* Reset Seconds Flag to re-enterant*/
		}


		if(HOUR_1_FLAG)
		{
			if((HOUR_1 & 0x0F) == COUNT_ONES)
			{
				HOUR_1 = INTIAL_VALUE;
				if((HOUR_2 & 0x0F) == COUNT_TENS)
				{
					HOUR_2 = INTIAL_VALUE;
				}
				else
				{
					HOUR_2++;
				}
			}
			else
			{
				HOUR_1++;
			}
			HOUR_1_FLAG = INTIAL_VALUE;				/* Reset Hours Flag to re-enterant*/
		}


		/* Enable & Disable every 2 milli-second 7-Segments pins to show counter */
		PORTA &= 0XC0;
		SET_BIT(PORTA, 0);
		PORTC = (PORTC & 0XF0) | (SEC_1 & 0X0F);
		_delay_ms(2);

		PORTA &= 0XC0;
		SET_BIT(PORTA, 1);
		PORTC = (PORTC & 0XF0) | (SEC_2 & 0X0F);
		_delay_ms(2);

		PORTA &= 0XC0;
		SET_BIT(PORTA, 2);
		PORTC = (PORTC & 0XF0) | (MIN_1 & 0X0F);
		_delay_ms(2);

		PORTA &= 0XC0;
		SET_BIT(PORTA, 3);
		PORTC = (PORTC & 0XF0) | (MIN_2 & 0X0F);
		_delay_ms(2);

		PORTA &= 0XC0;
		SET_BIT(PORTA, 4);
		PORTC = (PORTC & 0XF0) | (HOUR_1 & 0X0F);
		_delay_ms(2);

		PORTA &= 0XC0;
		SET_BIT(PORTA, 5);
		PORTC = (PORTC & 0XF0) | (HOUR_2 & 0X0F);
		_delay_ms(2);
	}
}
