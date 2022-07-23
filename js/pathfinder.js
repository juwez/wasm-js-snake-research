/* eslint-disable prefer-const */
/* eslint-disable no-param-reassign */
/* eslint-disable no-console */
// settings
const c = document.getElementById('canvas');
const ctx = c.getContext('2d');
const width = 640;
const height = 480;
const numRows = 10; // 10 20 40;
const numCols = 10; // 10 20 40;
const cellWidth = width / numCols;
const cellHeight = height / numRows;
let food;
let snake = [];
const buffer = [];
let dead = false;

function drawApple() {
  ctx.fillStyle = 'red';// make apple red
  ctx.fillRect(food.x * cellWidth, food.y * cellHeight, cellWidth, cellHeight);
}
function drawSnake() {
  ctx.fillStyle = 'green';
  snake.forEach((snakePiece) => {
    ctx.fillRect(snakePiece.x * cellWidth, snakePiece.y * cellHeight, cellWidth, cellHeight);
  });
}
function isBlocked(row, col) {
  return snake.some((e) => e.x === row && e.y === col);
}
function createSnake() {
  snake = [{ x: 0, y: 0 }];
}
function placeFood() {
  food = {
    x: 6, // Math.floor(Math.random() * numCols),
    y: 4, // Math.floor(Math.random() * numRows),
  };
  if (isBlocked(food.x, food.y)) {
    placeFood();
  }
}

function checkBounds(row, col) {
  return !(col < 0 || col > numCols - 1 || row < 0 || row > numRows - 1);
}

function getNeighboursBFS(node) {
  // eslint-disable-next-line prefer-const
  let neigbours = [];
  const row = node.x;
  const col = node.y;
  if (checkBounds(row, col - 1) && (!isBlocked(row, col - 1))) {
    neigbours.push(
      {
        x: row,
        y: col - 1,
      },
    );
  }
  if (checkBounds(row, col + 1) && (!isBlocked(row, col + 1))) {
    neigbours.push(
      {
        x: row,
        y: col + 1,
      },
    );
  }
  if (checkBounds(row - 1, col) && (!isBlocked(row - 1, col))) {
    neigbours.push(
      {
        x: row - 1,
        y: col,
      },
    );
  }
  if (checkBounds(row + 1, col) && (!isBlocked(row + 1, col))) {
    neigbours.push(
      {
        x: row + 1,
        y: col,
      },
    );
  }
  return neigbours;
}
function constructPath(node) {
  while (node !== undefined) {
    buffer.push({ x: node.x, y: node.y });
    node = node.parent;
  }
}

function findpathBFS() {
  let openSet = [snake[0]];
  let closedSet = [];
  // get head of snake
  while (openSet.length > 0) {
    const currentNode = openSet.shift();
    if (food.x === currentNode.x && food.y === currentNode.y) {
      constructPath(currentNode);
      return;
    }
    const neigbours = getNeighboursBFS(currentNode);
    neigbours.forEach((currentNeighbor) => {
      // can only contains it if we already proccessed it
      if (closedSet.some((x) => x.x === currentNeighbor.x && x.y === currentNeighbor.y)) {
        return;
      }
      if (!(openSet.some((x) => x.x === currentNeighbor.x && x.y === currentNeighbor.y))) {
        currentNeighbor.parent = currentNode;
        openSet.push(currentNeighbor);
      }
    });
    closedSet.push(currentNode);
  }
  dead = true;
}
function findpathDFS() {
  const stack = [snake[0]];
  const discoveredSet = [];
  while (stack.length > 0) {
    const currentNode = stack.pop();

    if (food.x === currentNode.x && food.y === currentNode.y) {
    // we found a path
      constructPath(currentNode);
      return;
    }
    if (!(discoveredSet.some((x) => x.x === currentNode.x && x.y === currentNode.y))) {
      discoveredSet.push(currentNode);
      const neigbours = getNeighboursBFS(currentNode);
      neigbours.forEach((currentNeighbor) => {
        // can only contains it if we already proccessed it
        stack.push(currentNeighbor);
        if (currentNeighbor.parent === undefined) {
          currentNeighbor.parent = currentNode;
        }
      });
    }
  }
  dead = true;
}
function heuristic(node) {
  return (Math.abs(node.x - food.x) + Math.abs(node.y - food.y));
}
function findpathAStar() {
  let openSet = [snake[0]];
  let closedSet = [];
  openSet[0].moveCost = 0;
  openSet[0].totalcost = heuristic(snake[0]) + openSet[0].moveCost;

  while (openSet.length > 0) {
    // choose node with lowest cost
    openSet.sort((a, b) => b.totalcost - a.totalcost);
    let currentNode = openSet.pop();
    if (food.x === currentNode.x && food.y === currentNode.y) {
      constructPath(currentNode);
      return;
    }
    closedSet.push(currentNode);
    const neigbours = getNeighboursBFS(currentNode);
    neigbours.forEach((currentNeighbor) => {
      // eslint-disable-next-line max-len
      if (!(closedSet.some((x) => x.x === currentNeighbor.x && x.y === currentNeighbor.y))) {
        if (currentNeighbor.parent === undefined) {
          currentNeighbor.parent = currentNode;
          currentNeighbor.moveCost = currentNode.moveCost + 1;
          currentNeighbor.totalcost = heuristic(currentNeighbor) + currentNeighbor.moveCost;
        }
        if (!(openSet.some((y) => y.x === currentNeighbor.x && y.y === currentNeighbor.y))) {
          openSet.push(currentNeighbor);
        }
      }
    });
  }
  dead = true;
}
function moveSnake() {
  if (buffer.length === 0) {
    // findpathBFS();
    // findpathDFS();
    findpathAStar();
    return;
  }
  const newHead = buffer.pop();
  let potentialTail = snake.pop();
  snake.unshift({ x: newHead.x, y: newHead.y });
  ctx.clearRect(0, 0, width, height);
  if (food.x === snake[0].x && food.y === snake[0].y) {
    placeFood();
    snake.push(potentialTail);
  }
  drawApple();
  drawSnake();
}
function mainLoop() {
  moveSnake();
}

function init() {
  dead = false;
  createSnake();
  placeFood();
}

function startGame() {
  init();
  let gameLoop = setInterval(() => {
    mainLoop();
    if (dead) {
      clearInterval(gameLoop);
      startGame();
    }
  }, 50);
}

startGame();
