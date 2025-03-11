/*
This file is part of Sand Sim.

Sand Sim is free software: you can redistribute it and/or modify it under the terms
 of the GNU General Public License as published by the Free Software Foundation, either
 version 3 of the License, or (at your option) any later version.

Sand Sim is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with RomanNumerals. 
If not, see <https://www.gnu.org/licenses/>.*/


#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
struct{
	int right;
	int left;
	int middle;
	int x;
	int y;
} mouse = {0,0,0,0,0};

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;
typedef struct{
	uint32_t color;
	uint32_t velocity;
	int on;
	int touched;
} buffer;
buffer* buff;
uint32_t* surf;
int rnng;
SDL_Event E;
int cursorsize = 4;
int width = 512, height = 512;
uint32_t color = 0xffffffff;
long frame=0;

void cleanUp(){
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	free(buff);
	SDL_Quit();
}

void getInputs(){
	while(SDL_PollEvent(&E)){
		switch(E.type){
			case SDL_QUIT:
				rnng=0;
				break;
			case SDL_MOUSEMOTION:
				mouse.x = E.motion.x;
				mouse.y = E.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch(E.button.button){
					case SDL_BUTTON_RIGHT:
						mouse.right = 1;
						break;
					case SDL_BUTTON_MIDDLE:
						mouse.middle = 1;
						break;
					case SDL_BUTTON_LEFT:
						mouse.left = 1;
						break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch(E.button.button){
					case SDL_BUTTON_RIGHT:
						mouse.right = 0;
						break;
					case SDL_BUTTON_MIDDLE:
						mouse.middle = 0;
						break;
					case SDL_BUTTON_LEFT:
						mouse.left = 0;
						break;
				}
				break;
			case SDL_WINDOWEVENT:
				if(E.window.event==SDL_WINDOWEVENT_RESIZED){
					int oldheight = height;
					int oldwidth = width;
					width = E.window.data1;
					height = E.window.data2;
					buffer* buff2=malloc(sizeof(buffer)*width*height);
					surf = realloc(surf, width*height*sizeof(uint32_t));
					for(int i=0;i<oldheight;i++){
						for(int j=0;j<oldwidth;j++){
							if(i*width+j<width*height&&i*width+j>=0)
								memcpy(buff2+i*width+j, buff+i*oldwidth+j, sizeof(buffer));;
						}
					}
					free(buff);
					buff = buff2;
					SDL_DestroyTexture(texture);
					texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);

				}
				break;
			case SDL_MOUSEWHEEL:
				cursorsize+=E.wheel.y;
				break;
		}
	}
}

void updateSand(){
	for(int i=0;i<width*height;i++){
		buff[i].touched=0;
	}
	if(mouse.left){
		int mid = mouse.x+mouse.y*width;
		for(int i=-cursorsize;i<=cursorsize;i++){
			for(int j=-cursorsize;j<=cursorsize;j++){
				const int pos=mid+i+j*width;
				if(pos<0||pos>width*height)
					continue;
				buff[pos].on=1;
				buff[pos].color=color;
			}
		}
	}
	int dit=0;
	for(int i=(height-1)*width;i>width;i-=width){
		for(int j=i;j<width+i;j++){
			if(!buff[j].on&&buff[j-width].on){
				buff[j].on=1;
				buff[j].color=buff[j-width].color;
				buff[j-width].on=0;
			} else if(buff[j-width].on&&!buff[j-1].on&&dit){
				buff[j-1].color=buff[j-width].color;
				buff[j-1].on=1;
				buff[j-width].on=0;
				j++;
				dit=0;
			} else if(buff[j-width].on&&!buff[j+1].on&&!dit){
				buff[j+1].color=buff[j-width].color;
				buff[j+1].on=1;
				buff[j-width].on=0;
				j++;
				dit=1;
			}
		}
	}
}

void updateCursor(){
	SDL_Rect cursor={mouse.x-cursorsize, mouse.y-cursorsize, cursorsize*2+1, cursorsize*2+1};
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(renderer, (color&0xff0000)>>16, (color&0xff00)>>8, color&0xff, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &cursor);
}

uint32_t hue(int frame, int time, uint8_t value){
	int x = frame%(time*3);
	uint32_t color=0x00;
	if(x<time){
		int c1 = 0xff*(time-x)/time;
		int c2 = 0xff*x/time;
		color+=c1<<16;
		color+=c2<<8;
		color+=value;
	}else if(x<time*2){
		x-=time;
		int c1 = 0xff*(time-x)/time;
		int c2 = 0xff*x/time;
		color+=c1<<8;
		color+=c2;
		color+=value<<16;
	}else{
		x-=time*2;
		int c1 = 0xff*(time-x)/time;
		int c2 = 0xff*x/time;
		color+=c1;
		color+=c2<<16;
		color+=value<<8;
	}
	return color;
}

uint32_t bw(int frame, int time){
	int x = frame%(time*2);
	int c=0x00;
	if(x<time){
		c = 0xff*x/time;
	}else{
		x-=time;
		c = 0xff*(time-x)/time;
	}
	c+=0x11;
	c&=0xff;
	return (c<<16)+(c<<8)+c;
}

void updateColor(){
	color = hue(frame, 100, 0xff);
	//color = bw(frame, 100);
}	


int main(int argc, char* argv[]){
	
	if(SDL_Init(SDL_INIT_EVERYTHING))
	{
		printf("Error %s", SDL_GetError());
	}
	window = SDL_CreateWindow("Sand", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_RESIZABLE|SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
	SDL_ShowCursor(0);
	rnng = 1;
	buff = malloc(width*height*sizeof(*buff));
	for(int i=0;i<width*height;i++){
		buff[i].on=0;
	}
	
	surf=malloc(width*height*sizeof(uint32_t));
	
	while(rnng){
		uint32_t startTime=SDL_GetTicks();
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
		getInputs();
		updateColor();
		updateSand();
		
		for(int i=0;i<width*height;i++){
			surf[i]=0xff111222;
			if(buff[i].on)
				surf[i]=buff[i].color;
		}
		
		SDL_UpdateTexture(texture, NULL, surf, width*sizeof(uint32_t));
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		
		updateCursor();
        
		SDL_RenderPresent(renderer);
		
		uint32_t delayTime=(SDL_GetTicks()-startTime);
		if(delayTime>16)
			delayTime=16;
		delayTime=16-delayTime;
		if(delayTime)
			SDL_Delay(delayTime);
		frame++;
	}
	
	cleanUp();
	return 0;
}
