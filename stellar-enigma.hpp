#ifndef STELLAR_ENIGMA_HPP
#define STELLAR_ENIGMA_HPP
#include <iostream>
#include <vector>
#include <deque>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "stellar.h"
#include "mode.h"
#include "level-util.h"
#include "audio.h"

class MapData {
public:
  Player& player;
  ModeSwitcher& modeSwitcher;
  Level& levelSlot;
  Menu& mainMenu;
  MusicPlayer& musicPlayer;
  MapData(Player*, ModeSwitcher*, Level*, Menu*, MusicPlayer*);
};

void initialSetup(std::string&, int&, int&, int&);
void customInit(MapData&);
void event0Handle(MapData&);
void event1Handle(MapData&);
void event2Handle(MapData&);
void event3Handle(MapData&);
void event4Handle(MapData&);
#endif
