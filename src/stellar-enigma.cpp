#include "stellar-enigma.hpp"

// This file has functions for the overarching classes

MapData::MapData(Player* pl, ModeSwitcher* ms, Level* ls, Menu* mm, MusicPlayer* mp, Message* msg) : player(*pl), modeSwitcher(*ms), levelSlot(*ls), mainMenu(*mm), musicPlayer(*mp), message(*msg) {
    
}
