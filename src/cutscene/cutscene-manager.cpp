#include "cutscene/cutscene-manager.h"


bool CutsceneManager::loadCutscenes(const std::string& fn) {

  std::ifstream read(fn);
  std::string data;
  if(!read.is_open()) {
    throw std::invalid_argument("CutsceneManager::loadCutscenes() : Error: Could not open config file");
  }
  getEntireFile(read, data);
  read.close();
  //read contents of file
  Json::Value save;

  Json::Reader reader;
  reader.parse(data, save);
  //parse json -- extract configs


  Json::Value cutscenes = save["cutscenes"];

  for(auto x : cutscenes) {
    Cutscene c;
    std::string name;
    c.loadFromFile(x, name);
    cutlist.emplace(name, c);
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
