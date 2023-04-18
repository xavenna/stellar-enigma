#ifndef CALLBACK_H
#define CALLBACK_H

#include "stellar-enigma.hpp"

//! For objects that don't interact with player
sf::Vector2i nullBehave(Object* ob, MapData* md, bool dryRun = false);

//! For objects that block player motion
sf::Vector2i solidBehave(Object& ob, MapData* md, bool dryRun = false);

sf::Vector2i toggleBehave(Object& ob, MapData* md, bool dryRun = false);

//! For objects that can be pushed by player
void pushBehave(Object& ob, MapData* md);

//! For objects that hurt the player
void hurtBehave(Object& ob, MapData* md);

#endif
