#ifndef CALLBACK_H
#define CALLBACK_H

#include "stellar-enigma.hpp"

//! For objects that don't interact with player
void nullBehave(Object* ob, Player* player, Level* level);

//! For objects that block player motion
void solidBehave(Object& ob, MapData* md);

//! For objects that can be pushed by player
void pushBehave(Object& ob, MapData* md);

//! For objects that hurt the player
void hurtBehave(Object& ob, MapData* md);

#endif
