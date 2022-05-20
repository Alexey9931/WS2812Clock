/*
 * RTC.h
 *
 * Created: 16.12.2021 16:08:31
 *  Author: Alex2
 */ 


#ifndef RTC_H_
#define RTC_H_

#include "main.h"

unsigned char RTC_ConvertFromDec(unsigned char c); //перевод двоично-десятичного числа в десятичное
unsigned char RTC_ConvertFromBinDec(unsigned char c); //перевод десятичного числа в двоично-десятичное
void ModifyRTC(void);


#endif /* RTC_H_ */