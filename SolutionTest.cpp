// Copyright 2018, Jonas Matuszak <jonas.matuszak@pluto.uni-freiburg.de>
// Using various code snippets kindy provided by
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#include <gtest/gtest.h>
#include <fstream>
#include "./Solution.h"
#include "./Level.h"

TEST(SolutionTest, loadSolutionXY) {
  std::ofstream file("solutionTestXY.xy.solution");
  file << "# (xy.solution)\n";
  file << "0,0,0,0\n";
  file << "10,11,12,13\n";
  file << "3,123,0,42\n";
  file.close();
  Solution s(nullptr);
  s.loadSolutionXY("solutionTestXY.xy.solution");
  ASSERT_EQ(0, s._clicks[0].first);
  ASSERT_EQ(0, s._clicks[0].second);
  ASSERT_EQ(0, s._clicks[1].first);
  ASSERT_EQ(0, s._clicks[1].second);
  ASSERT_EQ(12, s._clicks[3].first);
  ASSERT_EQ(123, s._clicks[4].second);
  ASSERT_EQ(42, s._clicks.back().second);
  std::remove("solutionTestXY.xy.solution");
}

TEST(SolutionTest, playSolution) {
  std::ofstream file("test.xy");
  file << "# 4:4 (xy)\n";
  file << "0,0,1\n";
  file << "3,0,2\n";
  file << "3,3,1\n";
  file.close();
  file.open("test.xy.solution");
  file << "# (xy.solution)\n";
  file << "# x1,y1,x2,x2\n";
  file << "# Single bridge\n";
  file << "0,0,3,0\n";
  file << "3,0,3,3\n";
  file.close();
  Level l;
  l.loadFileXY("test.xy");
  Solution s(&l);
  s.loadSolutionXY("test.xy.solution");
  s.playSolution();
  ASSERT_EQ(true, l.isSolved());
  std::remove("test.xy");
  std::remove("test.xy.solution");
}
