#include "stellar-enigma.hpp"

// This file has functions for the overarching classes

MapData::MapData(unsigned pCool, unsigned mWid, unsigned mCool, unsigned mElem, unsigned mCharSize) : player{pCool}, message{mWid, mCool, mElem, mCharSize} {
  
}

void MapData::handleEvents() {
  switch(modeSwitcher.getMode()) {
  case 0:
    event0Handle();
    break;
  case 1:
    event1Handle();
    break;
  case 2:
    event2Handle();
    break;
  case 3:
    event3Handle();
    break;
  case 4:
    //event4Handle();
    break;
  default:
    //don't handle any events... in an unrecognised mode
    break;

  }
}
