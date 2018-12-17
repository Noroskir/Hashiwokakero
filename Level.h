// Copyright 2018, Jonas Matuszak <jonas.matuszak@pluto.uni-freiburg.de>
// Using various code snippets kindy provided by
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#ifndef LEVEL_H_
#define LEVEL_H_

#include <gtest/gtest.h>
#include <ncurses.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include "./Island.h"

class Level {
 public:
  Level();
  ~Level();

  // load the level layout from a file
  void loadFilePlain(const char* filename);
  void loadFileXY(const char* filename);

  // display the game with ncurses
  void display() const;

  // check if all islands have the proper amount of bridges
  bool isSolved() const;

  // invoke appropriate action when clicking a certain cell
  // returns as in spawnBridge(), additional:
  // 1 if no island was clicked
  // 2 if the first or after a bridge was build
  int clickCell(int x, int y);

  // check if island is located at given coordinates
  // return the iterator to the island;
  std::vector<Island>::iterator checkForIsland(int x, int y);

  // undo the last build island
  void undo();

  // set the number of allowed undo's
  void setUndo(int x);

  // convert file-coordinates into scaled coordinates and back
  int scale(int a) const;
  int unscale(int a) const;


 private:
  // check if island has enough bridges and set appropriate color
  void colorIsland(Island* isl);

  // Spawn a bridge between two islands, if not possible return:
  // -1 if bridge would cross other bridge or island,
  // -2 if bridge can't be build straight,
  // else return 0;
  int spawnBridge(Island* a, Island* b);


  int _xSize;
  int _ySize;
  int _scale;
  int _offset;
  size_t _history;
  char** _grid;
  char** _colorGrid;
  std::vector<Island> _islands;
  std::vector<Island>::iterator _lastIsl;
  std::vector<std::pair<Island*, Island*>> _lastBuild;

  FRIEND_TEST(LevelTest, loadPlain);
  FRIEND_TEST(LevelTest, loadXY);
  FRIEND_TEST(LevelTest, spawnBridgeLegit);
  FRIEND_TEST(LevelTest, spawnBridgeCrossing);
  FRIEND_TEST(LevelTest, checkForIsland);
  FRIEND_TEST(LevelTest, clickCell);
  FRIEND_TEST(LevelTest, isSolved);
  FRIEND_TEST(LevelTest, colorIsland);
  FRIEND_TEST(LevelTest, undo);
};

#endif  // LEVEL_H_
