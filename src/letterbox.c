#include <letterbox.h>
#include <string.h>
// letterbox surface to a square
void letterbox(SDL_Surface** surf){
	SDL_Surface* oldsurf=*surf;
	SDL_SetSurfaceBlendMode(oldsurf, SDL_BLENDMODE_BLEND);			
	if(oldsurf->w > oldsurf->h){
		SDL_Rect srcrect={0, 0, oldsurf->w, oldsurf->h};
		SDL_Rect dstrect={0, (oldsurf->w - oldsurf->h)/2, oldsurf->w, oldsurf->h}; 
		SDL_Surface * newsurf=SDL_CreateRGBSurface(0, oldsurf->w, oldsurf->w, 32, 
				0x00ff0000, 
				0x0000ff00, 
				0x000000ff, 
				0xff000000);
		memset(newsurf->pixels, 0x00, sizeof(uint32_t)*newsurf->w*newsurf->h);
		SDL_SetSurfaceBlendMode(newsurf, SDL_BLENDMODE_BLEND);
		SDL_BlitSurface(oldsurf, &srcrect, newsurf, &dstrect);
		SDL_FreeSurface(oldsurf);
		*surf=newsurf;

	} else if(oldsurf->h > oldsurf->w){
		SDL_Rect srcrect={0, 0, oldsurf->w, oldsurf->h};
		SDL_Rect dstrect={(oldsurf->h - oldsurf->w)/2, 0, oldsurf->w, oldsurf->h}; 
		SDL_Surface * newsurf=SDL_CreateRGBSurface(0, oldsurf->h, oldsurf->h, 32, 
				0x00ff0000, 
				0x0000ff00, 
				0x000000ff, 
				0xff000000);
		memset(newsurf->pixels, 0x00, sizeof(uint32_t)*newsurf->w*newsurf->h);
		SDL_SetSurfaceBlendMode(newsurf, SDL_BLENDMODE_BLEND);
		SDL_BlitSurface(oldsurf, &srcrect, newsurf, &dstrect);
		SDL_FreeSurface(oldsurf);
		*surf=newsurf;

	} else {
		// pass
	}

}
