/*
 * button.c
 *
 * Created: 03.01.2022 23:29:53
 *  Author: Alex2
 */ 
#include "main.h"
#include "button.h"
//выбор режима индикации:часы/температура/дата/день недели
#define CLOCKMODE_CLOCK 0
#define CLOCKMODE_TEMPERATURE 1
#define CLOCKMODE_DATE 2
#define CLOCKMODE_WEAKDAY 3
#define CLOCKMODE_YEAR 4
#define CLOCKMODE_ALARMCLOCK 5
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
//———————————————
//выбор режима инкрементирования показателей либо оставить без изменений
#define MODE_INC 1 //инкрементирование показателя
#define MODE_NO_INC 2 //не меняем показатель
//———————————————
extern unsigned int button_counter;
extern unsigned char clock_change_mode, clock_incr_mode, alarm_now_mode;
extern int clockmode;
extern int Flag;
unsigned char buttonstat = 0,cnt=0;
//———————————————
void init_button_timer(void)//инициализация нулевого таймера
{
	TIMSK |= (1<<TOIE0); //устанавливаем бит разрешения прерывания 0-ого счетчика
	TCCR0 |= (1<<CS02)|(1<<CS00); // устанавливаем предделитель 1024
}
//———————————————
ISR (TIMER0_OVF_vect)
{
	  if (buttonstat == 1) cnt++;
	  if (cnt > 30)
	  {
		  buttonstat = 0;
		  cnt = 0;
	  }
	  if ((!(PIND&0b01000000)) && (buttonstat == 0))//если кнопка нажата и это не продолжение нажатия
	  {
		 button_counter++; 
	  }
	  else if ((button_counter > 5)&&(button_counter <= 40)) //если будет до 3 — то считаем это дребезгом
	  {
		  clock_incr_mode = MODE_INC;//включаем прибавление значения показателя
		  if (clock_change_mode == MODE_WITHOUT_CHANGE)   alarm_now_mode = 0;//отключение будильника
	      button_counter = 0;
	  }
	  else if (button_counter > 40) 
	  {
		  if (clock_change_mode < MODE_CHANGE_ALARM_MINUTES) 
		  {
			clock_change_mode++; 
			Flag++; 
		  }
		  else {clock_change_mode = MODE_WITHOUT_CHANGE;Flag = 0;}
		  clock_incr_mode=MODE_NO_INC;
		  button_counter = 0;
		  buttonstat = 1;
	  }
}