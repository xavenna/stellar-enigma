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
  //read contents of save file
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
    //dump cutscene value to stderr:
    std::cerr << "name:"<<name<<"; events:";
    for(unsigned i=0;i<c.getListLen();i++) {
      Event e = c.getEvent(i);
      std::cerr << "{type:"<<e.getType()<<"; duration:"<<e.getDuration()<<"}\n";
      
    }
  }


  /*
  std::ifstream load;
  std::string line;
  std::string name;
  std::string compName;
  std::string nfn = "assets/cutscene/" + filename;
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
  */
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
