#include<avr/io.h>
#include<util/delay.h>
#include<stdio.h>
#include<avr/interrupt.h>
#include<string.h>
#include<stdlib.h>
#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
void InitADC(void);
unsigned char GetADCResult(void);
void USART_AllText(char * text);
void InitTimer0( void );
// waveform generator pd6
	//TCCR0B |= (1<<WGM01) | (1<<WGM00);//1024
#define TIMER0_CNT 255;
unsigned char val;
int main(void)
{
	// for led 
	DDRC |= (1 << DDC1); // input  (photo sensor)	// knob
	DDRD |= (1 << DDD6); // output (led)			// buzzer
	DDRD |= (1 << DDD5);
	
	UART_Init(MYUBRR);
	InitADC();
	// for some reason it needs COM0A0 even though 11 is inverted mode
	// otherwise it gets brighter if the sensor is covered
	//TCCR0A |= (1<<WGM01) | (1<<WGM00) | (1 << COM0A1) | (1<<COM0A0);
	//TCCR0B |= 0b101;

	
	/*
//	 for led
    while (1)
    {
	    val = GetADCResult();

		if(val > 200)
		{
			OCR0A = val;
		}
		else
		{
			OCR0A = val/4;
		}
	    
    }
	*/
	
	// frequency math:
	
	// min freq - 1046
	// max freq - 2093
	// min adc val - 0
	// max adc val - 255

	// 2093 - 1046 = 1047
	// 1047 / 256 possible values = 4.08984375

	// adc value = 0
	// ( 0 * 4.08984375 ) + 1046 = 1046 frequency
	// ( 255 * 4.08984375 ) + 1046 = 2088.91015625 frequency ( close enough to 2093 )
	
/*
	// for buzzer
	TCCR0A |= (1<<WGM02) | (1<<WGM01) | (1<<WGM00) | (1 << COM0A1);
	TCCR0B |= 0b101;
*/
	InitTimer0();
	sei();

	while(1)
	{
		val = GetADCResult();
		OCR0A = 233 - ( val * 0.0865);
	}
}


void InitTimer0( void )
{
	TCCR0A = 0b00000000;
	//TCCR0A |= (1<<WGM01) | (1<<WGM00) | (1 << COM0A1);
	// No compare output, no waveform gen

	TCCR0B = 0b00000011; // 64
	//TCCR0B |= 0b101;

	// No forcing, No prescalar
	OCR0A = TIMER0_CNT;
	// Load Compare value for timer 0
	TIMSK0 = _BV(OCIE0A);
	// enable compare-match interrupt
	// _BV(n) == (1 << n)
	TCNT0 = 0;
	// Initialize timer to 0
}

ISR(TIMER0_COMPA_vect)
{
	PORTD ^= (1<<PORTD5);
	TCNT0 = 0;
}

void InitADC(void)
{
	DDRC &= 0x00; // PC1 = ADC1 is set as input
	// ADLAR set to 1 ? left adjusted result in ADCH
	// MUX3:0 set to 0001 ? input voltage at ADC1
	ADMUX = (1<<MUX0) | (1<<ADLAR);
	// ADEN set to 1 ? enables the ADC circuitry
	// ADPS2:0 set to 111 ? prescalar set to 128 (104us per conversion)
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
}

unsigned char GetADCResult(void)
{
	// Start A to D conversion
	ADCSRA |= (1<<ADSC);
	// Wait for conversion to complete
	while (ADCSRA & 1<<ADSC);
	return (ADCH);
}