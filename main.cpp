// Game1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <SDL.h>
#include <stdio.h>
#include <ctime>
#include <main.h>
#include <status.h>

#define GRAVITY 0.35f

unsigned int seed = 2;

unsigned int rand()
    {
        seed = 1664525 * seed + 1013904223;
        return seed;
    }

int initBlobs(GameState* game)
{   //adding star movement
    for (int i = 0; i < NUM_BLOBS; i++)
    {
        game->blobs[i].baseX = 320 + rand() % 38400;
        game->blobs[i].baseY = rand() % 480;
        game->blobs[i].mode = rand() % 2;
        game->blobs[i].phase = 2*3.14*(rand() % 360)/360.0f;

    }
    return 0;
}


int loadGame(GameState *game)
{
    SDL_Surface* surface = NULL;

    //Load images and create rendering textures from them

    //blob image
    surface = IMG_Load("blob.png");
    if (surface == NULL)
    {
        printf("Cannot find blob.png!\n\n");
        SDL_Quit();
        exit(1);
    }

    game->blob = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    
    //Man idle image
    surface = IMG_Load("man_idle.png");
    if (surface == NULL)
    {
        printf("Cannot find man_idle.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->manFrames[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);


    //man run
    surface = IMG_Load("man_run.png");
    if (surface == NULL)
    {
        printf("Cannot find man_run.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->manFrames[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    //tile image
    surface = IMG_Load("tile.png");
    if (surface == NULL)
    {
        printf("Cannot find tile.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->tile = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    //font image
    game->font = TTF_OpenFont("04B_30__.TTF", 24);
    if (!game->font)
    {
        printf("Cannot find 04B_30__.TTF!\n\n");
        SDL_Quit();
        exit(1);
    }

    game->label = NULL;
    game->label2 = NULL;

    game->man.x = 100;
    game->man.y = 240 - 40;
    game->man.dx = 0;
    game->man.dy = 0;
    game->man.onLedge = 0;
    game->man.animFrame = 0;
    game->man.lives = 3;
    game->man.isDead = 0;
    //game->man.facingLeft = 1;
    game->man.slowingDown = 0;
    game->statusState = STATUS_STATE_LIVES;

    init_status_lives(game);

    game->time = 0;
    game->scrollX = 0;
    game->deathCountdown = -1;

    initBlobs(game);
   

    //adding ledges and positioning
    for (int i = 0; i < NUM_LEDGES; i++)
    {
        game->ledges[i].x = i * 384;
        game->ledges[i].w = 256;
        game->ledges[i].h = 32;
        if(i == 0)
            game->ledges[i].y = 400;
        else
            game->ledges[i].y = 300+100 - rand()%200;
    }

    for (int i = NUM_LEDGES - 20; i < NUM_LEDGES; i++)
    {
        game->ledges[i].x = i * 350 + rand()%38400;
        game->ledges[i].w = 256;
        game->ledges[i].h = 32;
        if (i % 2 == 0)
            game->ledges[i].y = 200;
        else
            game->ledges[i].y = 350;
    }

    return 0;
}

int process(GameState* game)
{
    //add time
    game->time++;

    if (game->statusState == STATUS_STATE_LIVES)
    {
        if (game->time > 120)
        {
            shutdown_status_lives(game);
            game->statusState = STATUS_STATE_GAME;
        }
    }
    else if (game->statusState == STATUS_STATE_GAMEOVER)
    {
        if (game->time > 190)
        {
            SDL_Quit();
            exit(0);
        }
    }
    else if (game->statusState == STATUS_STATE_GAME)
    {
        if (!game->man.isDead) 
        {
            //man movement
            Man* man = &game->man;
            man->x += man->dx;
            man->y += man->dy;

            if (man->dx != 0 && man->onLedge && !man->slowingDown)
            {
                if (game->time % 10 == 0) //fps man animframes
                {
                    if (man->animFrame == 0)
                    {
                        man->animFrame = 1;
                    }
                    else
                    {
                        man->animFrame = 0;
                    }
                }
            }
            if (man->x > 38320)
            {
                init_game_win(game);
                game->statusState = STATUS_STATE_WIN;
            }
            man->dy += GRAVITY;

            //blobs movement
            for (int i = 0; i < NUM_BLOBS; i++)
            {
                game->blobs[i].x = game->blobs[i].baseX;
                game->blobs[i].y = game->blobs[i].baseY;

                if (game->blobs[i].mode == 0)
                {
                    game->blobs[i].x = game->blobs[i].baseX + sinf(game->blobs[i].phase + game->time * 0.06f) * 75;
                }
                else
                {
                    game->blobs[i].y = game->blobs[i].baseY + cosf(game->blobs[i].phase + game->time * 0.06f) * 75;
                }
            }
        }
        

        if (game->man.isDead && game->deathCountdown < 0)
        {
            game->deathCountdown = 120;
        }
        if (game->deathCountdown >= 0)
        {
            game->deathCountdown--;
            if (game->deathCountdown < 0)
            {
                //shutdown_status_lives(game);
                //game->statusState = STATUS_STATE_GAMEOVER;
                game->man.lives--;

                if (game->man.lives >= 0)
                {
                    init_status_lives(game);
                    game->statusState = STATUS_STATE_LIVES;
                    game->time = 0;

                    //reset
                    game->man.isDead = 0;
                    game->man.x = 100;
                    game->man.y = 240 - 40;
                    game->man.dx = 0;
                    game->man.dy = 0;
                    game->man.onLedge = 0;
                    initBlobs(game);
                }
                else
                {
                    init_game_over(game);
                    game->statusState = STATUS_STATE_GAMEOVER;
                    game->time = 0;
                }
            }

        }
    }

    game->scrollX = -game->man.x + 320;
    if (game->scrollX > 0)
        game->scrollX = 0;
    if (game->scrollX < -38000 + 320)
        game->scrollX = -38000 + 320;

    return 0;
}

int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2)
{
    return (!((x1 > (x2 + wt2)) || (x2 > (x1 + wt1)) || (y1 > (y2 + ht2)) || (y2 > (y1 + ht1))));
}

int collisionDetect(GameState* game)
{
    //collision with blobs
    for (int i = 0; i < NUM_BLOBS; i++)
    {
        if (collide2d(game->man.x, game->man.y, game->blobs[i].x, game->blobs[i].y, 24, 48, 16, 16))
        {
            if (!game->man.isDead)
            {
                game->man.isDead = 1;
            }
            break;
        }
     
    }

    //for falling
    if (game->man.y > 480)
    {
        if (!game->man.isDead)
        {
            game->man.isDead = 1;
        }
    }

    //Check for collision with any ledges (tiles)
    for (int i = 0; i < NUM_LEDGES; i++)
    {
        float mw = 24, mh = 48;
        float mx = game->man.x, my = game->man.y;
        float bx = game->ledges[i].x, by = game->ledges[i].y, bw = game->ledges[i].w, bh = game->ledges[i].h;

        if (mx + mw/2 > bx && mx + mw/2 <bx+bw)
        {
            //bumping our head?
            if (my < by + bh && my > by && game->man.dy < 0)
            {
                //correct y
                game->man.y = by + bh;
                my = by + bh;
                //bumped our head, stop velocity
                game->man.dy = 0;
                game->man.onLedge = 1;
            }
        }

        if (mx + mw > bx && mx < bx + bw)
        {
            //landing on edge?
            if (my + mh > by && my < by && game->man.dy > 0)
            {
                //correct y
                game->man.y = by - mh;
                my = by - mh;

                //landed on edge, stop any jump velocity
                game->man.dy = 0;
                game->man.onLedge = 1;
            }
        }

        if (my + mh > by && my < by + bh)
        {
            //rubbing against right edge
            if (mx < bx + bw && mx + mw > bx + bw && game->man.dx < 0)
            {
                //correct x
                game->man.x = bx + bw;
                mx = bx + bw;

                game->man.dx = 0;
            }
            //rubbing against left edge
            else if (mx + mw > bx && mx < bx && game->man.dx > 0)
            {
                //correct x
                game->man.x = bx - mw;
                mx = bx - mw;

                game->man.dx = 0;
            }
        }
    }
    return 0;
}

int processEvents(SDL_Window* window, GameState *game)
{
    SDL_Event event;
    int done = 0;
    //Check for events in while loop
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_WINDOWEVENT_CLOSE:
        {
            if (window)
            {
                SDL_DestroyWindow(window);
                window = NULL;
                done = 1;
            }
        }
        break;
        case SDL_KEYDOWN:
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                done = 1;
                break;
            case SDLK_UP:
                if (game->man.onLedge)       //or (game->man.dy == 0.0f)
                {
                    game->man.dy = -8;
                    game->man.onLedge = 0;
                }
                    break;
            }
        }
        break;
        case SDL_QUIT:
            //quit out of game
            done = 1;
            break;
        }
    }

    

    //walking
    const Uint8* state = SDL_GetKeyboardState(NULL);
    //Higher jump
    if (state[SDL_SCANCODE_UP])
    {
        game->man.dy -= 0.2f;
    }

    if (state[SDL_SCANCODE_LEFT])
    {   
        game->man.dx -= 0.5;
        if (game->man.dx < -6)
        {
            game->man.dx = -6;
        }
        //game->man.facingLeft = 1;
        game->man.slowingDown = 0;
    }
    else if (state[SDL_SCANCODE_RIGHT])
    {
        game->man.dx += 0.5;
        if (game->man.dx > 6)
        {
            game->man.dx = 6;
        }
        //game->man.facingLeft = 0;
        game->man.slowingDown = 0;

       
    }
    else
    {
        game->man.animFrame = 0;
        game->man.dx *= 0.8f;
        game->man.slowingDown = 1;
        if (fabsf(game->man.dx) < 0.1f)
        {
            game->man.dx = 0;
        }
    }
    /*if (state[SDL_SCANCODE_UP])
    {
        game->man.y -= 10;
    }
    if (state[SDL_SCANCODE_DOWN])
    {
        game->man.y += 10;
    }*/


    return done;
}

int renderCode(SDL_Renderer *renderer, GameState *game)
{
    if (game->statusState == STATUS_STATE_LIVES)
    {
        draw_status_lives(game);
    }
    else if (game->statusState == STATUS_STATE_GAMEOVER)
    {
        draw_game_over(game);
    }
    else if (game->statusState == STATUS_STATE_WIN)
    {
        draw_game_win(game);
    }
    else if (game->statusState == STATUS_STATE_GAME)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

        //Clear screen to blue
        SDL_RenderClear(renderer);

        //Set drawing colour to white
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        //draw ledges
        for (int i = 0; i < NUM_LEDGES; i++)
        {
            SDL_Rect ledgeRect = { game->scrollX+game->ledges[i].x, game->ledges[i].y, game->ledges[i].w, game->ledges[i].h };
            SDL_RenderCopy(renderer, game->tile, NULL, &ledgeRect);
        }

        //draw a rectangle at man's position
        SDL_Rect rect = { game->scrollX+game->man.x, game->man.y, 24, 48 };//man struct
        SDL_RenderCopyEx(renderer, game->manFrames[game->man.animFrame], NULL, &rect, 0, NULL, SDL_FLIP_NONE);

        //draw a blob image
        for (int i = 0; i < NUM_BLOBS; i++)
        {
            SDL_Rect blobRect = { game->scrollX + game->blobs[i].x,game->blobs[i].y,64, 64 };
            SDL_RenderCopy(renderer, game->blob, NULL, &blobRect);
        }

        
    }
    //Done drawing, now use "Present" to show the screen what we've drawn
    SDL_RenderPresent(renderer);
    return 0;

}

int main(int argc, char *argv[])
{
    GameState gameState;
    SDL_Window *window = NULL; //Declares Window
    SDL_Renderer *renderer = NULL; //Declares renderer

    SDL_Init(SDL_INIT_VIDEO); //Initialize SDL2

    //Create application window with the following settings
    window = SDL_CreateWindow("Game1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    gameState.renderer = renderer;

    TTF_Init(); //initialise font system

    //pass in load function
    loadGame(&gameState);

    //Window is open: enter program loop (see SDL_PollEvent)
    int done = 0;

    //Event loop (while loop)
    while (!done)
    {
        if (processEvents(window, &gameState) == 1) //check for events
            done=1;
        renderCode(renderer, &gameState); //Render display

        process(&gameState);//genereic method for processing all collisions
        collisionDetect(&gameState);

        //SDL_Delay(10);
    }

        //Shutdown game and unload all memory
        SDL_DestroyTexture(gameState.blob);
        SDL_DestroyTexture(gameState.tile);
        SDL_DestroyTexture(gameState.manFrames[0]);
        SDL_DestroyTexture(gameState.manFrames[1]);
        if (gameState.label != NULL)
            SDL_DestroyTexture(gameState.label);
        TTF_CloseFont(gameState.font);


        //Close and destroy window
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);


        TTF_Quit();

        //Clean up
        SDL_Quit();
        return 0;
    
}

