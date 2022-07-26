
class Node {
private:
  // position on the board
  int x, y;

  // movement cost and total (movement + heuristic)
  int moveCost, totalCost;

  bool isObstacle; // does the snake body occupy the current grid cell?

  Node *parent; // reference to parent

public:
  // constructor(s)
  Node(int x, int y);

  // getters & setters
  int getX();
  int getY();
  int getMoveCost();
  int getTotalCost();

  bool checkObstacle();

  void setObstacle(bool obstacle);
  void setMoveCost(int moveCost);
  void setTotalCost(int totalCost);
  void setParent(Node *parent);
  void reset();

  int heuristic(int xGoal, int yGoal);
  void updateTotalCost(int xGoal, int yGoal);

  Node *getParent();
};
