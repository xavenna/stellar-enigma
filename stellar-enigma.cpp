#include "stellar-enigma.hpp"


MapData::MapData(Player* pl, ModeSwitcher* ms, Level* ls, Menu* mm, MusicPlayer* mp) : player(*pl), modeSwitcher(*ms), levelSlot(*ls), mainMenu(*mm), musicPlayer(*mp) {
    
}
