// Copyright 2018, Jonas Matuszak <jonas.matuszak@pluto.uni-freiburg.de>
// Using various code snippets kindy provided by
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#ifndef SOLUTION_H_
#define SOLUTION_H_

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include "./Level.h"

class Solution {
 public:
  explicit Solution(Level *level);

  // load the solution from .xy file format
  void loadSolutionXY(const char* filename);

  // apply the solution to level in one go
  // return true if successful
  // else return false.
  bool playSolution();

 private:
  std::vector<std::pair<int, int>> _clicks;
  std::vector<std::pair<int, int>>::iterator _step;
  Level* _level;

  FRIEND_TEST(SolutionTest, loadSolutionXY);
};

#endif  // SOLUTION_H_
