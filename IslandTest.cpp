// Copyright 2018, Jonas Matuszak <jonas.matuszak@pluto.uni-freiburg.de>
// Using various code snippets kindy provided by
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#include <gtest/gtest.h>
#include "./Island.h"

TEST(IslandTest, bridgeStatus) {
  Island i(0, 0, 3);
  ASSERT_EQ(0, i.bridgeStatus());
  i.addBridge();
  i.addBridge();
  ASSERT_EQ(0, i.bridgeStatus());
  i.addBridge();
  ASSERT_EQ(1, i.bridgeStatus());
  i.addBridge();
  ASSERT_EQ(-1, i.bridgeStatus());
}

TEST(IslandTest, addRemoveBridge) {
  Island i(0, 0, 1);
  ASSERT_EQ(0, i._count);
  i.addBridge();
  i.addBridge();
  ASSERT_EQ(2, i._count);
  i.removeBridge();
  ASSERT_EQ(1, i._count);
}
