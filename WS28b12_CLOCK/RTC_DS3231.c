/*
 * RTC.c
 *
 * Created: 16.12.2021 16:08:19
 *  Author: Alex2
 */ 

#include "RTC_DS3231.h"

//����� ������ ��������������
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

unsigned char RTC_ConvertFromBinDec(unsigned char c)//������� � �������� ������
{
	unsigned char ch = ((c/10)<<4)|(c%10);
	return ch;
}
unsigned char RTC_ConvertFromDec(unsigned char c)//������� � ���������� ������
{
	unsigned char ch = ((c>>4)*10+(0b00001111&c));
	return ch;
}
// ������������� ��������� ���������
void RTC_init(void){
	I2C_StartCondition();               // ������ i2c
	I2C_SendByte(0b11010000);   // �������� ������ ����������, ����� ������
	I2C_SendByte(0x0E);            // �������� ������ ������
	I2C_SendByte(0b00100000);      // ��������� �������������� ����������� � ����� �� 1 ��
	I2C_SendByte(0b00001000);      // ��������� ����� 32 ���
	I2C_StopCondition();                // ��������� i2c
}
// ��������� ������� � ����
void RTC_read_time(void){
	I2C_StartCondition();               // ������ i2c
	I2C_SendByte(0b11010000);   // �������� ������ ����������, ����� ������
	I2C_SendByte(0x00);            // �������� ������ ������
	I2C_StopCondition();                // ��������� i2c
	
	I2C_StartCondition();               // ������ i2c
	I2C_SendByte(0b11010001);    // �������� ������ ����������, ����� ������
	sec = RTC_ConvertFromDec(I2C_ReadByte());     // ������ ������, ACK
	min = RTC_ConvertFromDec(I2C_ReadByte());     // ������ �����, ACK
	hour = RTC_ConvertFromDec(I2C_ReadByte());    // ������ �����, ACK
	day = RTC_ConvertFromDec(I2C_ReadByte());    // ������ ���� ������, ACK
	date = RTC_ConvertFromDec(I2C_ReadByte());     // ������ �����, ACK
	month = RTC_ConvertFromDec(I2C_ReadByte());   // ������ �����, ACK
	year = RTC_ConvertFromDec(I2C_ReadLastByte());    // ������ ���, NACK
	I2C_StopCondition();                // ��������� i2c	
}
// ��������� �������
void RTC_write_time(unsigned char hour1,unsigned char min1, unsigned char sec1){
	I2C_StartCondition();               // ������ i2c
	I2C_SendByte(0b11010000);   // �������� ������ ����������, ����� ������
	I2C_SendByte(0x00);            // �������� ������ ������
	I2C_SendByte(RTC_ConvertFromBinDec(sec1));       // 0x00 ������� (������������� �� �������� ��� � �������?)
	I2C_SendByte(RTC_ConvertFromBinDec(min1));       // 0x01 ������
	I2C_SendByte(RTC_ConvertFromBinDec(hour1));      // 0x02 ����
	I2C_StopCondition();                // ��������� i2c
}
// ��������� ����
void RTC_write_date(unsigned char wday, unsigned char day, unsigned char month, unsigned char year){	
	I2C_StartCondition();               // ������ i2c
	I2C_SendByte(0b11010000);   // �������� ������ ����������, ����� ������
	I2C_SendByte(0x03);        // �������� ������ ������
	I2C_SendByte(RTC_ConvertFromBinDec(wday));       // 0x03 ���� ������ (����������� - 1, �� 2, �� 3, �� 4, �� 5, �� 6, �� 7)
	I2C_SendByte(RTC_ConvertFromBinDec(day));        // 0x04 ���� �����
	I2C_SendByte(RTC_ConvertFromBinDec(month));      // 0x05 �����
	I2C_SendByte(RTC_ConvertFromBinDec(year));       // 0x06 ���
	I2C_StopCondition();                // ��������� i2c
}
// ������ �����������
/*
void RTC_read_temper(void){
	I2C_StartCondition();                // ������ i2c
	I2C_SendByte(0b11010000);   // �������� ������ ����������, ����� ������
	I2C_SendByte(0x11);            // �������� ������ ������
	I2C_StopCondition();                // ��������� i2c
	
	I2C_StartCondition();               // ������ i2c
	I2C_SendByte(0b11010001);   // �������� ������ ����������, ����� ������
	t1 = I2C_ReadByte();           // ������ MSB �����������
	t2 = I2C_ReadLastByte();           // ������ LSB �����������
	I2C_StopCondition();                // ��������� i2c
	
	t2=(t2/128);                    // �������� �� 6 - �������� 0,25 (2 ����)
	// �������� �� 7 - �������� 0,5 (1 ���)
	t2=t2*5;
}
*/
void ModifyRTC(void)
{
	I2C_StartCondition();
	I2C_SendByte(0b11010000);
	switch(clock_change_mode)
	{
		case MODE_CHANGE_HOURS: //����
		{
			I2C_SendByte(2);//��������� �� 0x02 � ���� �����
			if (hour < 23) I2C_SendByte(RTC_ConvertFromBinDec( hour+1 ));
			else I2C_SendByte(RTC_ConvertFromBinDec(0));
			break;
		}
		case MODE_CHANGE_MINUTES: // ������
		{
			I2C_SendByte(1);//��������� �� 0x01 � ���� �����
			if(min<59) I2C_SendByte(RTC_ConvertFromBinDec(min+1));
			else I2C_SendByte(RTC_ConvertFromBinDec(0));
			break;
		}
		case MODE_CHANGE_DATE: // ����
		{
			I2C_SendByte(4);//��������� �� 0x04 � ���� ����
			if (month==2) //�������
			{
				if(year%4==0) //���������� ���
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
		case MODE_CHANGE_MONTH: // �����
		{
			I2C_SendByte(5);//��������� �� 0x05 � ���� ������
			if(month<12) I2C_SendByte(RTC_ConvertFromBinDec(month+1));
			else I2C_SendByte(RTC_ConvertFromBinDec(1));
			break;
		}
		case MODE_CHANGE_YEAR: // ���
		{
			I2C_SendByte(6);//��������� �� 0x06 � ���� ����
			if(year<99) I2C_SendByte(RTC_ConvertFromBinDec(year+1));
			else I2C_SendByte(RTC_ConvertFromBinDec(1));
			break;
		}
		case MODE_CHANGE_WEAKDAY: // ���� ������
		{
			I2C_SendByte(3);//��������� �� 0x03 � ���� ��� ������
			if(day<7) I2C_SendByte(RTC_ConvertFromBinDec(day+1));
			else I2C_SendByte(RTC_ConvertFromBinDec(1));
			break;
		}
		case MODE_CHANGE_ALARM_HOURS: // ���� ����������
		{
			if(alarmhour<23) EEPROM_write(2,alarmhour+1);
			else EEPROM_write(2,0);
			break;
		}
		case MODE_CHANGE_ALARM_MINUTES: // ������ ����������
		{
			if(alarmmin<59) EEPROM_write(3,alarmmin+1);
			else EEPROM_write(3,0);
			break;
		}
	}
	I2C_StopCondition();
}