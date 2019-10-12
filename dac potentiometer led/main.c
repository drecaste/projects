void UART_Init(unsigned int ubrr);
unsigned char USART_Receive( void );
void USART_Transmit(unsigned char data);
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

unsigned char val;

#define DD_MOSI PB3
#define DD_SCK PB5
#define SPI_SS PB2

void SPI_MasterInit(void)
{
	// Set MOSI and SCK output, all others input
	DDRB = (1<<DD_MOSI)|(1<<DD_SCK) | (1<<SPI_SS);
	// Enable SPI, Master, clock = fosc/16
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);
}
void SPI_MasterTransmit(uint16_t cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
}

int main(void)
{
	DDRC |= (1 << DDC1); // input knob
	DDRD |= (1 << DDD6); // output (led)

	UART_Init(MYUBRR);
	InitADC();
	SPI_MasterInit();

	while (1)
	{
		val = GetADCResult(); // 0 - 255

		int i = val;
		char temp[20];
		itoa(i, temp, 10);
		USART_AllText(temp);
		USART_AllText(" - ");

		// 5V / 4095 possible. from 1.5v to 5v // volt in = vref(5v) * (adc reading / 4095)
		// min val = 4095 * 1.5 / 5 = 1228.5 -> 1229
		// 1.5v -> 5 * 1229 / 4095 -> map to 0   adc value
		// 5.0v -> 5 * 4095 / 4095	-> map to 255 adc value
		// 4095 - 1229 = 2866 / 256 possible values -> 11.1953125
		// voltage = 1229 + (val * 11.1953125) / 4095

		unsigned short voltage = 1229 + (val * 11.1953125);	

		char temp2[20];
		itoa(voltage, temp2, 10);
		USART_AllText(temp2);
		USART_AllText("\r\n");

		PORTB &= ~(1 << SPI_SS);				// slave select low

		voltage |= (1<<12) | (1<<13);			// output gain bit = 1, output power down = 1
		SPI_MasterTransmit(voltage >> 8);
		SPI_MasterTransmit(voltage & 0xFF);		// 0b11111111 // 255

		PORTB |= (1 << SPI_SS);					// slave select high
	}
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


void UART_Init(unsigned int ubrr)
{
	UBRR0H = (unsigned char) (ubrr >> 8);
	UBRR0L = (unsigned char) ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

unsigned char USART_Receive( void )
{
	while ( !(UCSR0A & (1 << RXC0)));
	return UDR0;
}

void USART_Transmit(unsigned char data)
{
	while ( !( UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}


void USART_AllText(char * text)
{
	while(*text)
	{
		USART_Transmit(*text++);
	}
}

