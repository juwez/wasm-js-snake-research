#include <SDL2/SDL.h>
#include "food.h"
#include "main.h"
#include "gamesettings.h"
using namespace std;
SDL_Rect food;

Food::Food(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Food::move( vector<Segment*> body)
{

	int newX, newY;
	do
	{
		//randomize coordinates as long as they are invalid (blocked by snake body)
		newX = (rand() % NUM_COLLS)*CELL_WIDTH;
		newY = (rand() % NUM_ROWS)*CELL_HEIGHT;
	} while (!validPositon(newX, newY, body));
	
	
	this->x = newX;				
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

