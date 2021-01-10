#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SCREEN_W 300
#define SCREEN_H 300

// initialize stuff
    SDL_Window *mainwindow = NULL;
    SDL_Surface *viewport = NULL; 
    SDL_Surface *msg = NULL;
    SDL_Renderer *mainrenderer = NULL;
    SDL_Texture *tx1 = NULL;

    SDL_Event e;
    
    uint8_t APP_TERM = 0;
    
    int coord_X = 10;
    int coord_Y = 10;
 
SDL_Texture *load_texture(char* ipath) {
    SDL_Texture* newtexture = NULL;
    SDL_Surface* loadedsurface = IMG_Load(ipath);
    if (loadedsurface == NULL) {
        printf("[EROR] <SDL> %s\n", IMG_GetError() );
    } else {
        newtexture = SDL_CreateTextureFromSurface( mainrenderer, loadedsurface );
        if( newtexture == NULL ) {
            printf("[EROR] <SDL> %s\n", IMG_GetError() );
        }
        SDL_FreeSurface( loadedsurface );
    }
    return newtexture;
}
    
void apply_surface(SDL_Surface *s_src, SDL_Surface *s_dest, int d_x, int d_y, int d_w, int d_h) {
    SDL_Rect dimgrect;
    dimgrect.x = d_x;
    dimgrect.y = d_y;
    dimgrect.w = d_w;
    dimgrect.h = d_h;
    
    SDL_BlitSurface(msg, NULL, viewport, &dimgrect);
}

int app_init() {
    int imgflags = IMG_INIT_PNG;
    
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf("[EROR] Unable to initialize video. \n%s\n",SDL_GetError());
        return 0;
    }
    
    mainwindow = SDL_CreateWindow( "test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN );
    if( mainwindow == NULL) {
        printf("[EROR] A problem occured. \n%s\n",SDL_GetError());
        return 0;
    } else {
            viewport = SDL_GetWindowSurface(mainwindow);
    }
    
    mainrenderer = SDL_CreateRenderer( mainwindow, -1, SDL_RENDERER_ACCELERATED );
    if( mainrenderer == NULL ) {
        printf("[EROR] A problem occured.\n%s\n",SDL_GetError());
        return 0;
    }
    SDL_SetRenderDrawColor( mainrenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    
    if(!( IMG_Init( imgflags ) & imgflags )) {
        printf("[EROR] A problem occured. \n%s\n",SDL_GetError());
        return 0;
    }
    return 1;
}

int app_load() {
    tx1 = load_texture("quote.bmp");
    if(!tx1) {
        printf("[EROR] Unable to load image\n%s\n",SDL_GetError());
        return 0;
    }
    return 1;
}

void app_draw() {
    SDL_RenderClear(mainrenderer);
    SDL_RenderCopy(mainrenderer, tx1, NULL, NULL);
    SDL_RenderPresent( mainrenderer);
}

void app_update() {
    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
            APP_TERM = 1;
        } 
        else if (e.type == SDL_KEYDOWN) {
            switch(e.key.keysym.sym) {
                case SDLK_UP:
                    coord_Y--;
                    break;
                case SDLK_DOWN:
                    coord_Y++;
                    break;
                case SDLK_LEFT:
                    coord_X--;
                    break;
                case SDLK_RIGHT:
                    coord_X++;
                    break; 
            }
        }
    }
}

void app_quit() {
    SDL_DestroyTexture(tx1);
    tx1 = NULL;
    SDL_DestroyRenderer(mainrenderer);
    mainrenderer = NULL;
    SDL_DestroyWindow(mainwindow);
    mainwindow = NULL;
    
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char **argv) {
    if(app_init()) {
        if(app_load()) {
            while(!APP_TERM){
                app_draw();
                app_update();
            }
            
        } else return 1;
    } else return 1;

    app_quit();
    return 0;
}
