#pragma once
#ifndef GAMESETTINGS
#define GAMESETTINGS

static const int SCREEN_WIDTH = 640;
static const int SCREEN_HEIGHT = 480;

static const int NUM_ROWS = 10;  // change here to desired value
static const int NUM_COLLS = 10; // change here to desired value

static const int CELL_WIDTH = SCREEN_WIDTH / NUM_COLLS;
static const int CELL_HEIGHT = SCREEN_HEIGHT / NUM_ROWS;
#endif