#ifndef COLOR_H
#define COLOR_Hi
#include <stdint.h>
extern uint32_t color;
uint32_t mix(uint32_t a, uint32_t b, int m, int max);
uint32_t hue1(int frame, int time);
uint32_t hue2(int frame, int time);
uint32_t bw(int frame, int time);
void updateColor(void);
#endif