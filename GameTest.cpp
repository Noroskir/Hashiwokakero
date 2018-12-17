// Copyright 2018, Jonas Matuszak <jonas.matuszak@pluto.uni-freiburg.de>
// Using various code snippets kindy provided by
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#include <gtest/gtest.h>
#include "./Game.h"

TEST(GameTest, parseCommandLineArgsNoArgs) {
  Game g;
  int argc = 1;
  char* argv[1] = {const_cast<char*>("")};
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_DEATH(g.parseCommandLineArgs(argc, argv), "Usage: .*");
}

TEST(GameTest, parseCommandLineArgs) {
  Game g;
  int argc = 2;
  char* argv[2] = {
    const_cast<char*>(""),
    const_cast<char*>("veryNiceInputFile")
  };
  g.parseCommandLineArgs(argc, argv);
  ASSERT_STREQ("veryNiceInputFile", g._inputFileName);
  ASSERT_EQ(5, g._undo);
}

TEST(GameTest, parseCommandLineArgsSetUndo) {
  Game g;
  int argc = 4;
  char* argv[4] = {
    const_cast<char*>(""),
    const_cast<char*>("-u"),
    const_cast<char*>("666"),
    const_cast<char*>("veryNiceInputFile")
  };
  g.parseCommandLineArgs(argc, argv);
  ASSERT_STREQ("veryNiceInputFile", g._inputFileName);
  ASSERT_EQ(666, g._undo);
}
