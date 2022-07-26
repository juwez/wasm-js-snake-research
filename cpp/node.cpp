#include "node.h"
#include <climits>
#include <cmath>
#include <iostream>
using namespace std;

Node::Node(int x, int y)
    : x(x), y(y), moveCost(0), totalCost(0), isObstacle(false),
      parent(nullptr) {}

int Node::getX() { return this->x; }
int Node::getY() { return this->y; }
int Node::getMoveCost() { return this->moveCost; }
int Node::getTotalCost() { return this->totalCost; }

bool Node::checkObstacle() { return this->isObstacle; }

Node *Node::getParent() { return this->parent; }

void Node::setObstacle(bool obstacle) { this->isObstacle = obstacle; }
void Node::setMoveCost(int moveCost) { this->moveCost = moveCost; }
void Node::setTotalCost(int totalCost) { this->totalCost = totalCost; }
void Node::setParent(Node *parent) { this->parent = parent; }
void Node::reset() {
  this->parent = nullptr;
  this->moveCost = INT_MAX;
  this->totalCost = INT_MAX;
  this->isObstacle = false;
}

// calculate the cost to move from current node to goal (Manhattan distance)
int Node::heuristic(int xGoal, int yGoal) {
  int manhattanDist = (abs(this->x - xGoal) + abs(this->y - yGoal));
  return manhattanDist;
}

void Node::updateTotalCost(int xGoal, int yGoal) {
  this->totalCost = this->moveCost + this->heuristic(xGoal, yGoal);
}
