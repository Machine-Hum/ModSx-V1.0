/*
 * 8BitLFO.c
 *
 * Walker - LFO (Low Frequency Ossilator)
 * 1Hz - 20Hz
 *
 * Created: 5/14/2015 6:21:04 PM
 *  Author: Walker
 */ 

//31254 "Ticks" on 16Bit Counter w/ 256 Samples = 1Hz
//1563 "Ticks" on 16Bit Counter w/ 256 Samples = 20Hz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#include <math.h>

#define PI 3.1415926
#define SIN 0
#define SAW 1
#define TRI 2
#define LED PD2

uint8_t SinW[256], WaveForm, Pointer;
uint16_t Tmr1;

//Funk Prototypes
void init(void);
void fill(void);
void set_8(uint8_t);
void set_16(uint16_t);
void delay(uint16_t);

ISR(TIMER1_COMPA_vect)
{
	cli(); // Don't do it
	
	TCNT1H = 0; //Kill the timer
	TCNT1L = 0;
	
	Pointer++; //Pointer up PLX
	
	//Saw Tooth
	if(WaveForm == SAW)
		set_8(Pointer);
	
	//Triangle
	else if(WaveForm == TRI) 
	{
		if(Pointer < 0x7F)
			set_8(Pointer*2);
		
		else if(Pointer > 0x7F)
			set_8((0xFF - Pointer)*2);
	}
	else //WaveForm == SIN
		set_8(SinW[Pointer]);
	
	
	//set_8(126);
		
	sei(); //Over
}

int main(void)
{
	Pointer = 0;
	
	cli(); //Disable interrupts for now plx
	
	init();
	fill();
	
	sei(); //Enable interrupts

	ADCSRA |= (1 << ADSC); //Start first Analog read

    while(1)
	{
		if((ADCSRA >> ADIF) & 1) //if there's a reading...
			{
				
				set_16((ADCH*(-119)) + 31254); //Set Deal
				ADCSRA |= (1 << ADIF); //READ THIS SCALEING THING
				
			}
			_delay_ms(10); // Let it tick a little
			
			if(~(PINC >> PC1) & 1)
				WaveForm = SAW;
			else if(~(PINC >> PC2) & 1)
				WaveForm = TRI;
			else
				WaveForm = SIN;
		//nop
	}
}

void init(void)
{
	DDRD |= (1 << PD6) | (1 << LED); // OC0A output	
	DDRC &= (0b11110000); // Lower 4 are inputs
	
	PORTC |= (0b00001110);
	
	//8Bit PWM stuff
	TCCR0A = 0b10000011; 
	TCCR0B = 0b00000001; //8Mhz
	
	//16Bit Interupt Source
	TCCR1A = 0b00000000;
	TCCR1B = 0b00000001; //8MHz Clock
	TIMSK1 = 0b00000010;

	//ADC
	ADMUX = 0b01100000;
	ADCSRA = 0b10100000; //Enable ADC | AutoTrigger
	ADCSRB &= 0b01000000; //FreeRunning
	DIDR0 |= (1 >> ADC0D);
	
	//NOW I'M ALIVE
	uint8_t dex = 100;
	
	while(dex != 0)
	{
		dex = (dex - 10);
		PORTD |= (1<<LED);
		delay(dex);
		PORTD &= ~(1<<LED);
		delay(dex);
	}
	
	PORTD |= (1<<LED);
}

void fill(void)
{
	double temp;
	Pointer = 0;
	
	temp = (2*PI)/256;
	
	do
	{
		SinW[Pointer] = (int) (((sin(temp*Pointer))*0x7f)+0x7f);
		Pointer++;
		
	}while(Pointer < 255);
	
	SinW[255] = 127; //Quick and Dirty
	
	Pointer = 0;
	//Not yet
}

//Set PWM DUC of output PD6 (0x00 to 0xFF)
void set_8(uint8_t data)
{
	OCR0A = data;
}

//Set PWM DUC of output PD6 (0x00 to 0xFF)
void set_16(uint16_t data)
{
	OCR1AH = ((data & 0xFF00) >> 8);
	OCR1AL = (data & 0x00FF);
}


void delay(uint16_t time_ms)
{
	while (time_ms != 0)
	{
		_delay_ms(1);
		time_ms--;
		
	}
	
}