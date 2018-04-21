/*
 * UASRT.c
 *
 *  Created on: ٠٩‏/٠٢‏/٢٠١٨
 *      Author: El-safi
 */
#include "UASRT.h"
#include<avr/io.h>


void UASRT_Setup(U8_t module,U8_t Int_TX,U8_t Int_RX){//start setup function.

//check which uart 0 or 1
if(module==0){//UART0 is configured.
  //Enable RX/TX module.
   UCSR0B|=(1<<RXEN0)|(1<<TXEN0);
  //No parity- Asynchronous mode- one stop bit- 8 bit frame..
   UCSR0C|=(1<<UCSZ00)|(1<<UCSZ01);
  //9600 bit per seconde
   UBRR0L=(U8_t)51;
  //check RX interrupt configuration.
   if(Int_RX==1){//RX interrupt.
	   UCSR0B|=(1<<RXCIE0);
   }//RX interrupt.
  //check TX interrupt configuration.
   if(Int_TX==1){//TX interrupt.
       UCSR0B|=(1<<UDRIE0);
   }//TX interrupt.

 }//End of configuration of uart0

else{//UART1 is configured.
  //Enable RX/TX module.
   UCSR1B|=(1<<RXEN1)|(1<<TXEN1);
  //No parity- Asynchronous mode- one stop bit- 8 bit frame..
   UCSR1C|=(1<<UCSZ10)|(1<<UCSZ11);
  //9600 bit per seconde
   UBRR1L=(U8_t)51;
   //check RX interrupt configuration.
    if(Int_RX==1){//RX interrupt.
 	   UCSR1B|=(1<<RXCIE1);
    }//RX interrupt.
   //check TX interrupt configuration.
    if(Int_TX==1){//TX interrupt.
        UCSR1B|=(1<<UDRIE1);
    }//TX interrupt.


 }//End of configuration of uart1

}//End of setup function.

/* *************************************************************************************************/

void USART_Write_byte(U8_t module,U8_t data){//start send data function.

	if(module==0){//start uart0
	 //check buffer empty flag
	    while(!(UCSR0A&(1<<UDRE0)));
	    UDR0=data;
	}//uart0

	else{//start uart1
	 //check buffer empty flag
	    while(!(UCSR1A&(1<<UDRE1)));
	    UDR1=data;
	}//uart1

}//end send data function.

/* ************************************************************************************************/
U8_t USART_Read_byte(U8_t module){//start Read data function.

	if(module==0){//start uart0
	 //check RX flag
	    while(!(UCSR0A&(1<<RXC0)));
	    return UDR0;
	}//uart0

	else{//start uart1
	 //check RX flag
	    while(!(UCSR1A&(1<<RXC1)));
	    return UDR1;
	}//uart1


}//end Read data function.

/* ************************************************************************************************ */
 void USART_Write_String(U8_t module,U8_t *phrase){//start of string function.

	while(*phrase!='\0'){
		USART_Write_byte(module,*phrase);
	     phrase++;
	}
}//end send string function.







