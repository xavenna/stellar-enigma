#ifndef CUTSCENE_MANAGER_H
#define CUTSCENE_MANAGER_H
#include "cutscene.h"
#include <map>
#include <stdexcept>
#include <fstream>

//! A class that loads and stores cutscenes for use by a CutscenePlayer
class CutsceneManager {
private:
  std::map<std::string, Cutscene> cutlist;
public:
  bool loadCutscenes(const std::string& filename);
  Cutscene getCutscene(const std::string& name);
};


#endif
