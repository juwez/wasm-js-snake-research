#include <SDL2/SDL.h>
#include "segment.h"
#include "food.h"
#include "main.h"

using namespace std;

SDL_Rect segment;
Segment::Segment(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Segment::move(int newX, int newY)
{
	this->x = newX;				//update segment coordinates
	this->y = newY;	
}

int Segment::getX()
{
	return this->x;
}

int Segment::getY()
{
	return this->y;
}	
