#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdint.h>
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
uint32_t*buff;
int rnng;
SDL_Event E;
const int cursorsize=4;
int width=512, height=512;
uint32_t color=0xffffffff;
long frame=0;

void cleanUp(){
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
				mouse.right=1;
				break;
			case SDL_MOUSEBUTTONUP:
				mouse.right=0;
				break;
		}
	}
}

static inline void setPixel(int pos, uint32_t color){
	if(pos<width*height&&pos>=0){
		buff[pos]=color;
	}
}

void updateSand(){
	if(mouse.right){
		int mid=mouse.x+mouse.y*width;
		for(int i=-cursorsize;i<=cursorsize;i++){
			for(int j=-cursorsize;j<=cursorsize;j++){
				setPixel(mid+i+j*width, color);
			}
		}
	}
	int dit=0;
	for(int i=(height-1)*width;i>=width;i-=width){
		for(int j=0;j<width;j++){
			if(buff[i+j]==0&&buff[i+j-width]){
				setPixel(i+j, buff[i+j-width]);
				setPixel(i+j-width, 0x00);
				continue;
			}
			
			if(buff[i+j-width]&&buff[i+j-1]==0x0&&dit){
				setPixel(i+j-1, buff[i+j-width]);
				setPixel(i+j-width, 0x00);
				j++;
				dit=0;
				continue;
			}
			if(buff[i+j-width]&&buff[i+j+1]==0x0&&!dit){
				setPixel(i+j+1, buff[i+j-width]);
				setPixel(i+j-width, 0x00);
				j++;
				dit=1;
				continue;
			}
		}
	}
}
uint32_t hue(int frame, int time){
	int x = frame%(time*3);
	uint32_t color=0x00;
	if(x<time){
		int c1=0xff*(time-x)/time;
		int c2=0xff*x/time;
		color+=c1<<16;
		color+=c2<<8;
	}else if(x<time*2){
		x-=time;
		int c1=0xff*(time-x)/time;
		int c2=0xff*x/time;
		color+=c1<<8;
		color+=c2;
	}else{
		x-=time*2;
		int c1=0xff*(time-x)/time;
		int c2=0xff*x/time;
		color+=c1;
		color+=c2<<16;
	}
	return color;
}

uint32_t bw(int frame, int time){
	int x = frame%(time*2);
	int c=0x00;
	if(x<time){
		c=0xf+0xe0*x/time;
	}else{
		x-=time;
		c=0xf+0xe0*(time-x)/time;
	}
	c&=0xff;
	return (c<<16)+(c<<8)+c;
}

void updateColor(){
	//color=hue(frame, 200);
	color=bw(frame, 100);
}	


int main(int argc, char* argv[]){
	
	if(SDL_Init(SDL_INIT_EVERYTHING))
	{
		printf("Error %s", SDL_GetError());
	}
	window = SDL_CreateWindow("Sand", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
	rnng=1;
	buff=malloc(width*height*sizeof(uint32_t));
	for(int i=0;i<width*height;i++){
		setPixel(i, 0x00);
	}
	while(rnng){
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
		getInputs();
		updateColor();
		updateSand();
		SDL_UpdateTexture(texture, NULL, buff, width*sizeof(uint32_t));
		SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
		SDL_Delay(16);
		frame++;
	}
	
	cleanUp();
	return 0;
}
