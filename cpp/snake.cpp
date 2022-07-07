#include "snake.h"
#include "food.h"
#include "main.h"

using namespace std;
//fix init order
Snake::Snake(int numSegments, int x, int y, int screenW, int screenH, string searchAlgorithm) :
startX(x),startY(y),
maxBufferSize(1000),
screenW(screenW), screenH(screenH),
searchAlgorithm(searchAlgorithm),
direction("RIGHT"), dead(false)
{	/*
	Fl_Box* bgImage = new Fl_Box(0, 0, 800, 600);
	bgImage->image(background);
	*/

    //instantiate pathfinder
    pathfinder = new Pathfinder(this);
	srand(time(NULL));
	for (int i = 0; i < numSegments; i++)
	{
		Segment* bodySeg = new Segment(x + (20*i), y);
		body.push_back(bodySeg);
	}
	
	food = new Food(0, 0);
	food->move(SCREEN_WIDTH, SCREEN_HEIGHT, body); //randomize initial food position
	//draw here
	
}

bool Snake::checkDead()
{
	return this->dead;
}

void Snake::addSegment()
{
	//get tail pos
	int tailX = body.back()->getX();
	int tailY = body.back()->getY();
	
	Segment* newSeg = new Segment(tailX, tailY);
	//this->add(newSeg);	//add the segment as a child of the window so it is drawn
	body.push_back(newSeg);
	return;
}

void Snake::checkCollision()
{
	int headX = body.front()->getX();
	int headY = body.front()->getY();
	int foodX = food->getX();
	int foodY = food->getY();
	
	//check for snake eating itself
	for(int i = 1; i < body.size(); i++) //start loop at 1 so we exclude the head!
	{	
		if(body.at(i)->getX() == headX && body.at(i)->getY() == headY)
		{
			//cout << "hit snake" << endl;
			this->dead = true;
		}
	}
	if(headX == foodX && headY == foodY)
	{	//check for food collision
		food->move(SCREEN_WIDTH, SCREEN_HEIGHT, body);
		pathfinder->resetPathFlag();
		this->addSegment();
		cout << "Snake: ate food (size = "  << body.size() << ")" << endl;
	}
	
	//check for out of bounds
	if(headX >= SCREEN_WIDTH || headX < 0 || headY >= SCREEN_HEIGHT || headY < 0)
	{
		this->dead = true;
	}
}

void Snake::move()
{
	//~ pathfinder->printGameState(pathfinder->updateGameState());

	//get head & food pos
	int headX = body.front()->getX();
	int headY = body.front()->getY();
	int foodX = food->getX();
	int foodY = food->getY();

	//only run A* if a valid path has not already been found
	if (!pathfinder->checkPathFound() || pathfinder->checkRepeatSearch())
	{
		//divide by 20 to convert values from pixels to grid coords (gross)
        if(searchAlgorithm == "AStar")
            pathfinder->AStar(headX / 20, headY / 20, foodX / 20, foodY / 20); 
        else if (searchAlgorithm == "BFS")
            pathfinder->BFS(headX / 20, headY / 20, foodX / 20, foodY / 20);
        else if (searchAlgorithm == "DFS")
            pathfinder->DFS(headX / 20, headY / 20, foodX / 20, foodY / 20);
	}
	
	
	if (!buffer.empty())
	{	//read direction from the input buffer
		this->direction = buffer.back();
		buffer.pop_back();
	}
	
	Segment* moveMe = body.back(); //select the segment at the end of the vector
	body.pop_back();			   
	

	//move head based on current direction
	if(this->direction == "UP")
		moveMe->move(headX, headY - 20);
	else if(this->direction == "DOWN")
		moveMe->move(headX, headY + 20);
	else if(this->direction == "LEFT")
		moveMe->move(headX - 20, headY);
	else
		moveMe->move(headX + 20, headY);
	//insert the last tail segment as the new head	
	body.insert(body.begin(), moveMe);
	SDL_SetRenderDrawColor(getRenderer(), 0, 255, 0, SDL_ALPHA_OPAQUE);
	// latest body state so draw here 
	      for (auto x : body){
			SDL_RenderFillRect(getRenderer(), new SDL_Rect{x->getX(), x->getY(),20,20});
		  }	
		SDL_SetRenderDrawColor(getRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
	    SDL_RenderFillRect(getRenderer(), new SDL_Rect{food->getX(), food->getY(),20,20});
    	SDL_RenderPresent(getRenderer());
	return;
}

Pathfinder* Snake::getPathfinder() { return this->pathfinder; }

int Snake::getSize() { return body.size(); }

