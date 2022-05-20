/*
 * RTC.c
 *
 * Created: 16.12.2021 16:08:19
 *  Author: Alex2
 */ 
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