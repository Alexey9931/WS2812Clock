/*
 * RTC.c
 *
 * Created: 16.12.2021 16:08:19
 *  Author: Alex2
 */ 
//выбор режима редактировани€
#define MODE_WITHOUT_CHANGE 0
#define MODE_CHANGE_HOURS 1
#define MODE_CHANGE_MINUTES 2
#define MODE_CHANGE_DATE 3
#define MODE_CHANGE_MONTH 4
#define MODE_CHANGE_YEAR 5
#define MODE_CHANGE_WEAKDAY 6
#define MODE_CHANGE_ALARM_HOURS 7
#define MODE_CHANGE_ALARM_MINUTES 8

extern unsigned char clock_change_mode;
extern unsigned char sec,min,hour,day,date,month,year,alarmhour,alarmmin;

unsigned char RTC_ConvertFromBinDec(unsigned char c)//перевод в двоичный формат
{
	unsigned char ch = ((c/10)<<4)|(c%10);
	return ch;
}
unsigned char RTC_ConvertFromDec(unsigned char c)//перевод в дес€тичный формат
{
	unsigned char ch = ((c>>4)*10+(0b00001111&c));
	return ch;
}
void ModifyRTC(void)
{
	I2C_StartCondition();
	I2C_SendByte(0b11010000);
	switch(clock_change_mode)
	{
		case MODE_CHANGE_HOURS: //часы
		{
		 I2C_SendByte(2);//ѕереходим на 0x02 Ч байт часов
		 if (hour < 23) I2C_SendByte(RTC_ConvertFromBinDec( hour+1 ));
		 else I2C_SendByte(RTC_ConvertFromBinDec(0));
		 break;
		}
		case MODE_CHANGE_MINUTES: // минуты
        {
		 I2C_SendByte(1);//ѕереходим на 0x01 Ч байт минут
		 if(min<59) I2C_SendByte(RTC_ConvertFromBinDec(min+1));
		 else I2C_SendByte(RTC_ConvertFromBinDec(0));	
		 break;
		}
		case MODE_CHANGE_DATE: // дата
		{
			I2C_SendByte(4);//ѕереходим на 0x04 Ч байт даты
			if (month==2) //февраль
			{
				if(year%4==0) //високосный год
				{
					if(date<29) I2C_SendByte(RTC_ConvertFromBinDec(date+1));
					else I2C_SendByte(RTC_ConvertFromBinDec(1));
				}
				else
				{
					if(date<28) I2C_SendByte(RTC_ConvertFromBinDec(date+1));
					else I2C_SendByte(RTC_ConvertFromBinDec(1));
				}
			}
			else if ((month==4)|(month==6)|(month==9)|(month==11))
			{
				if(date<30) I2C_SendByte(RTC_ConvertFromBinDec(date+1));
				else I2C_SendByte(RTC_ConvertFromBinDec(1));
			}
			else
			{
				if(date<31) I2C_SendByte(RTC_ConvertFromBinDec(date+1));
				else I2C_SendByte(RTC_ConvertFromBinDec(1));
			}
			break;
		}
		case MODE_CHANGE_MONTH: // мес€ц
		{
			I2C_SendByte(5);//ѕереходим на 0x05 Ч байт мес€ца
			if(month<12) I2C_SendByte(RTC_ConvertFromBinDec(month+1));
			else I2C_SendByte(RTC_ConvertFromBinDec(1));
			break;
		}
		case MODE_CHANGE_YEAR: // год
		{
			I2C_SendByte(6);//ѕереходим на 0x06 Ч байт года
			if(year<99) I2C_SendByte(RTC_ConvertFromBinDec(year+1));
			else I2C_SendByte(RTC_ConvertFromBinDec(1));
			break;
		}
		case MODE_CHANGE_WEAKDAY: // день недели
		{
			I2C_SendByte(3);//ѕереходим на 0x03 Ч байт дн€ недели
			if(day<7) I2C_SendByte(RTC_ConvertFromBinDec(day+1));
			else I2C_SendByte(RTC_ConvertFromBinDec(1));
			break;
		}
		case MODE_CHANGE_ALARM_HOURS: // часы будильника
        {
		    if(alarmhour<23) EEPROM_write(2,alarmhour+1);
		    else EEPROM_write(2,0);
		    break;
		}
		case MODE_CHANGE_ALARM_MINUTES: // минуты будильника
        {
		    if(alarmmin<59) EEPROM_write(3,alarmmin+1);
		    else EEPROM_write(3,0);
		    break;
		}
	}
	I2C_StopCondition();
}