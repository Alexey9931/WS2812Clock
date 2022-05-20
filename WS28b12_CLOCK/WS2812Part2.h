/*
 * WS2812Part2.h
 *
 * Created: 09.12.2021 14:45:36
 *  Author: Alex2
 */ 


#ifndef WS2812PART2_H_
#define WS2812PART2_H_

int* convert_massiv(int* array,int difference,int size);
int* chose_massivR1(int R);
int* chose_massivR2(int R);
int* replace_massiv(int* array,int distance);
int chose_size(int R);
void print_all_number(int number, int mode);
void recover_massiv(int* array,int difference,int size);


#endif /* WS2812PART2_H_ */