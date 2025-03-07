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
uint32_t* buff;
int rnng;
SDL_Event E;
const int cursorsize=2;
int width=256, height=256;
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
		buff[i]=0;
	}
	while(rnng){
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
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
		if(mouse.right){
			int mid=mouse.x+mouse.y*width;
			for(int i=-cursorsize;i<=cursorsize;i++){
				for(int j=-cursorsize;j<=cursorsize;j++){
					buff[mid+i+j*width]=0xffffffff;
				}
			}
		}
		for(int i=(height-1)*width;i>=width;i-=width){
			for(int j=0;j<width;j++){
				if(buff[i+j]==0&&buff[i+j-width]==0xffffffff){
					buff[i+j]=0xffffffff;
					buff[i+j-width]=0x00;
					
				}				
			}
		}
		SDL_UpdateTexture(texture, NULL, buff, width*sizeof(uint32_t));
		SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	free(buff);
	return 0;
}
