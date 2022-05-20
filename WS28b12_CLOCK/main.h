/*
 * main.h
 *
 * Created: 30.11.2021 16:51:18
 *  Author: Alex2
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "WS2812.h"
#include "WS2812Part2.h"
//#include "RTC.h"
#include "RTC_DS3231.h"
#include "TWI.h"
#include "adc.h"
#include "DS18B20.h"
#include "button.h"
#include "eeprom.h"
//---------------------------------
//выбор режимов/цветов
#define NONE 0//не горят
#define GREEN 1
#define RED 2
#define BLUE 3
#define WHITE 4
#define YELLOW 5
#define PURPLE 6
#define ORANGE 7
#define BLUELIGHT 8
#define DIFFERENT_colours 9
#define COLOUR_FIGURES 10
#define MODE11 11
#define MODE12 12
#define RADUGA 13
//---------------------------------
//выбор режима индикации:часы/температура/дата/день недели
#define CLOCKMODE_CLOCK 0
#define CLOCKMODE_TEMPERATURE 1
#define CLOCKMODE_DATE 2
#define CLOCKMODE_WEAKDAY 3
#define CLOCKMODE_YEAR 4
#define CLOCKMODE_ALARMCLOCK 5
//---------------------------------
//выбор режима редактирования
#define MODE_WITHOUT_CHANGE 0
#define MODE_CHANGE_HOURS 1
#define MODE_CHANGE_MINUTES 2
#define MODE_CHANGE_DATE 3
#define MODE_CHANGE_MONTH 4
#define MODE_CHANGE_YEAR 5
#define MODE_CHANGE_WEAKDAY 6
#define MODE_CHANGE_ALARM_HOURS 7
#define MODE_CHANGE_ALARM_MINUTES 8
//---------------------------------
//выбор режима инкрементирования показателей либо оставить без изменений
#define MODE_INC 1 //инкрементирование показателя
#define MODE_NO_INC 2 //не меняем показатель

#endif /* MAIN_H_ */