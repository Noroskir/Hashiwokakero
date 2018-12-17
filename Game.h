// Copyright 2018, Jonas Matuszak <jonas.matuszak@pluto.uni-freiburg.de>
// Using various code snippets kindy provided by
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#ifndef GAME_H_
#define GAME_H_

#include <gtest/gtest.h>
#include <ncurses.h>
#include <getopt.h>
#include <iostream>
#include <string>
#include "./Level.h"
#include "./Solution.h"

class Game {
 public:
  Game();
  ~Game();

  // initialize ncurses
  void initTerminal();

  // game loop
  void run();

  // parse command line arguments
  void parseCommandLineArgs(int argc, char **argv);

 private:
  // call the correct Level.load function depending
  // on the file format of _inputFileName.
  void loadLevel();

  // display the game with ncurses
  void display() const;

  // end game with 'q'
  // undo with 'z'
  void handleInput();

  // print usage and exit
  void printUsageAndExit() const;

  // display victory screen
  void displayVictory() const;

  // display wrong solution
  void displayWrongSolution() const;

  bool _running;
  bool _levelSolved;
  int _lastClickX;
  int _lastClickY;
  int _undo;
  const char *_inputFileName = "";
  Level _level;
  Solution *_solution;

  FRIEND_TEST(GameTest, parseCommandLineArgsNoArgs);
  FRIEND_TEST(GameTest, parseCommandLineArgs);
  FRIEND_TEST(GameTest, parseCommandLineArgsSetUndo);
};

#endif  // GAME_H_
