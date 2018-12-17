// Copyright 2018, Jonas Matuszak <jonas.matuszak@pluto.uni-freiburg.de>
// Using various code snippets kindy provided by
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#include "./Level.h"
#include <string>
#include <vector>
#include <utility>

Level::Level() {
  _xSize = 0;
  _ySize = 0;
  _grid = nullptr;
  _colorGrid = nullptr;
  _scale = 5;
  _offset = 5;
  _history = 5;
  _lastIsl = _islands.end();
}

Level::~Level() {
  if (_grid != nullptr) {
    for (int i = 0; i < _ySize; i++) {
      delete[] _grid[i];
    }
    delete[] _grid;
  }
  if (_colorGrid != nullptr) {
    for (int i = 0; i < _ySize; i++) {
      delete[] _colorGrid[i];
    }
    delete[] _colorGrid;
  }
}

void Level::display() const {
  for (int i = 0; i < _ySize; i++) {
    for (int j = 0; j < _xSize; j++) {
      if ((_grid[i][j]-'0' > 0 && _grid[i][j]-'0' <= 8)
          | (_grid[i][j] == ' ')) {
        if (_colorGrid[i][j] == 0) {
          attron(A_REVERSE);
        mvprintw(i, j, &_grid[i][j]);
        attroff(A_REVERSE);
        } else {
          attron(COLOR_PAIR(_colorGrid[i][j]));
          attron(A_REVERSE);
          mvprintw(i, j, &_grid[i][j]);
          attroff(A_REVERSE);
          attroff(COLOR_PAIR(_colorGrid[i][j]));
        }
      } else if (_grid[i][j] != 0) {
        mvprintw(i, j, &_grid[i][j]);
      }
    }
  }
  mvprintw(0, 0, "Undo's left: %d", _history);
}

bool Level::isSolved() const {
  for (auto& isl : _islands) {
    if (isl.bridgeStatus() != 1) return false;
  }
  return true;
}

int Level::spawnBridge(Island* a, Island* b) {
  if (a->getX() == b->getX()) {  // vertical bridge
    int x = a->getX();
    int yMin = a->getY() < b->getY() ? a->getY() : b->getY();
    int yMax = a->getY() > b->getY() ? a->getY() : b->getY();
    // check if bridge would intersect something
    for (int i = yMin+2; i < yMax-1; i++) {
      if (_grid[i][x] != 0 && _grid[i][x] != '|') return -1;
    }
    for (int i = yMin+2; i < yMax-1; i++) {
      _grid[i][x] = _grid[i][x] == 0 ? '|' : 'H';
    }
    a->addBridge();
    b->addBridge();
    colorIsland(a);
    colorIsland(b);
    _lastBuild.push_back(std::make_pair(a, b));
    if (_lastBuild.size() > _history) _lastBuild.erase(_lastBuild.begin());
    return 0;
  } else if (a->getY() == b->getY()) {  // horizontal bridge
    int y = a->getY();
    int xMin = a->getX() < b->getX() ? a->getX() : b->getX();
    int xMax = a->getX() > b->getX() ? a->getX() : b->getX();
    // check for intersections
    for (int i = xMin+3; i < xMax-2; i++) {
      if (_grid[y][i] != 0 && _grid[y][i] != '-') return -1;
    }
    for (int i = xMin+3; i < xMax-2; i++) {
      _grid[y][i] = _grid[y][i] == 0 ? '-' : '=';
    }
    a->addBridge();
    b->addBridge();
    colorIsland(a);
    colorIsland(b);
    _lastBuild.push_back(std::make_pair(a, b));
    if (_lastBuild.size() > _history) _lastBuild.erase(_lastBuild.begin());
    return 0;
  }
  return -2;
}

int Level::clickCell(int x, int y) {
  std::vector<Island>::iterator curIsl = checkForIsland(x, y);
  if (curIsl != _islands.end()) {
    if (_lastIsl != _islands.end() && _lastIsl != curIsl) {
      int ret = spawnBridge(&(*_lastIsl), &(*curIsl));
      _lastIsl = curIsl = _islands.end();
      return ret;
    }
    _lastIsl = curIsl;
    return 2;
  }
  return 1;
}

std::vector<Island>::iterator Level::checkForIsland(int x, int y) {
  std::vector<Island>::iterator it = _islands.begin();
  while (it != _islands.end()) {
    if (x <= it->getX()+2 && x >= it->getX()-2
        && y <= it->getY()+1 && y >= it->getY()-1) {
      return it;
    }
    it++;
  }
  return it;
}

void Level::loadFilePlain(const char *filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error opening File " << filename << std::endl;
  }
  std::string line;
  std::getline(file, line);
  size_t pos1 = line.find(" ");
  size_t pos2 = line.find(":", pos1+1);
  size_t pos3 = line.find(" ", pos2+1);
  _xSize = scale(atoi(line.substr(pos1+1, pos2-pos1-1).c_str()));
  _ySize = scale(atoi(line.substr(pos2+1, pos3-pos2-1).c_str()));

  // allocate arrays
  _grid = new char*[_ySize];
  _colorGrid = new char*[_ySize];
  for (int i = 0; i < _ySize; i++) {
    _grid[i] = new char[_xSize];
    _colorGrid[i] = new char[_xSize];
    for (int j = 0; j < _xSize; j++) {
      _grid[i][j] = 0;
      _colorGrid[i][j] = 0;
    }
  }
  // fill islands into grid
  for (int i = 0; i < unscale(_ySize); i++) {
    std::getline(file, line);
    for (int j = 0; j < unscale(_xSize); j++) {
      if (line[j] != ' ') {
        int num = atoi(line.substr(j, 1).c_str());
        Island isl(scale(j), scale(i), num);
        _islands.push_back(isl);
        // make island look nice and big
        for (int k = -1; k < 2; k++) {
          for (int n = -2; n < 3; n++) {
            _grid[scale(i)+k][scale(j)+n] = ' ';
          }
        }
        _grid[scale(i)][scale(j)] = num + '0';
      }
    }
  }
  file.close();
  _lastIsl = _islands.end();
}

void Level::loadFileXY(const char *filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error opening File " << filename << std::endl;
  }
  std::string line;
  std::getline(file, line);
  size_t pos1 = line.find(" ");
  size_t pos2 = line.find(":", pos1+1);
  size_t pos3 = line.find(" ", pos2+1);
  _xSize = scale(atoi(line.substr(pos1+1, pos2-pos1-1).c_str()));
  _ySize = scale(atoi(line.substr(pos2+1, pos3-pos2-1).c_str()));

  // allocate arrays
  _grid = new char*[_ySize];
  _colorGrid = new char*[_ySize];
  for (int i = 0; i < _ySize; i++) {
    _grid[i] = new char[_xSize];
    _colorGrid[i] = new char[_xSize];
    for (int j = 0; j < _xSize; j++) {
      _grid[i][j] = 0;
      _colorGrid[i][j] = 0;
    }
  }
  // fill islands into grid
  while (std::getline(file, line)) {
    int x = scale(atoi(line.substr(0, 1).c_str()));
    int y = scale(atoi(line.substr(2, 1).c_str()));
    int bridges = atoi(line.substr(4, 1).c_str());
    Island isl(x, y, bridges);
    _islands.push_back(isl);
    // fatten the island
    for (int k = -1; k < 2; k++) {
      for (int n = -2; n < 3; n++) {
        _grid[y+k][x+n] = ' ';
      }
    }
    _grid[y][x] = bridges + '0';
  }
  file.close();
  _lastIsl = _islands.end();
}

int Level::scale(int a) const {
  return a * _scale + _offset;
}

int Level::unscale(int a) const {
  return (a - _offset)/_scale;
}

void Level::colorIsland(Island *isl) {
  // display color if island has enough bridges
  if (isl->bridgeStatus() == 1) {
    for (int k = -1; k < 2; k++) {
      for (int n = -2; n < 3; n++) {
        _colorGrid[isl->getY()+k][isl->getX()+n] = 1;
      }
    }
  } else if (isl->bridgeStatus() == -1) {
    for (int k = -1; k < 2; k++) {
      for (int n = -2; n < 3; n++) {
        _colorGrid[isl->getY()+k][isl->getX()+n] = 2;
      }
    }
  } else if (isl->bridgeStatus() == 0) {
    for (int k = -1; k < 2; k++) {
      for (int n = -2; n < 3; n++) {
        _colorGrid[isl->getY()+k][isl->getX()+n] = 0;
      }
    }
  }
}

void Level::undo() {
  if (_history > 0 && _lastBuild.size() > 0) {
    Island *a = _lastBuild.back().first;
    Island *b = _lastBuild.back().second;
    a->removeBridge();
    b->removeBridge();
    if (a->getX() == b->getX()) {  // vertical bridge
      int x = a->getX();
      int yMin = a->getY() < b->getY() ? a->getY() : b->getY();
      int yMax = a->getY() > b->getY() ? a->getY() : b->getY();
      for (int i = yMin+2; i < yMax-1; i++) {
        _grid[i][x] = _grid[i][x] == 'H' ? '|' : 0;
      }
      colorIsland(a);
      colorIsland(b);
    } else if (a->getY() == b->getY()) {  // horizontal bridge
      int y = a->getY();
      int xMin = a->getX() < b->getX() ? a->getX() : b->getX();
      int xMax = a->getX() > b->getX() ? a->getX() : b->getX();
      for (int i = xMin+3; i < xMax-2; i++) {
        _grid[y][i] = _grid[y][i] == '=' ? '-' : 0;
      }
      colorIsland(a);
      colorIsland(b);
    }
    _lastBuild.pop_back();
    _history--;
  }
}

void Level::setUndo(int x) {
  _history = x;
}
