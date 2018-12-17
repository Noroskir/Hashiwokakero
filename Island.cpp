// Copyright 2018, Jonas Matuszak <jonas.matuszak@pluto.uni-freiburg.de>
// Using various code snippets kindy provided by
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#include "./Island.h"

Island::Island(int x, int y, int bridges) {
  _x = x;
  _y = y;
  _bridges = bridges;
  _count = 0;
}

int Island::bridgeStatus() const {
  return _bridges >= _count ? _bridges == _count : -1;
}

int Island::getX() const {
  return _x;
}

int Island::getY() const {
  return _y;
}

void Island::addBridge() {
  _count++;
}

void Island::removeBridge() {
  _count--;
}
