#include "pathfinder.h"
#include "gamesettings.h"
#include <chrono>
using namespace std;
bool compare(Node *lhs, Node *rhs) {
  return (lhs->getTotalCost() > rhs->getTotalCost());
}

Pathfinder::Pathfinder(Snake *snake)
    : snake(snake), numRows(NUM_ROWS), numCols(NUM_COLLS), pathFound(false) {
  for (int i = 0; i < numRows; i++) {
    vector<Node *> currentRow;
    for (int j = 0; j < numCols; j++) {
      Node *tempNode = new Node(j, i);
      currentRow.push_back(tempNode);
    }
    gameState.push_back(currentRow);
  }
}

void Pathfinder::resetPathFlag() { this->pathFound = false; }

void Pathfinder::AStar(int startX, int startY, int goalX, int goalY) {
  cout << "snakeLength " << snake->getSize() << endl;
  cout << "startX " << startX << endl;
  cout << "startY " << startY << endl;
  cout << "foodX " << goalX << endl;
  cout << "foodY " << goalY << endl;

  auto startTime = std::chrono::high_resolution_clock::now();
  // fetch the most recent iteration of the game board
  vector<vector<Node *>> currGameState = updateGameState();

  // initialize the open and closed sets
  vector<Node *> openSet;
  vector<Node *> closedSet;

  // init start node and add to open set
  openSet.push_back(currGameState.at(startY).at(startX));
  currGameState.at(startY).at(startX)->setMoveCost(0);
  currGameState.at(startY).at(startX)->setTotalCost(
      currGameState.at(startY).at(startX)->heuristic(goalX, goalY));

  while (!openSet.empty()) {
    // choose the node in the open set w/ the lowest total cost
    stable_sort(openSet.begin(), openSet.end(), compare);
    Node *current = openSet.back();

    // don't do anymore pathfinding if we've found the goal!
    if (checkGoal(current->getY(), current->getX(), goalY, goalX)) {
      auto finishTime = std::chrono::high_resolution_clock::now();
      auto searchTime = std::chrono::duration_cast<std::chrono::microseconds>(
          finishTime - startTime);
      cout << "nodeCount " << openSet.size() + closedSet.size() << endl;
      cout << "timeTaken " << searchTime.count() << endl;

      currGameState.at(startY).at(startX)->setParent(nullptr);
      // write path directly to snake's input buffer
      snake->buffer =
          buildPath(currGameState, currGameState.at(goalY).at(goalX));

      // tell the game to stop pathfinding (we're done until we eat the food)
      pathFound = true;
      return;
    }

    // add the current node to the closed set and remove from open set
    openSet.pop_back();
    closedSet.push_back(current);

    // fetch the four adjacent nodes to the current node (if they are valid)
    vector<Node *> neighborVec = getNeighbors2(currGameState, current);

    for (int i = 0; i < neighborVec.size(); i++) {
      Node *currentNeighbor = neighborVec.at(i);

      // if neighbor is not in closed set
      if (!nodeInSet(closedSet, currentNeighbor)) {
        // update score
        currentNeighbor->updateTotalCost(goalX, goalY);

        // if neighbor is not in open set
        if (!nodeInSet(openSet, currentNeighbor)) {
          openSet.push_back(currentNeighbor);
        }
      }
    }
  }

  auto finishTime = std::chrono::high_resolution_clock::now();
  auto searchTime = std::chrono::duration_cast<std::chrono::microseconds>(
      finishTime - startTime);

  cout << "nodeCount " << openSet.size() + closedSet.size() << endl;

  cout << "timeTaken " << searchTime.count() << endl;
  cout << "game over" << endl;
}

void Pathfinder::BFS(int startX, int startY, int goalX, int goalY) {
  cout << "snakeLength " << snake->getSize() << endl;
  cout << "startX " << startX << endl;
  cout << "startY " << startY << endl;
  cout << "foodX " << goalX << endl;
  cout << "foodY " << goalY << endl;
  auto startTime = std::chrono::high_resolution_clock::now();
  // fetch the most recent iteration of the game board
  vector<vector<Node *>> currGameState = updateGameState();

  // initialize the open and closed sets
  vector<Node *> openSet;
  vector<Node *> closedSet;

  // init start node and add to open set
  openSet.push_back(currGameState.at(startY).at(startX));

  while (!openSet.empty()) {
    Node *current = openSet.front();
    openSet.erase(openSet.begin());
    if (checkGoal(current->getY(), current->getX(), goalY, goalX)) {
      cout << "nodes " << openSet.size() << " " << closedSet.size() << endl;

      auto finishTime = std::chrono::high_resolution_clock::now();
      auto searchTime = std::chrono::duration_cast<std::chrono::microseconds>(
          finishTime - startTime);
      cout << "nodeCount " << openSet.size() + closedSet.size() << endl;
      cout << "timeTaken " << searchTime.count() << endl;

      currGameState.at(startY).at(startX)->setParent(nullptr);
      // write path directly to snake's input buffer
      snake->buffer =
          buildPath(currGameState, currGameState.at(goalY).at(goalX));
      // tell the game to stop pathfinding (we're done until we eat the food)
      pathFound = true;
      return;
    }

    vector<Node *> neighborVec = getNeighborsBFS(currGameState, current);
    for (int i = 0; i < neighborVec.size(); i++) {
      Node *currentNeighbor = neighborVec.at(i);
      if (nodeInSet(closedSet, currentNeighbor)) {
        continue;
      }

      if (!nodeInSet(openSet, currentNeighbor)) {
        currentNeighbor->setParent(current);
        openSet.push_back(currentNeighbor);
      }
    }
    closedSet.push_back(current);
  }
  auto finishTime = std::chrono::high_resolution_clock::now();
  auto searchTime = std::chrono::duration_cast<std::chrono::microseconds>(
      finishTime - startTime);
  cout << "nodeCount " << openSet.size() + closedSet.size() << endl;
  cout << "timeTaken " << searchTime.count() << endl;
  cout << "game over" << endl;
}

void Pathfinder::DFS(int startX, int startY, int goalX, int goalY) {
  cout << "snakeLength " << snake->getSize() << endl;
  cout << "startX " << startX << endl;
  cout << "startY " << startY << endl;
  cout << "foodX " << goalX << endl;
  cout << "foodY " << goalY << endl;
  auto startTime = std::chrono::high_resolution_clock::now();

  vector<vector<Node *>> currGameState = updateGameState();
  vector<Node *> discoveredSet;
  vector<Node *> stack;

  stack.push_back(currGameState.at(startY).at(startX));

  while (!stack.empty()) {
    Node *current = stack.back();
    stack.pop_back();

    if (checkGoal(current->getY(), current->getX(), goalY, goalX)) {
      auto finishTime = std::chrono::high_resolution_clock::now();
      auto searchTime = std::chrono::duration_cast<std::chrono::microseconds>(
          finishTime - startTime);
      cout << "nodeCount " << discoveredSet.size() << endl;
      cout << "timeTaken " << searchTime.count() << endl;

      currGameState.at(startY).at(startX)->setParent(
          nullptr); // shitty spaghet code

      // write path directly to snake's input buffer
      snake->buffer =
          buildPath(currGameState, currGameState.at(goalY).at(goalX));
      pathFound = true;
      return;
    }

    if (!nodeInSet(discoveredSet, current)) {
      discoveredSet.push_back(current);
      vector<Node *> neighborVec = getNeighborsBFS(currGameState, current);

      for (int i = 0; i < neighborVec.size(); i++) {
        Node *currentNeighbor = neighborVec.at(i);
        stack.push_back(currentNeighbor);
        if (currentNeighbor->getParent() == nullptr)
          currentNeighbor->setParent(current);
      }
    }
  }

  auto finishTime = std::chrono::high_resolution_clock::now();
  cout << "nodeCount " << discoveredSet.size() << endl;

  auto searchTime = std::chrono::duration_cast<std::chrono::microseconds>(
      finishTime - startTime);

  cout << "timeTaken " << searchTime.count() << endl;
  cout << "game over" << endl;
}

vector<Node *> Pathfinder::getNeighbors2(vector<vector<Node *>> gameState,
                                         Node *node) {
  // will contain the VALID adjacent nodes to the argument node
  vector<Node *> neighborVec;
  int row = node->getY();
  int col = node->getX();

  /* for each potential neighbor:
   * -check if it's in bounds
   * -check if it's NOT an obstacle
   * -if the potential neighbor does not have a parent, set it to the argument
   *  node */

  if (checkBounds(row - 1, col) && (!checkBlocked(gameState, row - 1, col))) {
    if (gameState.at(row - 1).at(col)->getParent() == nullptr) {
      gameState.at(row - 1).at(col)->setParent(node);
      gameState.at(row - 1).at(col)->setMoveCost(node->getMoveCost() + 1);
    }
    neighborVec.push_back(gameState.at(row - 1).at(col));
  }
  if (checkBounds(row + 1, col) && (!checkBlocked(gameState, row + 1, col))) {
    if (gameState.at(row + 1).at(col)->getParent() == nullptr) {
      gameState.at(row + 1).at(col)->setParent(node);
      gameState.at(row + 1).at(col)->setMoveCost(node->getMoveCost() + 1);
    }
    neighborVec.push_back(gameState.at(row + 1).at(col));
  }
  if (checkBounds(row, col - 1) && (!checkBlocked(gameState, row, col - 1))) {
    if (gameState.at(row).at(col - 1)->getParent() == nullptr) {
      gameState.at(row).at(col - 1)->setParent(node);
      gameState.at(row).at(col - 1)->setMoveCost(node->getMoveCost() + 1);
    }
    neighborVec.push_back(gameState.at(row).at(col - 1));
  }
  if (checkBounds(row, col + 1) && (!checkBlocked(gameState, row, col + 1))) {
    if (gameState.at(row).at(col + 1)->getParent() == nullptr) {
      gameState.at(row).at(col + 1)->setParent(node);
      gameState.at(row).at(col + 1)->setMoveCost(node->getMoveCost() + 1);
    }
    neighborVec.push_back(gameState.at(row).at(col + 1));
  }
  return neighborVec;
}

vector<Node *> Pathfinder::getNeighborsBFS(vector<vector<Node *>> gameState,
                                           Node *node) {
  // will contain the VALID adjacent nodes to the argument node
  vector<Node *> neighborVec;
  int row = node->getY();
  int col = node->getX();

  /* for each potential neighbor:
   * -check if it's in bounds
   * -check if it's NOT an obstacle
   */

  if (checkBounds(row - 1, col) && (!checkBlocked(gameState, row - 1, col))) {
    neighborVec.push_back(gameState.at(row - 1).at(col));
  }
  if (checkBounds(row + 1, col) && (!checkBlocked(gameState, row + 1, col))) {
    neighborVec.push_back(gameState.at(row + 1).at(col));
  }
  if (checkBounds(row, col - 1) && (!checkBlocked(gameState, row, col - 1))) {
    neighborVec.push_back(gameState.at(row).at(col - 1));
  }
  if (checkBounds(row, col + 1) && (!checkBlocked(gameState, row, col + 1))) {
    neighborVec.push_back(gameState.at(row).at(col + 1));
  }
  return neighborVec;
}

vector<string> Pathfinder::buildPath(vector<vector<Node *>> gameState,
                                     Node *goal) {
  // Start from goal node, move backwards to start
  vector<string> pathBuffer;
  Node *current = goal;

  while (current != nullptr) {
    Node *currentParent = current->getParent();
    if (currentParent == nullptr)
      break; // you're dereferencing a null pointer! open your eyes!

    int currentX = current->getX();
    int currentY = current->getY();
    int parentX = currentParent->getX();
    int parentY = currentParent->getY();

    // parent = closer along path towards goal than current
    if (currentX < parentX) {
      pathBuffer.push_back("LEFT");
    } else if (currentX > parentX) {
      pathBuffer.push_back("RIGHT");
    } else if (currentY < parentY) {
      pathBuffer.push_back("UP");
    } else {
      pathBuffer.push_back("DOWN");
    }

    // traverse the path by moving to the next successor node
    current = currentParent;
  }

  // add the current entry to the statistics vector
  return pathBuffer;
}

void Pathfinder::setSnake(Snake *snake) { this->snake = snake; }

vector<vector<Node *>> Pathfinder::updateGameState() {
  // initialize all grid cells to be empty

  // initialize game state vector
  for (int i = 0; i < numRows; i++) {
    vector<Node *> currentRow;
    for (int j = 0; j < numCols; j++) {
      gameState.at(i).at(j)->reset();
    }
  }

  // set the snake's body as obstacles
  for (int i = 0; i < snake->body.size(); i++) {
    gameState.at(snake->body.at(i)->getY() / CELL_HEIGHT)
        .at(snake->body.at(i)->getX() / CELL_WIDTH)
        ->setObstacle(true);
  }
  return gameState;
}

bool Pathfinder::nodeInSet(
    vector<Node *> set,
    Node *node) { // linearly search the argument set for the argument node
  for (int i = 0; i < set.size(); i++) {
    if (node == set.at(i)) {
      return true;
    }
  }
  return false;
}

bool Pathfinder::checkPathFound() { return this->pathFound; }

bool Pathfinder::checkBounds(int row, int col) {
  // check if the cell is within the board
  if (col < 0 || col > numCols - 1 || row < 0 || row > numRows - 1)
    return false;
  else
    return true;
}

bool Pathfinder::checkGoal(int row, int col, int goalRow, int goalCol) {
  return ((row == goalRow) && (col == goalCol));
}

bool Pathfinder::checkBlocked(vector<vector<Node *>> gameState, int row,
                              int col) {
  return gameState.at(row).at(col)->checkObstacle();
}

double Pathfinder::calcAvg(vector<double> data) {
  double dataSum = accumulate(data.begin(), data.end(), 0.0);
  double mean = dataSum / (double)data.size();
  return mean;
}

double Pathfinder::calcAvg(vector<int> data) {
  int dataSum = accumulate(data.begin(), data.end(), 0);
  double mean = (double)dataSum / (double)data.size();
  return mean;
}
