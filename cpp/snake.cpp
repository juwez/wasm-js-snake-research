#include "snake.h"
#include "food.h"
#include "gamesettings.h"
#include "main.h"
#include <chrono>

using namespace std;
// fix init order
Snake::Snake(int numSegments, int x, int y, int screenW, int screenH,
             string searchAlgorithm)
    : startX(x), startY(y), screenW(screenW), screenH(screenH),
      maxBufferSize(1000), dead(false), direction("RIGHT"),
      searchAlgorithm(searchAlgorithm) {
  // instantiate pathfinder
  pathfinder = new Pathfinder(this);
  srand(time(NULL));
  for (int i = 0; i < numSegments; i++) {
    Segment *bodySeg = new Segment(x + (CELL_WIDTH * i), y);
    body.push_back(bodySeg);
  }

  food = new Food(0, 0);
  food->move(body);
}

bool Snake::checkDead() { return this->dead; }

void Snake::addSegment() {
  int tailX = body.back()->getX();
  int tailY = body.back()->getY();

  Segment *newSeg = new Segment(tailX, tailY);
  body.push_back(newSeg);
  return;
}

void Snake::checkCollision() {
  int headX = body.front()->getX();
  int headY = body.front()->getY();
  int foodX = food->getX();
  int foodY = food->getY();

  // check for snake eating itself
  for (int i = 1; i < body.size();
       i++) // start loop at 1 so we exclude the head!
  {
    if (body.at(i)->getX() == headX && body.at(i)->getY() == headY) {
      this->dead = true;
    }
  }
  if (headX == foodX && headY == foodY) { // check for food collision
    food->move(body);
    pathfinder->resetPathFlag();
    this->addSegment();
  }

  // check for out of bounds
  if (headX >= SCREEN_WIDTH || headX < 0 || headY >= SCREEN_HEIGHT ||
      headY < 0) {
    this->dead = true;
  }
}

void Snake::move() {

  // get head & food pos
  int headX = body.front()->getX();
  int headY = body.front()->getY();
  int foodX = food->getX();
  int foodY = food->getY();

  if (!pathfinder->checkPathFound()) {
    if (searchAlgorithm == "AStar")
      pathfinder->AStar(headX / CELL_WIDTH, headY / CELL_HEIGHT,
                        foodX / CELL_WIDTH, foodY / CELL_HEIGHT);
    else if (searchAlgorithm == "BFS")
      pathfinder->BFS(headX / CELL_WIDTH, headY / CELL_HEIGHT,
                      foodX / CELL_WIDTH, foodY / CELL_HEIGHT);
    else if (searchAlgorithm == "DFS")
      pathfinder->DFS(headX / CELL_WIDTH, headY / CELL_HEIGHT,
                      foodX / CELL_WIDTH, foodY / CELL_HEIGHT);
  }

  if (!buffer.empty()) { // read direction from the input buffer
    this->direction = buffer.back();
    buffer.pop_back();
  }

  Segment *moveMe = body.back(); // select the segment at the end of the vector
  body.pop_back();

  // move head based on current direction
  if (this->direction == "UP")
    moveMe->move(headX, headY - CELL_HEIGHT);
  else if (this->direction == "DOWN")
    moveMe->move(headX, headY + CELL_HEIGHT);
  else if (this->direction == "LEFT")
    moveMe->move(headX - CELL_WIDTH, headY);
  else
    moveMe->move(headX + CELL_WIDTH, headY);
  // insert the last tail segment as the new head
  body.insert(body.begin(), moveMe);
  SDL_SetRenderDrawColor(getRenderer(), 0, 255, 0, SDL_ALPHA_OPAQUE);
  // latest body state so draw here
  for (auto x : body) {
    SDL_RenderFillRect(getRenderer(), new SDL_Rect{x->getX(), x->getY(),
                                                   CELL_WIDTH, CELL_HEIGHT});
  }
  SDL_SetRenderDrawColor(getRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(getRenderer(), new SDL_Rect{food->getX(), food->getY(),
                                                 CELL_WIDTH, CELL_HEIGHT});
  SDL_RenderPresent(getRenderer());
  return;
}

Pathfinder *Snake::getPathfinder() { return this->pathfinder; }

int Snake::getSize() { return body.size(); }
