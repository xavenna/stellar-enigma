#include "cutscene-manager.h"


bool CutsceneManager::loadCutscenes(const std::string& filename) {
  //for the future, this could use std::filesystem's directory iterator to find all cutscene files instead of using an external list.
  //this would need to use c++17 instead, though (i use c++11)
  std::ifstream load;
  std::string line;
  std::string name;
  std::string compName;
  std::string nfn = "assets/cutscene/" + filename + ".csl";
  load.open(nfn);
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
    std::string error = "CutsceneManager::getCutscene() : Cutscene of name '" +
      name + "' does not exist";
    throw std::out_of_range(error);
  }
  return cutlist[name];
}

bool CutsceneManager::cutsceneExists(const std::string& name) const {
  return cutlist.find(name)!=cutlist.end();
}
