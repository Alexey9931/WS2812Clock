/*
 * WS2812.h
 *
 * Created: 01.12.2021 16:57:49
 *  Author: Alex2
 */ 


#ifndef WS2812_H_
#define WS2812_H_

void send_0(void);
void send_1(void);
void reset(void);
void send_byte_to_1LED(int color);
void send_color_to_1LED(int colour, int m);
int convert_from_dec_to_bin(int col);
void turn_on_diap_of_LEDS(int* massiv, int mode);
int* randomize_colour();


#endif /* WS2812_H_ */