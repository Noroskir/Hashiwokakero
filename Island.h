// Copyright 2018, Jonas Matuszak <jonas.matuszak@pluto.uni-freiburg.de>
// Using various code snippets kindy provided by
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#ifndef ISLAND_H_
#define ISLAND_H_

#include <gtest/gtest.h>

class Island {
 public:
  Island(int x, int y, int bridges);

  // check if the proper amount of bridges is build on island
  // return 0 if too few,
  // return -1 if to many
  // return 1 if correct amount
  int bridgeStatus() const;
  int getX() const;
  int getY() const;

  // increase/decrease bridge counter
  void addBridge();
  void removeBridge();

 private:
  int _x;
  int _y;
  int _bridges;
  int _count;

  FRIEND_TEST(IslandTest, bridgeStatus);
  FRIEND_TEST(IslandTest, addRemoveBridge);
};

#endif  // ISLAND_H_
