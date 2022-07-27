#include <stdbool.h>
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "food.h"
#include "gamesettings.h"
#include "main.h"
#include "snake.h"
#include <sstream>
using namespace std;
Snake *game;

void quit(void);

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event e;

bool running = false;

bool init(void) {
  bool success = true;
  window = NULL;
  renderer = NULL;

  window = SDL_CreateWindow("snake game", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  running = true;
  string algorithm = "AStar";

  game = new Snake(1, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, algorithm);

  return success;
}

void main_loop(void) {
  Snake *snake = static_cast<Snake *>(game);
  if (!snake->checkDead()) {
    snake->move();
    snake->checkCollision();
  }
}

int main() {
  SDL_Delay(1000);
  if (!init())
    return -1;
  else {

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 1);
#endif

#ifndef __EMSCRIPTEN__
    while (running)
      main_loop();
#endif
  }

  quit_game();
  return 0;
}

void quit_game(void) {

#ifdef __EMSCRIPTEN__
  emscripten_cancel_main_loop();
#endif
}

SDL_Renderer *getRenderer() { return renderer; }