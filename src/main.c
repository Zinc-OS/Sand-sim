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

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "color.h"
#include <string.h>
#include <letterbox.h>

struct{
	int right;
	int left;
	int middle;
	int x;
	int y;
	int oldx;
	int oldy;
} mouse = {0,0,0,0,0,0,0};

int ctrl=0;
int bottomBar=128;
int useImg;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;
SDL_Surface* imgSurf;
SDL_Texture* imgTex;

typedef enum { air, sand, rock} objType;
typedef struct{
	uint32_t color;
	uint32_t velocity;
	objType type;
} buffer;
buffer* buff=NULL;

uint32_t* surf;
int rnng;
SDL_Event E;
int cursorsize = 3;
int width = 512, height = 512;
long frame=0;

void cleanUp(){
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	free(buff);
	free(surf);
	SDL_Quit();
}

void saveImg(char *name){
	FILE* out=fopen(name, "w");
	fprintf(out, "P3\n");
	fprintf(out, "%d %d\n255\n", width, height);
	for(int i=0;i<width*height;i++){
		uint32_t color=surf[i];
		fprintf(out, "%d\t%d\t%d\n", (color&0xff0000)>>16, (color&0xff00)>>8, (color&0xff));
	}
	fclose(out);
	printf("saved %s\n", name);
}

void setUpImg(char* file){
	printf("file:\"%s\"\n", file);
	fflush(stdout);
	imgSurf=IMG_Load(file);
	if(imgSurf==NULL){
		useImg=0;
		return;
	} else {
		letterbox(&imgSurf);
		imgTex=SDL_CreateTextureFromSurface(renderer, imgSurf);
		useImg=1;
	}
}

void getInputs(){
	while(SDL_PollEvent(&E)){
		switch(E.type){
			case SDL_QUIT:
				rnng=0;
				break;
			case SDL_MOUSEMOTION:
				mouse.oldx=mouse.x;
				mouse.oldy=mouse.y;
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
			case SDL_KEYDOWN:{
				SDL_Keycode sym=E.key.keysym.sym;
				if(sym==SDLK_LCTRL||sym==SDLK_RCTRL)
					ctrl=1;
			}
				break;
			case SDL_KEYUP:{
				SDL_Keycode sym=E.key.keysym.sym;
				if(sym==SDLK_LCTRL||sym==SDLK_RCTRL)
					ctrl=0;
				if(sym==SDLK_s&&ctrl==1){
					saveImg("test.ppm");
				}
				
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
					height = E.window.data2-bottomBar;
					buffer* buff2=malloc(width*height*sizeof(buffer));
					memset(buff2, 0x00, width*height*sizeof(buffer));
					surf = realloc(surf, width*height*sizeof(uint32_t));
					for(int i=0;i<oldheight;i++){
						for(int j=0;j<oldwidth;j++){
							if(i*width+j<width*height&&i*width+j>=0)
								memcpy(buff2+i*width+j, buff+i*oldwidth+j, sizeof(buffer));
						}
					}
					free(buff);
					buff = buff2;
					SDL_DestroyTexture(texture);
					texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);

				}
				break;
			case SDL_MOUSEWHEEL:
				if(cursorsize>0||E.wheel.y>0)
					cursorsize+=E.wheel.y;
				break;
		}
	}
}

int getAlpha(uint32_t col){
	return (col>>24)&0xff;
}

void mouseDraw(objType type){
	if(cursorsize<1){
		cursorsize=1;
	}
	int num=2+(cursorsize*2+abs(mouse.x-mouse.oldx)+abs(mouse.y-mouse.oldy))/(cursorsize*2+1);
	for(int i=0;i<num;i++){
		int x=mouse.oldx*i/num+mouse.x*(num-i)/num;
		int y=mouse.oldy*i/num+mouse.y*(num-i)/num;
		int mid=x+y*width;
		for(int i=-cursorsize;i<=cursorsize;i++){
			for(int j=-cursorsize;j<=cursorsize;j++){
				const int pos=mid+i+j*width;
				if(pos<0||pos>width*height)
					continue;
				if(useImg){
					uint32_t *pixels=imgSurf->pixels;
					int size=imgSurf->w;
					int x=(i+cursorsize)*size/(2*cursorsize);
					int y=(j+cursorsize)*size/(2*cursorsize);
					int imgPos=x+y*size;
					if(imgPos>=0&&imgPos<size*size){
						int imgColor=pixels[imgPos];
						if(getAlpha(imgColor)>0x7f){
							buff[pos].type=type;
							buff[pos].color=imgColor|0xff000000;
							buff[pos].velocity=1;
						}
					}
				} else {
					buff[pos].type=type;
					buff[pos].color=color;
					buff[pos].velocity=1;
				}
			}
		}
	}

	if(useImg){
		SDL_Delay(100);
	}
}

int dit=0;

static inline void updateSandFunc(int j, int way){
	if(buff[j].type==air&&buff[j-width].type==sand){
			buff[j].type=sand;
			buff[j].velocity=buff[j-width].velocity;
			buff[j].velocity++;
			buff[j].color=buff[j-width].color;
			buff[j-width].type=air;
	} else if(buff[j-width].type==sand&&buff[j-1].type==air&&dit){
			buff[j-1].color=buff[j-width].color;
			buff[j-1].type=sand;
			buff[j-width].type=air;
			if(way)
				j--;
			dit=0;
	} else if(buff[j-width].type==sand&&buff[j+1].type==air&&!dit){
			buff[j+1].color=buff[j-width].color;
			buff[j+1].type=sand;
			buff[j-width].type=air;
			if(!way)
				j++;
			dit=1;
	} else{
			dit=!dit;
	}
}

int sw=0;
void updateSand(){
	if(mouse.right||mouse.left&&ctrl){
		mouseDraw(rock);
	} else if(mouse.left){
		mouseDraw(sand);
	}
	for(int i=(height-1)*width;i>width;i-=width){
		if(sw){
			for(int j=i;j<width+i;j++){
				updateSandFunc(j, 1);
			}	
	 	} else {
			for(int j=width+i-1;j>=i;j--){
				updateSandFunc(j, 0);
			}
		}
	}
	sw=!sw;
}

void updateCursor(){
	SDL_Rect cursor={mouse.x-cursorsize, mouse.y-cursorsize, cursorsize*2+1, cursorsize*2+1};
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(renderer, (color&0xff0000)>>16, (color&0xff00)>>8, color&0xff, SDL_ALPHA_OPAQUE);
	if(useImg){
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_RenderCopy(renderer, imgTex, NULL, &cursor);
	} else {
		SDL_RenderFillRect(renderer, &cursor);
	}
}

void updateSurf(){
	for(int i=0;i<height;i++){
		uint32_t col=mix(0xff111244, 0xff000022, i, height);
		for(int j=0;j<width;j++)
			surf[i*width+j]=col;
	}
	for(int i=0;i<width*height;i++){
		switch(buff[i].type){
			case air:
				//surf[i]=0xff111222;
				break;
			case sand:
				surf[i]=buff[i].color;
				break;
			case rock:
				surf[i]=0xff999999;
				break;
			default:
				surf[i]=0xffff00ff;
		}
	}
}

int loop(){
	uint32_t startTime=SDL_GetTicks();
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	
	getInputs();
	
	updateColor();
	updateSand();
	updateSurf();
	
	SDL_Rect copy={0, 0, width, height};
	SDL_UpdateTexture(texture, &copy, surf, width*sizeof(uint32_t));
	SDL_RenderCopy(renderer, texture, &copy, &copy);
	
	updateCursor();
	
	SDL_RenderPresent(renderer);
	
	uint32_t delayTime=(SDL_GetTicks()-startTime);
	if(delayTime>16)
		delayTime=16;
	delayTime=16-delayTime;
	if(delayTime)
		SDL_Delay(delayTime);
	frame++;
	
	/**char x[16]="";
	sprintf(x, "out%4d.ppm", frame);
	saveImg(x);**/
}

int main(int argc, char* argv[]){
	
	if(SDL_Init(SDL_INIT_EVERYTHING))
	{
		printf("Error %s", SDL_GetError());
	}

	window = SDL_CreateWindow("Sand Sim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height + bottomBar, SDL_WINDOW_RESIZABLE|SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
	SDL_ShowCursor(0);
	rnng = 1;
	buff = malloc(width*height*sizeof(*buff));
	for(int i=0;i<width*height;i++){
		buff[i].type=air;
	}

	surf=malloc(width*height*sizeof(uint32_t));
	if(argc>1){
		setUpImg(argv[1]);
	} else {
		useImg=0;
	}
	while(rnng)
		loop();
	
	cleanUp();
	return 0;
}
