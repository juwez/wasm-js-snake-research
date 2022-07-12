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
const searchAlgorithm = 'BFS';// DFS AStar

let food;
let snake = [];
const buffer = [];

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
function createSnake() {
  snake = [{ x: 0, y: 0 }];
}
function placeFood() {
  food = {
    x: Math.floor(Math.random() * numCols),
    y: Math.floor(Math.random() * numRows),
  };
}

function moveSnake() {
  console.log(buffer);
  const newHead = buffer.pop();
  snake.pop();
  console.log(buffer);
  snake.unshift({ x: newHead.x, y: newHead.y });
  console.log(snake);
  ctx.clearRect(0, 0, width, height);
  drawApple();
  drawSnake();
  console.log(food.x,food.y,snake);
}
function checkBounds(row, col) {
  return !(col < 0 || col > numCols - 1 || row < 0 || row > numRows - 1);
}
function isBlocked(row, col) {
  return snake.some((e) => e.x === row && e.y === col);
}
function getNeighboursBFS(node) {
  // eslint-disable-next-line prefer-const
  let neigbours = [];
  const row = node.x;
  const col = node.y;
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
  return neigbours;
}
function constructPath(node) {
  while (node !== undefined) {
    buffer.push({ x: node.x, y: node.y });
    node = node.parent;
  }
}

function findpathBFS() {
  const openSet = [snake[0]];
  const closedSet = [];
  console.log(food.x, food.y, 'snake', snake[0].x, snake[0].y);
  // get head of snake
  while (openSet.length > 0) {
    const currentNode = openSet.shift();
    if (food.x === currentNode.x && food.y === currentNode.y) {
      constructPath(currentNode);
      break;
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
}
function findpathDFS() {
  // use stack here

}
function findpath_ASTAR() {

  // use openset and closeset here

}
function heuristic() {

}

function mainLoop() {
  findpathBFS();
  while (buffer.length > 0) {
    moveSnake();
  }
}

function init() {
  createSnake();
  placeFood();
  mainLoop();
}

function startGame() {
  init();
  // setInterval(draw, 1000);
}

startGame();
