#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#define MAIN_DELAY    2000
#define SCREEN_WIDTH  512
#define SCREEN_HEIGHT 512
#define SCREEN_BPP    32
#define TILE_SIZE     256

void main_loop();
void update();
void draw();
void load();

int TERM = 0;

int msgx = 10;
int msgy = 10;

SDL_Color txtcolor = {0xFF, 0xFF, 0xFF};

SDL_Surface *LoadImage(char* ipath){ 
  SDL_Surface* temp = IMG_Load(ipath);
  SDL_Surface* img = NULL;
  if (temp != NULL){
    img = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);
    return img;
  } else {
    return NULL;
  }
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination ) {
  SDL_Rect offset;
  offset.x = x;
  offset.y = y;
  SDL_BlitSurface( source, NULL, destination, &offset );
}

SDL_Surface *SetColorKey(SDL_Surface *img,int kr,int kg,int kb) {
  if (img != NULL) {
    Uint32 colorkey = SDL_MapRGB( img->format, kr, kg, kb);
    SDL_SetColorKey( img, SDL_SRCCOLORKEY, colorkey );
    return img;
  } else {
    return NULL;
  }
}
//-----------------------

int main(int argc, char **argv){
  SDL_Surface* screen = NULL;
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) return 1;
  
  if( TTF_Init() == -1 ) return 1; 
  
  TTF_Font *deffont = NULL;
  deffont = TTF_OpenFont( "mono.ttf", 16 );
  
  screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
  if(screen == NULL) return 2;
  SDL_WM_SetCaption("test",NULL);
  
  SDL_Surface *message = TTF_RenderText_Solid( deffont, "The surest protection against temptation is cowardice.", txtcolor );
  
  
  SDL_Event event;
  
  if(background == NULL && message == NULL) return 1;
  
  void draw() {
  for(int i = 0; i < (SCREEN_WIDTH/TILE_SIZE); i++){
    for(int j=0;j<(SCREEN_HEIGHT/TILE_SIZE);j++){
      apply_surface(0 + TILE_SIZE*i, 0 + TILE_SIZE*j, background, screen );
      }
    }
    apply_surface( msgx, msgy , message, screen );
  }
  
  void update() {
    SDL_Flip( screen );
    while (SDL_PollEvent(&event)){
      if( event.type == SDL_QUIT ){
        TERM = 1;
      } else if(event.type == SDL_KEYDOWN){
        switch(event.key.keysym.sym){
          case SDLK_UP: msgy -= 1; break;
          case SDLK_DOWN: msgy += 1; break;
          case SDLK_LEFT: msgx -= 1; break;
          case SDLK_RIGHT: msgx += 1; break;
        }
        
      }
    }
  }
  
  void main_loop() {
    while(!TERM) {
      draw();
      update();
    }
  }
  
  void cleanup() {
    SDL_FreeSurface(background);
    SDL_FreeSurface(message);
  }
  
    main_loop();
    
    cleanup();
    SDL_Quit();
    
    return 0;
}
