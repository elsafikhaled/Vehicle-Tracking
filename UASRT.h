/*
 * UASRT.h
 *
 *  Created on: ٠٨‏/٠٢‏/٢٠١٨
 *      Author: El-safi
 */

#ifndef UASRT_H_
#define UASRT_H_
#include "std_data_types.h"

void UASRT_Setup(U8_t module,U8_t Int_TX,U8_t Int_RX);
/*
 * Function  :: -setup the uart0-uart1 mode for << 9600 baudRate- 8bit- one stop bit- No parity >>
 *
 * parameters:: - module 0 for UART0 or 1 for UART1.
 *              - Int_TX 0 for disable TX interrupt or 1 for enable it.
 *              - Int_RX 0 for disable RX interrupt or 1 for enable it.
 *
 ********************************************************************************************************/

void USART_Write_byte(U8_t module,U8_t data);
/*
 * Function   :: - Send one byte.
 *
 * parameters :: - module 0 for UART0 or 1 for UART1.
 *               - data = one byte TX
 *
 **********************************************************************************************************/
U8_t USART_Read_byte(U8_t module);
/*
 * Function   :: - Read one byte.
 *
 * parameters :: - module 0 for UART0 or 1 for UART1.
 *
 * Return     :: - one byte- RX
 *
 **********************************************************************************************************/
void USART_Write_String(U8_t module,U8_t *phrase);
/*
 * Function   :: - Send number of  bytes.
 *
 * parameters :: - module 0 for UART0 or 1 for UART1.
 *               - data = string.
 *
 **********************************************************************************************************/
#endif /* UASRT_H_ */
