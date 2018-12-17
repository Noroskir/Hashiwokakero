// Copyright 2018, Jonas Matuszak <jonas.matuszak@pluto.uni-freiburg.de>
// Using various code snippets kindy provided by
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#include <gtest/gtest.h>
#include <vector>
#include "./Level.h"

TEST(LevelTest, loadPlain) {
  std::ofstream file("plainTest.txt");
  file << "# 4:4 (plain)\n";
  file << "3 1 \n";
  file << "    \n";
  file << "2   \n";
  file << "    \n";
  file.close();
  Level l;
  l.loadFilePlain("plainTest.txt");
  ASSERT_EQ('3', l._grid[l.scale(0)][l.scale(0)]);
  ASSERT_EQ(0, l._grid[l.scale(1)][l.scale(1)]);
  ASSERT_EQ('1', l._grid[l.scale(0)][l.scale(2)]);
  ASSERT_EQ('2', l._grid[l.scale(2)][l.scale(0)]);
  ASSERT_EQ(3, l._islands.size());
  std::remove("plainTest.txt");
}

TEST(LevelTest, loadXY) {
  std::ofstream file("xyTest.txt");
  file << "# 4:4 (xy)\n";
  file << "0,0,3\n";
  file << "2,0,1\n";
  file << "0,2,2\n";
  file.close();
  Level l;
  l.loadFileXY("xyTest.txt");
  ASSERT_EQ('3', l._grid[l.scale(0)][l.scale(0)]);
  ASSERT_EQ(0, l._grid[l.scale(1)][l.scale(1)]);
  ASSERT_EQ('1', l._grid[l.scale(0)][l.scale(2)]);
  ASSERT_EQ('2', l._grid[l.scale(2)][l.scale(0)]);
  ASSERT_EQ(3, l._islands.size());
  std::remove("xyTest.txt");
}

TEST(LevelTest, spawnBridgeLegit) {
  std::ofstream file("xyTest.txt");
  file << "# 4:4 (xy)\n";
  file << "0,0,3\n";
  file << "2,0,1\n";
  file << "0,2,2\n";
  file.close();
  Level l;
  l.loadFileXY("xyTest.txt");
  ASSERT_EQ(0, l.spawnBridge(&l._islands[0], &l._islands[2]));
  ASSERT_EQ('|', l._grid[l.scale(1)][l.scale(0)]);
  ASSERT_EQ(0, l.spawnBridge(&l._islands[0], &l._islands[2]));
  ASSERT_EQ('H', l._grid[l.scale(1)][l.scale(0)]);
  ASSERT_EQ(0, l.spawnBridge(&l._islands[0], &l._islands[1]));
  ASSERT_EQ('-', l._grid[l.scale(0)][l.scale(1)]);
  ASSERT_EQ(0, l.spawnBridge(&l._islands[0], &l._islands[1]));
  ASSERT_EQ('=', l._grid[l.scale(0)][l.scale(1)]);
  std::remove("xyTest.txt");
}

TEST(LevelTest, spawnBridgeCrossing) {
  std::ofstream file("plainTest.txt");
  file << "# 5:3 (plain)\n";
  file << "2 1 2\n";
  file << " 1 1 \n";
  file << "  1  \n";
  file.close();
  Level l;
  l.loadFilePlain("plainTest.txt");
  ASSERT_EQ(-1, l.spawnBridge(&l._islands[0], &l._islands[2]));
  l.spawnBridge(&l._islands[3], &l._islands[4]);
  ASSERT_EQ(-1, l.spawnBridge(&l._islands[1], &l._islands[5]));
  ASSERT_EQ(-2, l.spawnBridge(&l._islands[0], &l._islands[5]));
  std::remove("plainTest.txt");
}

TEST(LevelTest, checkForIsland) {
  std::ofstream file("plainTest.txt");
  file << "# 4:4 (plain)\n";
  file << "3 1 \n";
  file << "    \n";
  file << "2   \n";
  file << "    \n";
  file.close();
  Level l;
  l.loadFilePlain("plainTest.txt");
  std::vector<Island>::iterator it = l._islands.begin();
  ASSERT_EQ(it, l.checkForIsland(l.scale(0), l.scale(0)));
  ASSERT_EQ(it, l.checkForIsland(l.scale(0)+2, l.scale(0)+1));
  ASSERT_EQ(it, l.checkForIsland(l.scale(0)+2, l.scale(0)-1));
  ASSERT_EQ(it, l.checkForIsland(l.scale(0)-1, l.scale(0)));
  ASSERT_EQ(l._islands.end(), l.checkForIsland(l.scale(0)+3, l.scale(0)+2));

  it++;
  ASSERT_EQ(it, l.checkForIsland(l.scale(2), l.scale(0)));
  ASSERT_EQ(it, l.checkForIsland(l.scale(2)+2, l.scale(0)));
  ASSERT_EQ(it, l.checkForIsland(l.scale(2)+2, l.scale(0)-1));
  ASSERT_EQ(it, l.checkForIsland(l.scale(2)-2, l.scale(0)-1));
  std::remove("plainTest.txt");
}

TEST(LevelTest, clickCell) {
  std::ofstream file("plainTest.txt");
  file << "# 4:4 (plain)\n";
  file << "3 1 \n";
  file << "    \n";
  file << "2   \n";
  file << "    \n";
  file.close();
  Level l;
  l.loadFilePlain("plainTest.txt");
  ASSERT_EQ(1, l.clickCell(l.scale(0)+3, l.scale(0)));
  ASSERT_EQ(2, l.clickCell(l.scale(0), l.scale(0)));
  ASSERT_EQ(0, l.clickCell(l.scale(2), l.scale(0)));
  ASSERT_EQ(2, l.clickCell(l.scale(0)-1, l.scale(0)+1));
  ASSERT_EQ(0, l.clickCell(l.scale(0)+2, l.scale(2)-1));

  ASSERT_EQ(2, l.clickCell(l.scale(2), l.scale(0)));
  ASSERT_EQ(-2, l.clickCell(l.scale(0), l.scale(2)));
  std::remove("plainTest.txt");
}

TEST(LevelTest, isSolved) {
  std::ofstream file("plainTest.txt");
  file << "# 4:4 (plain)\n";
  file << "3 1 \n";
  file << "    \n";
  file << "2   \n";
  file << "    \n";
  file.close();
  Level l;
  l.loadFilePlain("plainTest.txt");
  ASSERT_EQ(false, l.isSolved());

  l.spawnBridge(&l._islands[0], &l._islands[1]);
  ASSERT_EQ(false, l.isSolved());
  l.spawnBridge(&l._islands[0], &l._islands[2]);
  l.spawnBridge(&l._islands[0], &l._islands[2]);
  ASSERT_EQ(true, l.isSolved());
  std::remove("plainTest.txt");
}

TEST(LevelTest, colorIsland) {
  std::ofstream file("plainTest.txt");
  file << "# 4:4 (plain)\n";
  file << "3 1 \n";
  file << "    \n";
  file << "2   \n";
  file << "    \n";
  file.close();
  Level l;
  l.loadFilePlain("plainTest.txt");
  l.colorIsland(&l._islands[2]);
  ASSERT_EQ(0, l._colorGrid[l.scale(2)][l.scale(0)]);
  l._islands[2].addBridge();
  l.colorIsland(&l._islands[2]);
  ASSERT_EQ(0, l._colorGrid[l.scale(2)][l.scale(0)]);
  l._islands[2].addBridge();
  l.colorIsland(&l._islands[2]);
  ASSERT_EQ(1, l._colorGrid[l.scale(2)][l.scale(0)]);
  l._islands[2].addBridge();
  l.colorIsland(&l._islands[2]);
  ASSERT_EQ(2, l._colorGrid[l.scale(2)][l.scale(0)]);
  std::remove("plainTest.txt");
}

TEST(LevelTest, undo) {
  std::ofstream file("plainTest.txt");
  file << "# 4:4 (plain)\n";
  file << "3 1 \n";
  file << "    \n";
  file << "2   \n";
  file << "    \n";
  file.close();
  Level l;
  l.loadFilePlain("plainTest.txt");
  l.spawnBridge(&l._islands[0], &l._islands[2]);
  l.spawnBridge(&l._islands[0], &l._islands[1]);
  l.undo();
  ASSERT_EQ(0, l._grid[l.scale(0)][l.scale(1)]);
  ASSERT_EQ(0, l._colorGrid[l.scale(0)][l.scale(2)]);
  l.undo();
  ASSERT_EQ(0, l._grid[l.scale(1)][l.scale(0)]);
  ASSERT_EQ(0, l._colorGrid[l.scale(2)][l.scale(0)]);
  std::remove("plainTest.txt");
}
