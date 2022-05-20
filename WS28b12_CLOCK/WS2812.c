/*
 * WS2812.c
 *
 * Created: 01.12.2021 16:57:36
 *  Author: Alex2
 */ 
#include "main.h"
int k=0;
int m;
extern double BRIGHTNESS;
extern int counter_of_leds;
extern int red_mode12, green_mode12;
extern int red_mode13, green_mode13, blue_mode13;

void send_0(void)
{
	PORTB |= (1<<(PORTB3));
	//_delay_us(0.45);
	asm("nop");asm("nop");
	PORTB &= ~(1<<(PORTB3));
    asm("nop");asm("nop");asm("nop");asm("nop");
	//_delay_us(0.8);
}
void send_1(void)
{
	PORTB |= (1<<(PORTB3));
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	PORTB &= ~(1<<(PORTB3));
	//_delay_us(0.8);
	asm("nop");
	//_delay_us(0.45);
}
void reset(void)
{
	PORTB &= ~(1<<(PORTB3));
	_delay_us(60);
}
void send_byte_to_1LED(int byte)
{
	int bit[8];
	int k;
	for ( k = 0; k < 8; k++)
	{
		bit[k] = byte&0b1;
		byte = byte>>1;
	}
	for ( k = 7; k >= 0; k--)
	{
		switch ( bit[k] )
		{
			case 1: send_1(); break;
			case 0: send_0(); break;
		}	
	}
}
int convert_from_dec_to_bin(int col)
{
	int bin = 0;
	int reminder=0, n = 1;
	while (col!=0) 
	{
	    reminder = col%2;
		col /=2;
		bin += reminder*n;
		n *= 10;
	}
	return bin;
}
void send_color_to_1LED(int mode, int m) //r g b от 0 до 255
{
	int green, red, blue;
	//green = convert_from_dec_to_bin(green);
	//red = convert_from_dec_to_bin(red);
	//blue = convert_from_dec_to_bin(blue);
    switch(mode)
	{
		case 0:{ green = 0; red = 0; blue = 0; break;}
		case 1:{ green = (float)(255*BRIGHTNESS); red = 0; blue = 0; break;}
		case 2:{ green = 0; red = (float)(255*BRIGHTNESS); blue = 0; break;}
		case 3:{ green = 0; red = 0; blue = (float)(255*BRIGHTNESS); break;}
		case 4:{ green = (float)(255*BRIGHTNESS); red = (float)(255*BRIGHTNESS); blue = (float)(255*BRIGHTNESS); break;}
		case 5:{ green = (float)(255*BRIGHTNESS); red = (float)(255*BRIGHTNESS); blue = 0; break;}
		case 6:{ green = 0; red = (float)(165*BRIGHTNESS); blue = (float)(165*BRIGHTNESS); break;}
		case 7:{ green = (float)(127*BRIGHTNESS); red = (float)(255*BRIGHTNESS); blue = 0; break;}
		case 8:{ green = (float)(125*BRIGHTNESS); red = 0; blue = (float)(158*BRIGHTNESS); break;}	
		case 11:{ green = m/2; red = 255/2; blue = 0; break;}// в пол яркости все время для данного режима
		case 12:{ green = green_mode12/2; red = red_mode12/2; blue = 70/2; break;}// в пол яркости все время для данного режима
		case 13:{ green = green_mode13; red = red_mode13; blue = blue_mode13; break;}// в пол яркости все время для данного режима
	}
	send_byte_to_1LED(green);
	send_byte_to_1LED(red);
	send_byte_to_1LED(blue);
	PORTB &= ~(1<<(PORTB3));
}
/*void turn_on_diap_of_LEDS(int start1, int finish1, int start2, int finish2, int start3, int finish3)
{
	int m;
	for(m = 1; m <= 59; m++)
	{
		if ((m<start1)||(m>finish1))//не горят
		{
			send_color_to_1LED(0, 0, 0);
		}
		if ((m>=start)&&(m<=finish))//горят
		{
			send_color_to_1LED(255, 255, 255);
		}
	}
}
*/
int mode9;
int mode10=1;
void turn_on_diap_of_LEDS(int* massiv, int mode)
{
	k=0;
	for(m = 1; m <= 59; m++)
	{
		if (m == massiv[k])//горят
		{
			if (mode == DIFFERENT_colours)//режим разноцветных светодиодов
			{
				mode9 += 1;
				if (mode9==9)
				{
					mode9=1;
				}
				send_color_to_1LED(mode9, m);//r g b от 0 до 255
			}
			else if (mode == COLOUR_FIGURES)//режим цифр разного цвета
			{
				if ((m>0)&&(m<15))//1ая цифра
				{
					send_color_to_1LED(1, m);
				}
				if ((m>14)&&(m<29))//2ая цифра
				{
					send_color_to_1LED(5, m);
				}
				if ((m>28)&&(m<32))//точки
				{
					send_color_to_1LED(2, m);
				}
				if ((m>31)&&(m<46))//3я цифра
				{
					send_color_to_1LED(3, m);
				}
				if ((m>45)&&(m<60))//4ая цифра
				{
					send_color_to_1LED(6, m);
				}		
			}
			else if ((mode == MODE11)||(mode == MODE12)||(mode == RADUGA))
			{
				if ((m>0)&&(m<15))//1ая цифра
				{
					send_color_to_1LED(mode, counter_of_leds);
				}
				if ((m>14)&&(m<29))//2ая цифра
				{
					send_color_to_1LED(mode, counter_of_leds);
				}
				if ((m>28)&&(m<32))//точки
				{
					send_color_to_1LED(mode, counter_of_leds);
				}
				if ((m>31)&&(m<46))//3я цифра
				{
					send_color_to_1LED(mode, counter_of_leds);
				}
				if ((m>45)&&(m<60))//4ая цифра
				{
					send_color_to_1LED(mode, counter_of_leds);
				}
			}
			else//режим одного цвета
			{
			    send_color_to_1LED(mode, m);//r g b от 0 до 255
			}
			k++;
		}
		else//не горят
		{
			send_color_to_1LED(NONE, m);//r g b от 0 до 255
		}
	}
}


