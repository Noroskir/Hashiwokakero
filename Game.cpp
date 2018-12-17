// Copyright 2018, Jonas Matuszak <jonas.matuszak@pluto.uni-freiburg.de>
// Using various code snippets kindy provided by
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#include <getopt.h>
#include <cstdlib>
#include <string>
#include "./Game.h"

Game::Game() {
  _running = true;
  _levelSolved = false;
  _lastClickX = 0;
  _lastClickY = 0;
  _undo = 5;
  _solution = nullptr;
}

Game::~Game() {
  if (_solution != nullptr) delete _solution;
  endwin();
}

void Game::initTerminal() {
  // init ncurses
  initscr();
  start_color();
  cbreak();
  noecho();
  curs_set(false);
  nodelay(stdscr, true);
  keypad(stdscr, true);
  mousemask(ALL_MOUSE_EVENTS, NULL);

  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
}

void Game::run() {
  loadLevel();
  while (_running) {
    display();
    handleInput();
  }
}

void Game::display() const {
  refresh();
  _level.display();
  if (_levelSolved) displayVictory();
}

void Game::handleInput() {
  int key = getch();
  MEVENT event;
  switch (key) {
    case 'q':
      _running = false;
      break;
    case 'z':
      if (_undo > 0) {
        _level.undo();
        clear();
      }
      break;
    case KEY_MOUSE:
      if (getmouse(&event) == OK && (event.bstate & BUTTON1_CLICKED)) {
        _lastClickX = event.x;
        _lastClickY = event.y;
        _level.clickCell(_lastClickX, _lastClickY);
        _levelSolved = _level.isSolved();
      }
      break;
  }
}

void Game::parseCommandLineArgs(int argc, char **argv) {
  struct option options[] = {
    { "undo", 1, NULL, 'u' },
    { NULL, 0, NULL, 0}
  };
  optind = 1;
  while (true) {
    char c = getopt_long(argc, argv, "u:", options, NULL);
    if (c == -1 ) break;
    switch (c) {
      case 'u':
        _undo = atof(optarg);
        _level.setUndo(_undo);
        break;
    }
  }
  if (optind + 1 != argc) printUsageAndExit();
  _inputFileName = argv[optind];
}

void Game::loadLevel() {
  std::string name = _inputFileName;
  size_t pos = name.find(".");
  name = name.substr(pos+1);
  if (name == "plain") {
    _level.loadFilePlain(_inputFileName);
  } else if (name == "xy") {
    _level.loadFileXY(_inputFileName);
  } else if (name == "xy.solution") {
    _solution = new Solution(&_level);
    name = _inputFileName;
    _solution->loadSolutionXY(name.c_str());
    pos = name.find(".solution");
    _level.loadFileXY(name.substr(0, pos).c_str());
    if (_solution->playSolution()) _levelSolved = true;
    else
      displayWrongSolution();
  }
}

void Game::displayWrongSolution() const {
  int x = COLS/4;
  int y = LINES/3;
  mvprintw(y, x, "################");
  mvprintw(y+1, x, "#  Solution    #");
  mvprintw(y+2, x, "#     not      #");
  mvprintw(y+3, x, "#   working    #");
  mvprintw(y+4, x, "################");
}

void Game::printUsageAndExit() const {
  endwin();
  std::cerr << "Usage: ./HashiwokakeroMain [options] <filename>\n"
            << "-u, --undo: number of undo's (default 5)\n";
  exit(1);
}

void Game::displayVictory() const {
  int x = COLS/4;
  int y = LINES/3;
  mvprintw(y, x, "################");
  mvprintw(y+1, x, "#              #");
  mvprintw(y+2, x, "#  VICTORIOUS  #");
  mvprintw(y+3, x, "#              #");
  mvprintw(y+4, x, "################");
}
