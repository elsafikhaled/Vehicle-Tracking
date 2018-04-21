/*
 * ATM128_ADC.h
 *
 *  Created on: ١١‏/٠٢‏/٢٠١٨
 *      Author: El-safi
 */

#ifndef ATM128_ADC_H_
#define ATM128_ADC_H_
#include "std_data_types.h"


void ADC_SETUP(U8_t INTERRUPT);
/************************************************************************
 * Function    :: setup up the ADC registers.
 *
 * parameters  :: INTERRUPT Enable=1 , Disable=0.
 *
 * Description :: - External voltage reference.
 *                - Divison factor 8
 ************************************************************************/
U8_t ADC_Read(U8_t CH);
/************************************************************************
 * Function   :: to get digital reading from ADC 1 byte.
 *
 * Parameters :: CH to choos ADC channel from 0 to 7.
 *
 ************************************************************************/

#endif /* ATM128_ADC_H_ */
