/*
This file is part of Sand Sim.

Sand Sim is free software: you can redistribute it and/or modify it under the terms
 of the GNU General Public License as published by the Free Software Foundation, either
 version 3 of the License, or (at your option) any later version.

Sand Sim is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Sand sim. 
If not, see <https://www.gnu.org/licenses/>.*/

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