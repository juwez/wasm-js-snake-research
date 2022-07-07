#include <SDL2/SDL.h>
#include "food.h"
#include "main.h"
using namespace std;
SDL_Rect food;

Food::Food(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Food::move(int maxX, int maxY, vector<Segment*> body)
{

	//jump to a random position within bounds (max x/y)
	int newX, newY;
	do
	{
		//randomize coordinates as long as they are invalid (blocked by snake body)
		newX = (rand() % maxX/20)*20;
		newY = (rand() % maxY/20)*20;
	} while (!validPositon(newX, newY, body));
	
	
	this->x = newX;				//update segment coordinates
	this->y = newY;
}


bool Food::validPositon(int x, int y, vector<Segment*> body)
{
	for (int i = 0; i < body.size(); i++)
	{
		if (body.at(i)->getX() == x && body.at(i)->getY() == y)
			return false;
	}
	return true;
	
}

int Food::getX()
{
	return this->x;
}

int Food::getY()
{
	return this->y;
}	

