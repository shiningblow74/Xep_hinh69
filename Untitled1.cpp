#include<windows.h>
#include<string>
#include<SDL.h>
#include<SDL_image.h>

/*
SDL_Surface* LoadImage(std::file_path){
    SDL_Surface *load_image = NULL;
    SDL_Surface *optimize_image = NULL;

    load_image = IMG_Load(file_path.c_str());
    if(load_image != NULL){
        optimize_image = SDL_DisplayFormat(load_image);
        SDL_FreeSurface(load_image);
    }

    return optimize_image;
}
*/
int main(int arc, char* argv[]){
    SDL_Surface *screen;
    SDL_Surface *image;
    if(SDL_Init(SDL_INT_EVERYTHING)==-1)
        return 1;
    screen =SDL_SetVideoMode(1200, 600, 32, SDL_SWSURFACE);
    image = LoadImage("bg3.png");
    SDL_BlitSurface(image,NULL,screen,NULL);
    SDL_Flip(screen);
    SDL_Delay(5000);
    SDL_FreeSurface(image);
    SDL_Quit();

    return 0;
}
