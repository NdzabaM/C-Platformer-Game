#pragma once


int init_status_lives(GameState* gameState);
int draw_status_lives(GameState* gameState);
int shutdown_status_lives(GameState* gameState);

int init_game_over(GameState* gameState);
int draw_game_over(GameState* gameState);
int shutdown_game_over(GameState* gameState);

int init_game_win(GameState* gameState);
int draw_game_win(GameState* gameState);
int shutdown_game_win(GameState* gameState);