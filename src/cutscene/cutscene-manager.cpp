#include "cutscene/cutscene-manager.h"


bool CutsceneManager::loadCutscenes(const std::string& fn) {

  std::ifstream read(fn);
  std::string data;
  std::string err;
  if(!read.is_open()) {
    throw std::invalid_argument("CutsceneManager::loadCutscenes() : Error: Could not open config file");
  }
  getEntireFile(read, data);
  read.close();
  //read contents of file
  json11::Json save;
  save = save.parse(data, err);
  if(!err.empty()) {
    throw std::invalid_argument("CutsceneManager::loadCutscenes() : Json parse error: '"+err);
  }
  //parse json -- extract configs

  json11::Json::object k = save.object_items();

  json11::Json::array cutscenes = k["cutscenes"].array_items();

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
