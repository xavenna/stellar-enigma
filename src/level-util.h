#ifndef LEVEL_UTIL_H
#define LEVEL_UTIL_H
#include "level.h"
#include "player.h"

//Documented

//! Calculates how far player can move in specified direction (0 if can't move)
int validMove(Level&, Player&);
//! Returns true if player can move into space with specified id
bool passableSpace(int);

#endif
