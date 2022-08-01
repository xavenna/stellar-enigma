#include "stellar-enigma.hpp"

// This file has functions for the overarching classes

//this should probably be moved to mapdata.cpp

MapData::MapData(Player* pl, ModeSwitcher* ms, Level* ls, Menu* mm, MusicPlayer* mp, Message* msg, CutscenePlayer* cp, CutsceneManager* cm) : player(*pl), modeSwitcher(*ms), levelSlot(*ls), mainMenu(*mm), musicPlayer(*mp), message(*msg), cutscenePlayer(*cp), cutsceneManager(*cm) {
    
}
