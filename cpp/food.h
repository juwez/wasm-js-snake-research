#pragma once 
#include <iostream>
#include <vector>
#include "segment.h"

class Food
{
	private:
		int x, y;	//food coordinates in the game window
		int w, h; //needed for SDL_RECT
	public:
		Food(int x, int y);
		void renderFood(void);
		//jump to a random position within bounds specified by max x/y
		void move(int maxX, int maxY, std::vector<Segment*> body);
		int getX();
		int getY();

		bool validPositon(int x, int y, std::vector<Segment*> body);
};
