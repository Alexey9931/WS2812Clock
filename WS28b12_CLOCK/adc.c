/*
 * adc.c
 *
 * Created: 19.11.2021 17:07:06
 *  Author: Alex2
 */ 
#include "adc.h"
//�������������-
void ADC_Init(void)
{
	ADCSRA |= (1<<ADEN);//��� ���
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//���������� 64���
	ADMUX |= (1<<REFS1)|(1<<REFS0);//����� ��� ����� ������
}
unsigned int ADC_convert (void)
{
	ADCSRA |= (1<<ADSC);//������ ���������
	while((ADCSRA&(1<<ADSC)));
	return (unsigned int) ADC;
}
