/*
 * main.c
 *
 *  Created on: ٠٦‏/٠٤‏/٢٠١٨
 *      Author: El-safi
 */
#include "UASRT.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "std_data_types.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ATM128_ADC.h"

//Macros GSM/SIM808
#define true  1
#define false 0
#define ADC_ACTIVE false

//function declarations. GSM/SIM808.
void GSM_Send_Msg(char *num,char *sms);
void GSM_Setup(void);
char GSM_Wait_for_Msg();
void GSM_Msg_Read(int position);
void GPS_Setup(void);
void GSM_Delete_All_Msg();
void GPS_READ(void);


//global data types. GSM - SIM808

char buff[160];			/* buffer to store responses and messages */
volatile char status_flag = 0;		/* for checking any new message */
volatile int buffer_pointer;
char Mobile_no[14];		/* store mobile no. of received message */
char message_received[60];	/* save received message */
int position = 0;		/* save location of current message */
volatile U8_t GSM_ENABLE=true;
//global data types*-GPS-*
char gps_buff[160];
volatile int gps_buff_pointer;
volatile U8_t GPS_ENABLE=false;
char location[100];
char *GPS;
//latitude
char *GPSL;
//longitude.
char *GPSg;
//store GPS location.
char gps_sms[160];
char comma[10];
volatile int myindex=0;
U8_t sms_index=0;
U8_t index2=0;
//voltage -current sensors.
U16_t voltage=0,current=0,VDigital=0,Cdigital=0;

/************************************* ISR RX UART0 ************************************************************/

ISR(USART0_RX_vect){// START service routine to get information from SIM808 From UART0 RX-interrupt.

//! GSM
if(GSM_ENABLE==true){//save messages from GSM.
	   	/* flag for new message arrival */
buff[buffer_pointer] = UDR0;	/* copy UDR (received value) to buffer */
	   buffer_pointer++;
	   status_flag = 1;
                    }//END-GSM
//! GPS
else if(GPS_ENABLE==true){//save GPS data.
	   gps_buff[gps_buff_pointer] = UDR0;	// copy UDR (received value) to buffer
	   if(gps_buff[gps_buff_pointer]==','){
		   comma[myindex]=gps_buff_pointer;
		   myindex++;
	   }
	   gps_buff_pointer++;

                         }//END-GP
}//ISR

/* ******************************************** START SYSTEM. ************************************************************/

int main(void){//MAIN FUNCTION.

//start GSM/GPS SIM808 buffers.
  U16_t is_msg_arrived=0;
  memset(message_received, 0, 60);
  memset(buff,0,160);
  memset(Mobile_no, 0, 14);
  memset(gps_buff,0,160);

//system initialization.
 // UART0 for SIM808 module.
  UASRT_Setup(0,0,1);
  UASRT_Setup(1,0,0);
 // ADC for Analog  sensors-close interrupt.
  ADC_SETUP(0);
 // start global interrupt.
  sei();
  //start GSM MODULE.
  GSM_Setup();
  //Delete All messages.
  GSM_Delete_All_Msg();

  USART_Write_String(1,(U8_t*)"GSM STARTED\n");

	while(1){

		// **************************** check if any new message received ************************************
		if(status_flag==1){//check for message arrival
			is_msg_arrived = GSM_Wait_for_Msg();
			if(is_msg_arrived== true)
			{
			   USART_Write_String(1,(U8_t*)"message reached\n");
			   GSM_Msg_Read(position); // read arrived message
              // _delay_ms(3000);
               if(strstr( message_received,"LOCATION")){
            	   //when location is word is received so send SMS with google map link.
    			   USART_Write_String(1,(U8_t*)"message started to sent\n");
    			   //turn ON GPS.
    			   GPS_Setup();
    			   //Return "location" global variable as string.
    			   GPS_READ();
    			   //send location = google map link.
            	   GSM_Send_Msg("01001646878",location);
    			   USART_Write_String(1,(U8_t*)"message sent\n");
            	   _delay_ms(1000);
               }//check message TEXT.
			}//message arrived.
	    }//status_flag
		  //zeros all flags.
		  memset(Mobile_no, 0, 14);
		  memset(message_received, 0, 60);

      // ************************************ GET CURRENT,VOLTAGE VALUES    *******************************
#if (ADC_ACTIVE==true)
        //get current from sensor.
		  Cdigital=ADC_Read(0);
		  current=(U16_t)((((float)Cdigital*0.074)-(float)37.87));
		//get voltage.
          VDigital=ADC_Read(1);
          voltage=(U16_t)(VDigital*(float)0.00488);
#endif


        }//! WHILE-LOOP-INFINTY.

	return 0;

}//! MAIN FUNCTION.





/* *********************** GPS SETUP ****************************************************************/
void GPS_Setup(void){//!GPS-SETUP
	//enable GPS-Turn GSM off
      GPS_ENABLE=true;
      GSM_ENABLE=false;
    //GSM AT
	  USART_Write_String(0,(U8_t*)"AT\r");
	  _delay_ms(1000);
	  //power ON GPS.
	  USART_Write_String(0,(U8_t*)"AT+CGNSPWR=1\r");
	  _delay_ms(1000);
	  //make format is NMEA "GPSGGA"=power active,fix status,time-date,latitude,longitude,....,etc.
	  USART_Write_String(0,(U8_t*)"AT+CGNSSEQ=\"GGA\"\r");
	   _delay_ms(1000);
	  //make received data as string data.not hex data.
	  USART_Write_String(0,(U8_t*)"AT+CGNSCMD=0,\"$PMTK000*32\"\r");
	   _delay_ms(1000);
	  //start to get NMEA string.
	  USART_Write_String(0,(U8_t*)"AT+CGNSINF\r");
	  _delay_ms(1000);
}//!GPS-SETUP

/* ***************************** GPS READ FUNCTION *******************************************************/
void GPS_READ(void){//! start reading latitude,Longitude.
	//search in NMEA for ',' to get our data.
	 char dim[]=",";
	//GPS power on/off.
	   GPS=strtok(gps_buff,dim);
	   if(GPS){//IF-GPS
		   //GPS fix status
           strtok(NULL,dim);
           //time-date
           strtok(NULL,dim);
           //start to get our navigation information.
           strtok(NULL,dim);
           //get latitude.
           GPSL=strtok(NULL,dim);
           //get longitude.
           GPSg=strtok(NULL,dim);
           //convert numbers into string. with LINK FORMAT.
	       sprintf(location,"http://maps.google.com/?q=%s,%s",GPSL,GPSg);
		   //enable GSM-Turn GPS off
		    GSM_ENABLE=true;
		    GPS_ENABLE=false;
		   //zeros buffer of GPS to save new reading.
		    memset(gps_buff,0,160);
		    gps_buff_pointer=0;
	          }//If-GPS


}//!END-GPS-RAEDING.
/* **************************************** GSM SETUP  *********************************************** */
void GSM_Setup(void){//GSM SETUP
	//enable GSM-Turn GPS off
      GSM_ENABLE=true;
      GPS_ENABLE=false;
    //GSM AT-make 0.5 second delay to let SIM808 module return "OK" answer,or error flag.
	  USART_Write_String(0,(U8_t*)"AT\r");
	  _delay_ms(500);
	//configure GSM FOR TEXT format.
	  USART_Write_String(0,(U8_t*)"AT+CMGF=1\r");
	  _delay_ms(500);

}//!GSM SETUP


/* ****************************************** GSM SEND SMS  ******************************************* */



void GSM_Send_Msg(char *num,char *sms)
{//! START SENDING SMS
	char sms_buffer[35];
	buffer_pointer=0;
	//send mobile number that will receive SMS.
	sprintf(sms_buffer,"AT+CMGS=\"%s\"\r",num);
	//send command AT+CMGS="Mobile No."\r
	USART_Write_String(0,(U8_t*)sms_buffer);
	//make delay to let sim808 return '>'=0x3e in ASCI. when MCU receive it so we have access to send text sms.
	_delay_ms(200);

	while(1)
	{//search for '>'
		if(buff[buffer_pointer]==0x3e) /* wait for '>' character*/
		{ // '>' is found so now send SMS text.
		   buffer_pointer = 0;
		   memset(buff,0,strlen(buff));
		   USART_Write_String(0,(U8_t*)sms); /* send SMS to given no. */
		   USART_Write_byte(0,0x1A); /* send CTRL+Z =0X1A */
		   break;
		}
		buffer_pointer++;
	}//end of searching loop.
	_delay_ms(300);
	//zeros buffer for new massages to be sent.
	buffer_pointer = 0;
	memset(buff,0,strlen(buff));
	memset(sms_buffer,0,strlen(sms_buffer));
}//!SEND SMS.

/* *************************************************** GSM MESSGE READ  ************************************************** */
void GSM_Msg_Read(int position)
{//start reading messages.
	char read_cmd[10];
	//get position of message in memory. position is global variable.
	sprintf(read_cmd,"AT+CMGR=%d\r",position);
	// read message at specified location/position
	USART_Write_String(0,(U8_t*)read_cmd);
	_delay_ms(500);
	//when AT+CMGR=position, is sent so module will return +CMGR:mobile number,text.
	if(!(strstr(buff,"+CMGR")))	//check for +CMGR response
	{
		//Write some codes here as no message reached.
	}
	else
	{//message reached.
		buffer_pointer = 0;

		while(1)
		{
              //wait till \r\n is found to get first number.

			if(buff[buffer_pointer]=='\r' || buff[buffer_pointer]== 'n'){
				buffer_pointer++;
			}
			else{
			     break;
		     	}
		 }

		// search for 1st ',' to get mobile no.
		while(buff[buffer_pointer]!=',')
		{
			buffer_pointer++;
		}
		buffer_pointer = buffer_pointer+2;

		/* extract mobile no. of message sender */
		for(int i=0;i<=12;i++)
		{
			Mobile_no[i] = buff[buffer_pointer];
			buffer_pointer++;
		}

		do
		{
			buffer_pointer++;
		}while(buff[buffer_pointer-1]!= '\n');

		int i=0;

		/* display and save message */
		while(buff[buffer_pointer]!= '\r' && i<31)
		{
				message_received[i]=buff[buffer_pointer];
				buffer_pointer++;
				i++;

		}

		buffer_pointer = 0;
		memset(buff,0,strlen(buff));
	}
	status_flag = 0;
}
/* ******************************************** CHECK MESSAGE  ******************************************************** */
char GSM_Wait_for_Msg()
{//check if message is received or not ?!
	char msg_location[4];
	int i;
	_delay_ms(500);
	buffer_pointer=0;

	while(1)
	{
           /*eliminate "\r \n" which is start of string */

	   if(buff[buffer_pointer]=='\r' || buff[buffer_pointer]== '\n'){
			buffer_pointer++;
		}
		else{
			break;
		    }
	}

	/* "CMTI:" to check if any new message received */

	if(strstr(buff,"CMTI:")){// check if received to get message location.
		while(buff[buffer_pointer]!= ',')
		{
			buffer_pointer++;
		}
		buffer_pointer++;

		i=0;
		while(buff[buffer_pointer]!= '\r')
		{
			msg_location[i]=buff[buffer_pointer];/* copy location of received message where it is stored */
			buffer_pointer++;
			i++;
		}

		/* convert string of position to integer value */
		position = atoi(msg_location);

		memset(buff,0,strlen(buff));
		buffer_pointer=0;

		return true;
	}
	else
		return false;
    }//!CHECH MESSAGE.


/* *************************************** DELETE MESSAGE  ********************************************************** */

void GSM_Delete_All_Msg()
{//delete all messages in module memory.
	USART_Write_String(0,(U8_t*)"AT+CMGDA=\"DEL ALL\"\r"); /* delete all messages of SIM */
	_delay_ms(500);
}//! DELETE MESSAGES
