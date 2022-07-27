const puppeteer = require('puppeteer');
const fs = require('fs');
// clean this up
let csv = [];
const data = [{
  snakeLength: '', startX: '', startY: '', foodX: '', foodY: '', nodeCount: '', timeTaken: '',
}];
let counter = 0;
async function toCsv() {
  for (let row = 0; row < data.length; row++) {
    const keysAmount = Object.keys(data[row]).length;
    let keysCounter = 0;
    if (row === 0) {
      for (const key in data[row]) {
        csv += key + (keysCounter + 1 < keysAmount ? ',' : '\r\n');
        keysCounter++;
      }
    } else {
      for (const key in data[row]) {
        csv += data[row][key] + (keysCounter + 1 < keysAmount ? ',' : '\r\n');
        keysCounter++;
      }
    }

    keysCounter = 0;
  }
  fs.writeFileSync('./file.csv', csv);
}

async function registerConsoleEvent(page, i, res, rej) {
  page.on('console', async function consoleListener(msg) {
    try {
      // console.log(msg.text());
      if (msg.text().includes('game over')) {
        page.removeListener('console', consoleListener);
        res();
      } else if (msg.text().includes('snakeLength')) {
        counter++;
        const message = msg.text().split(' ');
        data[counter] = {};
        data[counter][message[0]] = message[1];
      } else {
        const message = msg.text().split(' ');
        data[counter][message[0]] = message[1];
      }
    } catch (e) {
      rej(e);
    }
  });
}
function waitForEndEvent(page, i) {
  return new Promise((res, rej) => {
    registerConsoleEvent(page, i, res, rej);
  });
}
async function runSnake(i) {
  const browser = await puppeteer.launch();
  const page = await browser.newPage();
  await page.goto('http://localhost:5500/cpp/app.html');
  await waitForEndEvent(page, i);
  await browser.close();
}

(async () => {
  for (let i = 0; i < 3; i++) {
    await runSnake();
  }
  await toCsv();
})();
