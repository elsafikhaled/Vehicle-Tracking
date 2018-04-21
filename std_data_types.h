/*
 * std_data_types.h
 *
 *  Created on:17/9/2016
 *            :saturday
 *      Author: El-safi
 */

#ifndef STD_DATA_TYPES_EKE_H_
#define STD_DATA_TYPES_EKE_H_

typedef signed char        S8_t ;     // 1 byte
typedef unsigned char      U8_t ;     // 1 byte

typedef unsigned char      BOOLEAN ;  //byte

typedef signed int         S16_t ;    // 2 byte   but it depend on compiler
typedef unsigned int       U16_t ;    // 2byte

typedef signed long        S32_t ;    // 4 byte
typedef unsigned long      U32_t ;    //4byte

typedef  float             FP32_t ;   //4byte
typedef double             DP64_t ;   // 8byte


#define TRUE               1
#define FALSE              0
#define ON                 1
#define OFF                0


#define WORD              U16_t
#define Sbyte             U8_t

#define SUCCESS           1
#define FAILURE           -1
//#define NULL              0
#define input             0
#define output            1
#endif /* STD_DATA_TYPES_EKE_H_ */
