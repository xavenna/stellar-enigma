#include "cutscene-manager.h"


bool CutsceneManager::loadCutscenes(const std::string& filename) {
  //for the future, this could use std::filesystem's directory iterator to find all cutscene files instead of using an external list.
  //this would need to use c++17 instead, though (i use c++11)
  std::ifstream load;
  std::string line;
  std::string name;
  std::string compName;
  load.open(filename);
  if(!load.is_open()) {
    //error
    return false;
  }
  while(load.peek() != EOF) {
    std::getline(load, line);
    compName = "assets/cutscene/" + line;
    Cutscene c;
    //attempt to load line into a cutscene
    if(c.loadFromFile(compName)) {
      name = line.substr(0,line.find("."));
      cutlist.emplace(name, c);
    }
    else {
      std::cout << "cutscene not loaded from file '" << line << "'\n";
    }
  }
  return true;
}



Cutscene CutsceneManager::getCutscene(const std::string& name) {
  if(cutlist.find(name) == cutlist.end()) {
    throw std::out_of_range("CutsceneManager::getCutscene() : Cutscene of that name does not exist");
  }
  return cutlist[name];
}
