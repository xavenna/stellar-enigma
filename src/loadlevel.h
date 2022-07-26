#ifndef LOADLEVEL_H
#define LOADLEVEL_H
#include <fstream>
#include <string>
#include "stellar.h"

//I don't think anything uses this, as Level now has a load method
void loadLevel(Level&, const std::string&);

#endif
