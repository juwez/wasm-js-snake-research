#pragma once 
#include <iostream>
#include <vector>
#include "segment.h"

class Food
{
	private:
		int x, y;	public:
		Food(int x, int y);
		void renderFood(void);
		void move( std::vector<Segment*> body);
		int getX();
		int getY();

		bool validPositon(int x, int y, std::vector<Segment*> body);
};
