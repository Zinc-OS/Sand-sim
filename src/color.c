#include "color.h"
#include "frame.h"
#include <stdint.h>

uint32_t color = 0xffffffff;
uint32_t mix(uint32_t a, uint32_t b, int m, int max){
	uint32_t out = 0;
	out+=(a&0xff)*(max-m)/max + (b&0xff)*m/max;
	
	out+=(((((a&0x0000ff00)>>8)*(max-m)/max)&0xff)<<8);
	out+=(((((b&0x0000ff00)>>8)*(m)/max)&0xff)<<8);

	out+=(((((a&0x00ff0000)>>16)*(max-m)/max)&0xff)<<16);
	out+=(((((b&0x00ff0000)>>16)*(m)/max)&0xff)<<16);

	out+=(((((a&0xff000000)>>24)*(max-m)/max)&0xff)<<24);
	out+=(((((b&0xff000000)>>24)*(m)/max)&0xff)<<24);

	return out;
}

uint32_t hue1(int frame, int time){
	int x = frame%(time*3);
	if(x<time){
		return mix(0xffff00ff, 0xff00ffff, x, time);
	} else if(x<time*2){
		return mix(0xffffff00, 0xffff00ff, x-time, time);
	} else{
		return mix(0xff00ffff, 0xffffff00, x-time*2, time);
	}
}

uint32_t hue2(int frame, int time){
	int x = frame%(time*6);
	if(x<time){
		return mix(0xff0000ff, 0xff00ffff, x, time);
	} else if(x<time*2){
		return mix(0xff00ffff, 0xff00ff00, x-time, time);
	} else if(x<time*3){
		return mix(0xff00ff00, 0xffffff00, x-time*2, time);
	} else if(x<time*4){
		return mix(0xffffff00, 0xffff0000, x-time*3, time);
	} else if(x<time*5){
		return mix(0xffff0000, 0xffff00ff, x-time*4, time);
	} else {
		return mix(0xffff00ff, 0xff0000ff, x-time*5, time);
	}


}

uint32_t bw(int frame, int time){
	int x = frame%(time*2);
	if(x<time){
		return mix(0xffffffff, 0xff000000, x, time);
	} else{
		return mix(0xff000000, 0xffffffff, x-time, time);
	}
}

void updateColor(){
	//color = hue1(frame, 100);
	color = hue2(frame, 100);
	//color = bw(frame, 100);
}	
