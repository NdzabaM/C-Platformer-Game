#pragma once

#define STATUS_STATE_LIVES 0
#define STATUS_STATE_GAME 1
#define STATUS_STATE_GAMEOVER 2
#define STATUS_STATE_WIN 3

#define NUM_BLOBS 200
#define NUM_LEDGES 120

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

typedef struct
{
    float x, y;
    float dx, dy;
    short lives;
    char* name;
    int onLedge, isDead;

    int animFrame, facingLeft, slowingDown;
} Man;

typedef struct
{
    int x, y, baseX, baseY, mode;
    float phase;
} Blob;

typedef struct {
    int x, y, w, h;
} Ledge;

typedef struct
{
    float scrollX;

    //Players
    Man man;

    //Starts
    Blob blobs[NUM_BLOBS];

    //Ledges
    Ledge ledges[NUM_LEDGES];

    //Images
    SDL_Texture* blob;
    SDL_Texture* manFrames[2];
    SDL_Texture* tile;
    SDL_Texture* label, *label2;
    int labelW, labelH, label2W, label2H;

    //Fonts
    TTF_Font *font;

    //time
    int time, deathCountdown;
    int statusState;

    //Renderer
    SDL_Renderer* renderer;

    //Window
    SDL_Window* window;
} GameState;

//Prototypes
int renderCode(SDL_Renderer* renderer, GameState* game);
