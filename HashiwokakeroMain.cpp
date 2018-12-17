// Copyright 2018, Jonas Matuszak <jonas.matuszak@pluto.uni-freiburg.de>
// Using various code snippets kindy provided by
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#include <iostream>
#include "./Game.h"

int main(int argc, char** argv) {
  Game game;
  game.parseCommandLineArgs(argc, argv);
  game.initTerminal();
  game.run();
  return 0;
}
