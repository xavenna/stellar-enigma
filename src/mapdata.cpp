#include "stellar-enigma.hpp"

// This file has functions for the overarching classes

//this should probably be moved to mapdata.cpp

MapData::MapData(unsigned pCool, unsigned mWid, unsigned mCool, unsigned mElem, unsigned mCharSize) : player{pCool}, message{mWid, mCool, mElem, mCharSize} {
  
}
