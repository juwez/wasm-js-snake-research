#pragma once
#include "node.h"
#include "snake.h"
#include <algorithm>
#include <climits>
#include <ctime>
#include <numeric>
#include <queue>
#include <unistd.h>
#include <vector>

// forward declarations
class Snake;

class Pathfinder {
private:
  Snake *snake;

  int numRows, numCols; // grid size

  bool pathFound; // is a path found?

  std::vector<std::vector<Node *>> gameState;

public:
  Pathfinder(Snake *snake);

  void setSnake(Snake *snake);

  void AStar(int startX, int startY, int goalX, int goalY);
  void BFS(int startX, int startY, int goalX, int goalY);
  void DFS(int startX, int startY, int goalX, int goalY);

  void resetPathFlag();

  std::vector<std::vector<Node *>> updateGameState();
  std::vector<Node *> getNeighbors2(std::vector<std::vector<Node *>> gameState,
                                    Node *node);
  std::vector<Node *>
  getNeighborsBFS(std::vector<std::vector<Node *>> gameState, Node *node);

  // logic checks
  bool checkBounds(int row, int col); // is node out of bounds?
  bool checkGoal(int row, int col, int goalRow,
                 int goalCol); // is the node the goal?
  bool checkBlocked(std::vector<std::vector<Node *>> gameState, int row,
                    int col); // is the node within the snake?
  bool checkPathFound();
  bool nodeInSet(std::vector<Node *> set,
                 Node *node); // performs a linear search for node in set

  // Walks the path created by A*, returns a buffer of directions
  std::vector<std::string> buildPath(std::vector<std::vector<Node *>> gameState,
                                     Node *goal);

  double calcAvg(
      std::vector<int> data); // takes floating-point mean of an integer vector
  double calcAvg(
      std::vector<double> data); // takes floating-point mean of a double vector
};
