/*
 * DS18B20.h
 *
 * Created: 02.01.2022 21:07:11
 *  Author: Alex2
 */ 


#ifndef DS18B20_H_
#define DS18B20_H_

#include "main.h"
#define PORTTEMP PORTD
#define DDRTEMP DDRD
#define PINTEMP PIND
#define BITTEMP 7
#define NOID 0xCC //���������� �������������
#define T_CONVERT 0x44 //��� ��������� �����������
#define READ_DATA 0xBE //�������� ������ ��������

int dt_check(void); //������� �������������� ��������� ������� � �����������
char dt_testdevice(void); //dt � digital termomether | ���������, ���� �� ���������� �� ����
unsigned char dt_readbyte(void);//������� ������ ����� � ����������
char dt_readbit(void);//������� ������ ���� � ����������
void dt_sendbyte(unsigned char bt);//������� ������ ����� �� ����������
void dt_sendbit(char bt);//������� ������ ���� �� ����������
char converttemp (unsigned int tt);//�������������� ����������� � �������

#endif /* DS18B20_H_ */