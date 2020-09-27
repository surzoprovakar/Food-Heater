/*
 * servo_demo.c
 *
 * Created: 26-Dec-17 6:27:55 PM
 * Author : Provakar
 */



#include<stdio.h>
#include<stdlib.h>

#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"


#define SERVO_MAX 2300
#define SERVO_MIN 500

unsigned short degreeToOcr(int degree)
{
	return (SERVO_MIN + (SERVO_MAX - SERVO_MIN) /180 * degree);  // (1000) * (1 + degree / 180);
}

void Rotate(int degree)
{
	OCR1A = ICR1 - degreeToOcr(degree);
	return;
}

int read_adc_channel(unsigned char channel)
{
	int adc_value;
	unsigned char temp;
	ADMUX=(1<<REFS0)|channel;
	_delay_ms(1);
	temp=ADCL;
	adc_value=ADCH;
	adc_value=(adc_value<<8)|temp;
	return adc_value;
}

void adc_init(void)
{
	ADCSRA=(1<<ADEN)|(1<<ADSC)|(1<<ADATE)|(1<<ADPS2);
	SFIOR=0x00;
}

int main(void)
{
    /* Replace with your application code */
	unsigned int result;
	unsigned volt,volt2;
	int adc_output;
	char string[256];

	DDRC = 0xFF; //Makes RC0 output pin
	//PORTC = 0x00;
	DDRD = 0xFF;
	//unsigned char c = 0x01;
	//unsigned char x = 0x02;
	DDRB= 0xFF;

	/*
		//Configure TIMER1
		TCCR1A |= (1 << COM1A1) | (1 << COM1A0) | (1 << WGM11);        //NON Inverted PWM
		TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS10);		   //PRESCALER=64 MODE 14(FAST PWM)

		ICR1 = 19999;  	//fPWM=50Hz (Period = 20ms Standard).
		DDRB |= 0xFF;     //(1<<PD4);   //PWM Pins as Out

	MCUCSR = (1<<JTD);*/
	adc_init();
	//ADCSRA = 0x80; //make ADC enable and select ck/4
	//ADMUX = 0xE0;
	//ADCSRA = 0x8F;
	//ADMUX = 0xE0;
	int state=0;
	Lcd4_Init();
	Lcd4_Clear();

	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Temp:");
    while (1)
    {


		adc_output=read_adc_channel(0);
		volt=(adc_output*125)/512;

		memset(string,0,sizeof(string[0])*256); // Clear all to 0 so string properly represented
		sprintf(string,"%d *C",volt);

		Lcd4_Set_Cursor(1,10);
		Lcd4_Write_String(string);

		//adc_output=read_adc_channel(0);
		//volt2=(adc_output*125)/512;

		//float val = (volt + volt2 )/2;

	if(volt<35){  //sustain temperature 35 degree C
		PORTD = 0x01;
		//if(state!=1){
			//Rotate(0);    //0 degree 500;
			//_delay_ms(500);
			//state=1;
			//OCR1A=0;
		//}
	}
	else{
		PORTD = 0x02;

		//if(state!=2){
			//Rotate(60);   //45 degree 950;//
			//_delay_ms(500);
			//state=2;
			//OCR1A=0;
		//}
		int i = 0;
		while(i<6) {
			PORTC = 0x01;  //servo will rotate
			PORTC = 0x00;
			i++;
		}

	}
	//_delay_ms(100);

	_delay_ms(100);
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Temp:");

    }
}

