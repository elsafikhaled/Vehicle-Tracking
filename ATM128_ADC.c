/*
 * ATM128_ADC.c
 *
 *  Created on: ١١‏/٠٢‏/٢٠١٨
 *      Author: El-safi
 */

#include "ATM128_ADC.h"
#include <avr/io.h>

void ADC_SETUP(U8_t INTERRUPT){//! Start of ADC setup function.

//ENable ADC.
	ADCSRA|=(1<<ADEN);
//check interrupt enable.
	if(INTERRUPT==1){//!Interrupt check.
       ADCSRA|=(1<<ADIE);
	}
	else{//default.
	   ADCSRA&=~(1<<ADIE);
	}//!End of configuration interrupt.
//choose division factor to be 8.
	ADCSRA|=(1<<ADPS0)|(1<<ADPS1);
//choose external voltage reference.
	ADMUX|=(1<<REFS0);


}//! End of ADC setup function.


U8_t ADC_Read(U8_t CH){//! start Reading ADC result.

//choose channel(0->7) by MUX.
	ADMUX=(0x40|CH);
//start conversion.
    ADCSRA|=(1<<ADSC);
//check for data converted successfully.
    while(!(ADCSRA&(1<<ADIF)));
//return converted,digital byte.
	return ADC;

}//! End of Reading ADC result.
