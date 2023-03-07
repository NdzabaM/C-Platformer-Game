#include <stdio.h>
#include <main.h>

int init_status_lives(GameState* gameState)
{
	char str[128] = "";

	sprintf_s(str, "x %d", (int)gameState->man.lives);

	//Create label textures for status screen
	SDL_Color white = { 255, 255, 255, 255 };

	SDL_Surface* tmp = TTF_RenderText_Blended(gameState->font, str, white);
	gameState->labelW = tmp->w;
	gameState->labelH = tmp->h;
	gameState->label = SDL_CreateTextureFromSurface(gameState->renderer, tmp);
	SDL_FreeSurface(tmp);

	SDL_Color lightBlue = { 128, 192, 255, 255 };
	tmp = TTF_RenderText_Blended(gameState->font, "Reach the End ALIVE!", lightBlue);
	gameState->label2W = tmp->w;
	gameState->label2H = tmp->h;
	gameState->label2 = SDL_CreateTextureFromSurface(gameState->renderer, tmp);
	SDL_FreeSurface(tmp);

	return 0;
}
int draw_status_lives(GameState* gameState)
{
	SDL_Renderer* renderer = gameState->renderer;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_RenderClear(renderer);

	SDL_Rect rect = { 320 - 80, 240 - 24, 24, 48 };//man struct
	SDL_RenderCopyEx(gameState->renderer, gameState->manFrames[0], NULL, &rect, 0, NULL, SDL_FLIP_NONE);

	SDL_SetRenderDrawColor(gameState->renderer, 255, 255, 255, 255);

	SDL_Rect textRect = { 320 - 20, 240 - gameState->labelH / 2, gameState->labelW, gameState->labelH };
	SDL_RenderCopy(gameState->renderer, gameState->label, NULL, &textRect);

	SDL_Rect textRect2 = { 320 - gameState->label2W/2, 100 - gameState->label2H / 2, gameState->label2W, gameState->label2H };
	SDL_RenderCopy(renderer, gameState->label2, NULL, &textRect2);

	return 0;
}
int shutdown_status_lives(GameState* gameState)
{
	SDL_DestroyTexture(gameState->label);
	gameState->label = NULL;

	SDL_DestroyTexture(gameState->label2);
	gameState->label2 = NULL;

	return 0;
}

int init_game_over(GameState* game)
{
	//Create label textures for status screen
	SDL_Color white = { 255, 255, 255, 255 };

	SDL_Surface* tmp = TTF_RenderText_Blended(game->font, "GAME OVER!", white);
	game->labelW = tmp->w;
	game->labelH = tmp->h;
	game->label = SDL_CreateTextureFromSurface(game->renderer, tmp);
	SDL_FreeSurface(tmp);

	return 0;
}

int draw_game_over(GameState* game)
{
	SDL_Renderer* renderer = game->renderer;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	//Clear the screen
	SDL_RenderClear(renderer);

	//set the drawing color to white
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	SDL_Rect textRect = { 320 - game->labelW / 2, 240 - game->labelH / 2, game->labelW, game->labelH };
	SDL_RenderCopy(renderer, game->label, NULL, &textRect);

	return 0;
}

int shutdown_game_over(GameState* game)
{
	SDL_DestroyTexture(game->label);
	game->label = NULL;

	return 0;
}

int init_game_win(GameState* game)
{
	//Create label textures for status screen
	SDL_Color white = { 255, 255, 255, 255 };

	SDL_Surface* tmp = TTF_RenderText_Blended(game->font, "You live..", white);
	game->labelW = tmp->w;
	game->labelH = tmp->h;
	game->label = SDL_CreateTextureFromSurface(game->renderer, tmp);
	SDL_FreeSurface(tmp);

	return 0;
}

int draw_game_win(GameState* game)
{
	SDL_Renderer* renderer = game->renderer;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	//Clear the screen
	SDL_RenderClear(renderer);

	//set the drawing color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_Rect textRect = { 320 - game->labelW / 2, 240 - game->labelH / 2, game->labelW, game->labelH };
	SDL_RenderCopy(renderer, game->label, NULL, &textRect);

	return 0;
}

int shutdown_game_win(GameState* game)
{
	SDL_DestroyTexture(game->label);
	game->label = NULL;

	return 0;
}