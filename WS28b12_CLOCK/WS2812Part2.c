/*
 * WS2812Part2.c
 *
 * Created: 09.12.2021 14:45:28
 *  Author: Alex2
 */ 
#include "main.h"
#include "WS2812.h"

extern int sec;
extern int timer1_count;
extern int clockmode;
extern unsigned char clock_change_mode;

int  massivR1_0[12] = {46,47,48,49,50,51,52,53,54,55,56,57};//0
int  massivR1_1[4] = {50,51,52,53};//1
int  massivR1_2[10] = {46,47,48,49,52,53,54,55,58,59};//2
int  massivR1_3[10] = {48,49,50,51,52,53,54,55,58,59};//3
int  massivR1_4[8] = {50,51,52,53,56,57,58,59};//4
int  massivR1_5[10] = {48,49,50,51,54,55,56,57,58,59};//5	
int  massivR1_6[12] = {46,47,48,49,50,51,54,55,56,57,58,59};//6
int  massivR1_7[6] = {50,51,52,53,54,55};//7
int  massivR1_8[14] = {46,47,48,49,50,51,52,53,54,55,56,57,58,59};//8
int  massivR1_9[12] = {48,49,50,51,52,53,54,55,56,57,58,59};//9	
	
int  massivR2_0[12] = {34,35,36,37,38,39,40,41,42,43,44,45};//0
int  massivR2_1[4] = {34,35,44,45};//1
int  massivR2_2[10] = {32,33,34,35,36,37,40,41,42,43};//2
int  massivR2_3[10] = {32,33,34,35,36,37,42,43,44,45};//3
int  massivR2_4[8] = {32,33,34,35,38,39,44,45};//4
int  massivR2_5[10] = {32,33,36,37,38,39,42,43,44,45};//5
int  massivR2_6[12] = {32,33,36,37,38,39,40,41,42,43,44,45};//6
int  massivR2_7[6] = {34,35,36,37,44,45};//7
int  massivR2_8[14] = {32,33,34,35,36,37,38,39,40,41,42,43,44,45};//8
int  massivR2_9[12] = {32,33,34,35,36,37,38,39,42,43,44,45};//9
	
int massiv_points[3] = {29,30,31};//points
int massiv_one_point[1] = {29};
int massiv_C[8] = {46,47,48,49,54,55,56,57};
int massiv_1st_tire[2] = {27,28};
int massiv_2nd_tire[2] = {58,59};		

int* convert_massiv(int* array,int difference,int size)
{
	int i;
	for(i=0;i<size;i++)
	{
		array[i]=array[i]-difference;
	}
	return array;
}
void recover_massiv(int* array,int difference,int size)
{
	int i;
	for(i=0;i<size;i++)
	{
		array[i]=array[i]+difference;
	}
}

int* chose_massivR1(int R)
{
	switch(R)
	{
		case 0: return massivR1_0; break;
		case 1: return massivR1_1; break;
		case 2: return massivR1_2; break;
		case 3: return massivR1_3; break;
        case 4: return massivR1_4; break;
		case 5: return massivR1_5; break;
		case 6: return massivR1_6; break;
		case 7: return massivR1_7; break;
		case 8: return massivR1_8; break;
		case 9: return massivR1_9; break;
	}
}	
int* chose_massivR2(int R)
{
	switch(R)
	{
		case 0: return massivR2_0; break;
		case 1: return massivR2_1; break;
		case 2: return massivR2_2; break;
		case 3: return massivR2_3; break;
		case 4: return massivR2_4; break;
		case 5: return massivR2_5; break;
		case 6: return massivR2_6; break;
		case 7: return massivR2_7; break;
		case 8: return massivR2_8; break;
		case 9: return massivR2_9; break;
	}
}
int* replace_massiv(int* array,int distance)
{
	int i=0;
	for(i=0;i<distance;i++)
	{
		array[i+distance] = array[i];
		array[i]=0;
	}
}
int chose_size(int R)
{
	switch(R)
	{
		case 0: return 12; break;
		case 1: return 4; break;
		case 2: return 10; break;
		case 3: return 10; break;
		case 4: return 8; break;
		case 5: return 10; break;
		case 6: return 12; break;
		case 7: return 6; break;
		case 8: return 14; break;
		case 9: return 12; break;
	}
}	
void print_all_number(int number, int mode)
{
	int mas_of_leds[59];
	int i,m;
	int* mas0;
	int* mas1;
	int* mas2;
	int* mas3;
	int k,r;
	int sizeR3,sizeR4,sizeR1,sizeR2;
	int R1=0, R2=0, R3=0, R4=0;
	
	R1 = number%10;
	R2 = (number/10)%10;
	R3 = (number/100)%10;
	R4 = number/1000;
//-------------------------------------------------------------------------------------------------------------------------------------------
//вывод разряда	R4
	mas0 = convert_massiv(chose_massivR2(R4),31,chose_size(R4));//записываем в конечный массив старший разряд R4 через разницу от R2
	k=0;
	if ( ((clock_change_mode == MODE_CHANGE_HOURS)||(clock_change_mode == MODE_CHANGE_ALARM_HOURS)||(clock_change_mode == MODE_CHANGE_DATE)||(clock_change_mode == MODE_CHANGE_YEAR)) && (timer1_count == 0))
	{
		sizeR4 = 0;
	}
	else
	{
	 if (clockmode == CLOCKMODE_WEAKDAY)
	 {
	  sizeR4 = 0;
	 }
	 else
	 {
	  sizeR4 = chose_size(R4);
      for (i=0;i<sizeR4;i++)
      {
	  mas_of_leds[i] = mas0[k];
	  k++;
      }
	 }
	}
	recover_massiv(chose_massivR2(R4),31,chose_size(R4));
//-------------------------------------------------------------------------------------------------------------------------------------------
//вывод разряда	R3	
    k=0;
	mas1 = convert_massiv(chose_massivR1(R3),31,chose_size(R3));
	if ( ((clock_change_mode == MODE_CHANGE_HOURS)||(clock_change_mode == MODE_CHANGE_ALARM_HOURS)||(clock_change_mode == MODE_CHANGE_DATE)||(clock_change_mode == MODE_CHANGE_YEAR)||(clock_change_mode == MODE_CHANGE_WEAKDAY)) && (timer1_count == 0))
	{
		sizeR3 = 0;
	}
	else
	{
	 if (clockmode == CLOCKMODE_WEAKDAY)
	 {
	   sizeR3 = 2;	
	   for (i=sizeR4;i<sizeR4+sizeR3;i++)
	    {
		 mas_of_leds[i] = massiv_1st_tire[k];
		 k++;
	    }
	 }
	 else
	 {
	  sizeR3 = chose_size(R3);
	  for (i=sizeR4;i<sizeR4+sizeR3;i++)
	  {
	 	 mas_of_leds[i] = mas1[k];
		 k++;
	  }
	 }
	}
	recover_massiv(chose_massivR1(R3),31,chose_size(R3));
//-------------------------------------------------------------------------------------------------------------------------------------------
//вывод разряда	с точками
    k=0;
	if ( ((clockmode == CLOCKMODE_CLOCK) || (clockmode == CLOCKMODE_ALARMCLOCK)) && ((sec%2) == 0) )
	{
		r=3;
		for (i=sizeR4+sizeR3;i<sizeR4+sizeR3+r;i++)
		{
			mas_of_leds[i] = massiv_points[k];
			k++;
		}
	}
	else if ((clockmode == CLOCKMODE_TEMPERATURE) || (clockmode == CLOCKMODE_DATE))
	{
		r=1;
		for (i=sizeR4+sizeR3;i<sizeR4+sizeR3+r;i++)
		{
			mas_of_leds[i] = massiv_one_point[k];
			k++;
		}
	}
	else
	{
		r=0;
	}
//-------------------------------------------------------------------------------------------------------------------------------------------
//вывод разряда	R2	
	mas2 = chose_massivR2(R2);
	k=0;
	if ( ((clock_change_mode == MODE_CHANGE_MINUTES)||(clock_change_mode == MODE_CHANGE_ALARM_MINUTES)||(clock_change_mode == MODE_CHANGE_MONTH)||(clock_change_mode == MODE_CHANGE_YEAR)||(clock_change_mode == MODE_CHANGE_WEAKDAY)) && (timer1_count == 0))
	{
	 sizeR2 = 0;
	}
	else
	{
	 sizeR2 = chose_size(R2);
	 for (i=sizeR4+sizeR3+r;i<sizeR4+sizeR3+sizeR2+r;i++)
	 {
		mas_of_leds[i] = mas2[k];
		k++;
	 }
	}
//-------------------------------------------------------------------------------------------------------------------------------------------
//вывод разряда	R1	
	mas3 = chose_massivR1(R1);
	k=0;
	if ( ((clock_change_mode == MODE_CHANGE_MINUTES)||(clock_change_mode == MODE_CHANGE_ALARM_MINUTES)||(clock_change_mode == MODE_CHANGE_MONTH)||(clock_change_mode == MODE_CHANGE_YEAR)||(clock_change_mode == MODE_CHANGE_WEAKDAY)) && (timer1_count == 0))
	{
		sizeR1 = 0;
	}
	else
	{
	 if (clockmode == CLOCKMODE_TEMPERATURE)
	 {
	  sizeR1 = 8;
	  for (i=sizeR4+sizeR3+sizeR2+r;i<sizeR4+sizeR3+sizeR2+sizeR1+r;i++)
	   {
		 mas_of_leds[i] = massiv_C[k];
		 k++;
	   }
	 }
	 else if (clockmode == CLOCKMODE_WEAKDAY)
	 {
	   sizeR1 = sizeR3;
	   for (i=sizeR4+sizeR3+sizeR2+r;i<sizeR4+sizeR3+sizeR2+sizeR1+r;i++)
	   {
		   mas_of_leds[i] = massiv_2nd_tire[k];
		   k++;
	   }
	 }
	 else
	 {
	  sizeR1 = chose_size(R1);
	  for (i=sizeR4+sizeR3+sizeR2+r;i<sizeR4+sizeR3+sizeR2+sizeR1+r;i++)
	  {
	 	 mas_of_leds[i] = mas3[k];
	   	 k++;
	  }
	 }
	}
//-------------------------------------------------------------------------------------------------------------------------------------------
//обнуление оставшихся элементов массива
    for(m=sizeR4+sizeR3+sizeR2+sizeR1+r;m<59;m++)
    {
	mas_of_leds[m] = 0;
    }
	turn_on_diap_of_LEDS(mas_of_leds, mode);
}
