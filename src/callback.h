#ifndef CALLBACK_H
#define CALLBACK_H

#include "stellar-enigma.hpp"

void nullBehave(Object* ob, Player* player, Level* level);

void solidBehave(Object& ob, MapData* md);

void pushBehave(Object& ob, MapData* md);

#endif
