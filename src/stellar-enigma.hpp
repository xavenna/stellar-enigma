#ifndef STELLAR_ENIGMA_HPP
#define STELLAR_ENIGMA_HPP

// This is the header to include for stellar-enigma

#include <iostream>
#include <vector>
#include <deque>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "direction.h"
#include "mutable.h"
#include "object.h"
#include "entity.h"
#include "player.h"
#include "message.h"
#include "mapnode.h"
#include "texturemap.h"
#include "level.h"
#include "cutsceneplayer.h"
#include "menu.h"
#include "mapdata.h"
#include "mode.h"
#include "level-util.h"
#include "audio.h"


void initialSetup(std::string&, int&, int&, int&);


#endif
