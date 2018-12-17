// Copyright 2018, Jonas Matuszak <jonas.matuszak@pluto.uni-freiburg.de>
// Using various code snippets kindy provided by
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include "./Solution.h"

Solution::Solution(Level* level) {
  _level = level;
  _step = _clicks.begin();
}

void Solution::loadSolutionXY(const char *filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error opening File " << filename << std::endl;
  }
  std::string line;
  while (std::getline(file, line)) {
    if (line[0] != '#') {
      std::stringstream ss(line);
      std::string elem;
      std::vector<std::string> pos;
      while (std::getline(ss, elem, ',')) {
        pos.push_back(elem);
      }
      _clicks.push_back(std::make_pair(std::stoi(pos[0]), std::stoi(pos[1])));
      _clicks.push_back(std::make_pair(std::stoi(pos[2]), std::stoi(pos[3])));
    }
  }
  file.close();
}

bool Solution::playSolution() {
  _step = _clicks.begin();
  while (_step != _clicks.end()) {
    int res = _level->clickCell(_level->scale(_step->first),
                                _level->scale(_step->second));
    if (res < 0) {
      return false;
    }
    _step++;
  }
  return true;
}
