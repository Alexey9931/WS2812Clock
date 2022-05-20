/*
 * WS28b12_CLOCK.c
 *
 * Created: 30.11.2021 16:50:51
 * Author : Alex2
 */ 

#include "main.h"

unsigned char clock_change_mode, clock_incr_mode, alarmmode, alarm_flag, alarm_now_mode;
int mode = 1;
int counter_of_leds = 0;
int flag_mode11,flag_mode12 = 0;
int red_mode12 = 35;
int green_mode12 = 5;
int red_mode13 = 255;
int green_mode13 = 0;
int blue_mode13 = 0;
int flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0, flag5 = 0;
int timer1_count = 0;
int clockmode;
double BRIGHTNESS = 1;//яркость свечения светодиодов
unsigned char sec,min,hour,day,date,month,year,alarmhour,alarmmin;
unsigned int adc_value,adc_counter,adc_tmp;
int temperature=0; //переменная для хранения температуры
unsigned int button_counter;// счетчик времени нажатия кнопки для настройки
int Flag = 0;
//---------------------------------------------
//инициализация таймера для моргания в режиме редактирования часов
void timer_ini(void)
{
	TCCR1B |= (1<<WGM12); // устанавливаем режим СТС (сброс по совпадению)
	TIMSK |= (1<<OCIE1A); //устанавливаем бит разрешения прерывания 1ого счетчика по совпадению с OCR1A(H и L)
	OCR1AH = 0b01111010; //записываем в регистр число для сравнения
	OCR1AL = 0b00010010;
	TCCR1B |= (1<<CS11);//установим делитель.
	TCCR1B |= (1<<CS10);//частота получается 8 Гц
}
//---------------------------------------------
// прерывания таймера
ISR (TIMER1_COMPA_vect)
{
	timer1_count++;
	if (timer1_count > 1)
	{
		timer1_count = 0;
	}
	//------------------------------------------------------
	// для режима индикации mode11
		 if (counter_of_leds > 255)
		 {
			 flag_mode11 = 1;
		 }
		 if (counter_of_leds < 1)
		 {
			 flag_mode11 = 0;
		 }
		 if (flag_mode11 == 0) counter_of_leds += 5;
		 else counter_of_leds -= 5;
	//------------------------------------------------------
	// для режима индикации mode12
	if (green_mode12 > 50)
	{
		flag_mode12 = 1;
	}
	if (green_mode12 < 5)
	{
		flag_mode12 = 0;
	}
	if (flag_mode12 == 0)
	{
		red_mode12 = red_mode12 - 2;
		green_mode12 = green_mode12 + 3;
	}
	else
	{
		red_mode12 = red_mode12 + 2;
		green_mode12 = green_mode12 - 3;
	}
	//------------------------------------------------------
	// для режима индикации mode13 - радуга
	   if ( (flag1 == 0) &&(flag2 == 0) &&(flag3 == 0) &&(flag4 == 0) && (flag5 == 0)&& (green_mode13 < 255) && (red_mode13 == 255) && (blue_mode13 == 0) ) {green_mode13 = green_mode13 + 5;} //красный->оранжевый->желтый
	   else {flag1 = 1;
	   if ( (flag1 == 1) && (flag2 == 0)&&(flag3 == 0) &&(flag4 == 0) && (flag5 == 0) && (green_mode13 > 204) && (red_mode13 > 0) && (blue_mode13 == 0) ) {green_mode13 = green_mode13 - 3; red_mode13 = red_mode13 - 15;}//...->зеленый
	   else {flag2 = 1;
	   if ( (flag1 == 1) && (flag2 == 1) && (flag3 == 0) &&(flag4 == 0) && (flag5 == 0)&& (blue_mode13 < 152) && (green_mode13 > 147) && (red_mode13 == 0)) {green_mode13 = green_mode13 - 3; blue_mode13 = blue_mode13 + 8;}//...->голубой
	   else {flag3 = 1;
	   if ( (flag1 == 1) && (flag2 == 1) && (flag3 == 1) && (flag4 == 0) && (flag5 == 0)&& (green_mode13 > 27) && (red_mode13 <30 ) && (blue_mode13 <182 )){blue_mode13 = blue_mode13 + 3; green_mode13 = green_mode13 - 12; red_mode13 = red_mode13 + 3; }//1
	   else {flag4 = 1;
	   if ( (flag1 == 1) && (flag2 == 1) && (flag3 == 1) && (flag4 == 1) && (flag5 == 0)&& (blue_mode13 > 164) && (red_mode13 <165 ) && (green_mode13 >0 )){green_mode13 = green_mode13 - 3; blue_mode13 = blue_mode13 - 2; red_mode13 = red_mode13 + 15; }//2
	   else {flag5 = 1;
	   if ( (flag1 == 1) && (flag2 == 1) && (flag3 == 1) && (flag4 == 1) && (flag5 == 1) && (blue_mode13 > 2) && (red_mode13 <255 ) && (green_mode13 == 0 )){green_mode13 = 0; blue_mode13 = blue_mode13 - 18; red_mode13 = red_mode13 + 10; }//3   
	   else { flag1 = flag2 = flag3 = flag4 = flag5 = 0; red_mode13 = 255; green_mode13 = 0; blue_mode13 = 0;}}}}}}
	//------------------------------------------------------ 
	// выбор режима индикации clockmode
	if (clockmode == CLOCKMODE_CLOCK) print_all_number( hour*100+min, mode );
	if (clockmode == CLOCKMODE_TEMPERATURE) print_all_number( (temperature>>1)*100+(temperature%2)*50, mode );
	if (clockmode == CLOCKMODE_DATE) print_all_number( month + date*100, mode );
	if (clockmode == CLOCKMODE_WEAKDAY) print_all_number( day*10, mode );
	if (clockmode == CLOCKMODE_YEAR) print_all_number( 2000+year, mode );
	if (clockmode == CLOCKMODE_ALARMCLOCK) print_all_number( alarmhour*100+alarmmin, mode );
	//------------------------------------------------------
}
//---------------------------------------------
//прерывания от кнопки для переключения режимов индикации 
void int_ini(void)
{
	//включим прерывания INT0 по нисходящему фронту
	MCUCR |= (1<<ISC01);
	//разрешим внешние прерывания INT0
	GICR |= (1<<INT0);
}
//---------------------------------------------
//обработчик прерываний от кнопки выбора режимов
ISR(INT0_vect)
{
	mode++;
	if (mode == 14)
	{
		mode = 1;
	}
}
//---------------------------------------------
//инициализация портов
void port_ini(void)
{
	//Включим ножку INT0 (PD2) на вход
	DDRD &= ~(0b00000100);
	//Подтянем резистор на ножке INT0 (PD2) к питанию
	PORTD |= 0b00000100;
	
	DDRB = 0xFF;//установка ножек на выход
	PORTB = 0b00000000;
	
	//будильник
	DDRD |= (1<<DDRD); // ножку PD0 на выход
	PORTD |= (1<<PORTD0); //логическая 1
	
	DDRD &= ~(0b01000000); //кнопка PD6 - на вход
	PORTD |= 0b01000000;//Подтянем резистор на ножке PD6 к питанию
}
//---------------------------------------------
//функция звонка будильника
void alarm(void)
{
	PORTD&=~(1<<PORTD0); //логический 0
	_delay_ms(15);
	PORTD |= (1<<PORTD0); //логическая 1
}
//---------------------------------------------
int main(void)
{
	alarm_now_mode = 0;
	button_counter = 0;
	clock_change_mode = MODE_WITHOUT_CHANGE;
	clock_incr_mode = MODE_NO_INC;
	alarm_flag = 0;
	unsigned int adc_value;
    port_ini();
    init_button_timer();//инициализация нулевого таймера
	I2C_Init();
	int_ini();
	ADC_Init();
	RTC_init();//для DS3231
	sei();
	timer_ini();//инициализация таймера
	
	if(EEPROM_read(1) > 1) EEPROM_write(1,0); //если в ячейке число более 1 (либо ни разу не использовалось,
	//либо какие то странные значения, либо использовалось в других целях), то установим туда 0
	alarmmode = EEPROM_read(1); //считаем флаг задействования будильника из памяти EEPROM
	if(EEPROM_read(2)>23) EEPROM_write(2,0); //если в ячейке число более 23 (либо ни разу не использовалось,
	//либо какие то странные значения, либо использовалось в других целях), то установим туда 0
	if(EEPROM_read(3)>59) EEPROM_write(3,0); //если в ячейке число более 59 (либо ни разу не использовалось,
	//либо какие то странные значения, либо использовалось в других целях), то установим туда 0
	
	//---------------------------------------------
    /*  Для DS1307
	//Устанавливаем время
	I2C_StartCondition();
	I2C_SendByte(0b11010000);
	I2C_SendByte(0);//Переходим на 0x00
	I2C_SendByte(RTC_ConvertFromBinDec(0)); //секунды
	I2C_SendByte(RTC_ConvertFromBinDec(28)); //минуты
	I2C_SendByte(RTC_ConvertFromBinDec(14)); //часы
	I2C_SendByte(RTC_ConvertFromBinDec(1)); //день недели
	I2C_SendByte(RTC_ConvertFromBinDec(17)); //дата
	I2C_SendByte(RTC_ConvertFromBinDec(1)); //месяц
	I2C_SendByte(RTC_ConvertFromBinDec(22)); //год
	I2C_StopCondition();
    */
    // Установка времени для DS3231
    //RTC_write_time(14,35, 0);
    //RTC_write_date(5, 21, 1, 22);
	//---------------------------------------------
	_delay_ms(1000);
    while (1) 
    {
	  //---------------------------------------------
	  //Читаем время (для DS1307)
	  /*
	  //------------------------------------------------------
	  I2C_SendByteByADDR(0,0b11010000); //переходим на адрес 0	
	  _delay_ms(300);
	  I2C_StartCondition(); //Отправим условие START
	  I2C_SendByte(0b11010001); //отправим в устройство бит чтения
	  sec = I2C_ReadByte();
	  min = I2C_ReadByte();
	  hour = I2C_ReadByte();
	  day = I2C_ReadByte();
	  date = I2C_ReadByte();
	  month = I2C_ReadByte();
	  year = I2C_ReadLastByte();
	  I2C_StopCondition(); //Отправим условие STOP
	  sec = RTC_ConvertFromDec(sec); //Преобразуем в десятичный формат
	  min = RTC_ConvertFromDec(min); //Преобразуем в десятичный формат
	  hour = RTC_ConvertFromDec(hour); //Преобразуем в десятичный формат
	  day = RTC_ConvertFromDec(day); //Преобразуем в десятичный формат
	  year = RTC_ConvertFromDec(year); //Преобразуем в десятичный формат
	  month = RTC_ConvertFromDec(month); //Преобразуем в десятичный формат
	  date = RTC_ConvertFromDec(date); //Преобразуем в десятичный формат
	  */
	  //------------------------------------------------------
	  //Читаем время (для DS3231) - по сути функция RTC_read_time
	  I2C_StartCondition();               // запуск i2c
	  I2C_SendByte(0b11010000);   // передача адреса устройства, режим записи
	  I2C_SendByte(0x00);            // передача адреса памяти
	  I2C_StopCondition();                // остановка i2c
	  
	  I2C_StartCondition();               // запуск i2c
	  I2C_SendByte(0b11010001);    // передача адреса устройства, режим чтения
	  sec = RTC_ConvertFromDec(I2C_ReadByte());     // чтение секунд, ACK
	  min = RTC_ConvertFromDec(I2C_ReadByte());     // чтение минут, ACK
	  hour = RTC_ConvertFromDec(I2C_ReadByte());    // чтение часов, ACK
	  day = RTC_ConvertFromDec(I2C_ReadByte());    // чтение день недели, ACK
	  date = RTC_ConvertFromDec(I2C_ReadByte());     // чтение число, ACK
	  month = RTC_ConvertFromDec(I2C_ReadByte());   // чтение месяц, ACK
	  year = RTC_ConvertFromDec(I2C_ReadLastByte());    // чтение год, NACK
	  I2C_StopCondition();                // остановка i2c
	  //------------------------------------------------------
	  //контроль уровня яркости
	  if ((adc_value = ADC_convert()) > 240)
	  {
		  BRIGHTNESS = 0.05;
	  }
	  else
	  {
		  BRIGHTNESS = 1;
	  }
	  //------------------------------------------------------
	  temperature = converttemp(dt_check()); //измеряем температуру
	  //------------------------------------------------------
	  //считаем показания будильника
	  alarmhour = EEPROM_read(2);
	  alarmmin = EEPROM_read(3);
	  
	  if ((alarmmin == min) && (alarmhour == hour) && (clock_change_mode == MODE_WITHOUT_CHANGE)) 
	  {
		//добавим условие, чтобы не включить повторно после короткого нажатия кнопки
		if (alarm_flag == 0) alarm_now_mode = 1;  
		alarm_flag = 1;  
	  }
	  else alarm_flag = 0;
	  //------------------------------------------------------
	  if ((Flag ==1)||(Flag ==2)) clockmode = CLOCKMODE_CLOCK;
	  else if ((Flag ==3)||(Flag ==4)) clockmode = CLOCKMODE_DATE;
	  else if (Flag ==5) clockmode = CLOCKMODE_YEAR;
	  else if (Flag ==6) clockmode = CLOCKMODE_WEAKDAY;
	  else if ((Flag ==7)||(Flag ==8)) clockmode = CLOCKMODE_ALARMCLOCK;
	  else
	  {
	  if ((sec<30)||(sec>41)) clockmode = CLOCKMODE_CLOCK;
	  else if (sec<34) clockmode = CLOCKMODE_TEMPERATURE;
	  else if (sec<38) clockmode = CLOCKMODE_DATE;
	  else clockmode = CLOCKMODE_WEAKDAY;
	  }
	  //------------------------------------------------------
	  if ((clock_change_mode != MODE_WITHOUT_CHANGE) && (clock_incr_mode == MODE_INC))
	  {
		  ModifyRTC();
		  clock_incr_mode = MODE_NO_INC;
	  }
	  //------------------------------------------------------
	  if((alarm_flag == 1) && (alarm_now_mode == 1)) alarm();
	  else PORTD |= (1<<PORTD0);//логическая 1
	  //------------------------------------------------------
    }
}

