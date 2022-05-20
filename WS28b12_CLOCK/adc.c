/*
 * adc.c
 *
 * Created: 19.11.2021 17:07:06
 *  Author: Alex2
 */ 
#include "adc.h"
//覧覧覧覧覧覧�-
void ADC_Init(void)
{
	ADCSRA |= (1<<ADEN);//粲� ﾀﾖﾏ
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//���瑙粽齏� 64�ﾃﾖ
	ADMUX |= (1<<REFS1)|(1<<REFS0);//粹��� 頌� ����� �瑜���
}
unsigned int ADC_convert (void)
{
	ADCSRA |= (1<<ADSC);//�璞琿� 韈�褞褊韜
	while((ADCSRA&(1<<ADSC)));
	return (unsigned int) ADC;
}
