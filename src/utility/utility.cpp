#include "utility/utility.h"


unsigned Utility::getFrame() {
  return counter;
}
void Utility::nextFrame() {
  counter++;
}

int Utility::getRandInt(int m, int n) {
  std::uniform_int_distribution<int> dist(m, n);
  return dist(engine);
}
float Utility::getRandFloat(float m, float n) {
  std::uniform_real_distribution<float> dist(m, n);
  return dist(engine);
}

#ifndef RAND_USE_TIME
Utility::Utility() : engine{rd()} {
}
#else
Utility::Utility() : engine(std::chrono::system_clock::now().time_since_epoch().count()) {
}
#endif
// DO NOT MOVE THIS ENDIF

bool Utility::loadPaths(const std::string& lname) {
  std::string complevel = "assets/level/" + lname + ".path";

  std::ifstream read(complevel);
  std::string data;
  if(!read.is_open()) {
    std::clog << "Error: Could not open path list\n";
    return false;
  }
  getEntireFile(read, data);
  read.close();
  //read contents of save file
  Json::Value save;

  Json::Reader reader;
  reader.parse(data, save);

  Json::Value paths = save["paths"];
  // TODO: ADD MORE ERROR CHECKING

  for(auto array : paths) {
    Path p;
    //for each path

    if(!array.isArray() || array.size() < 2) {
      std::cerr << "Error: missing path geometry\n";
      return false;
    }

    p.segments.reserve(array.size()-1);
    for(int i=1;i<array.size();i++) {
      //The iterator is shingled: since each point is shared between two segments, 
      //each loop uses array[i-1] and array[i]
      const auto& x = array[i-1];
      const auto& y = array[i];
      // x should be an array with 2 elements
      if(!x.isObject() || !y.isObject()) {
        std::cerr << "Error: Invalid path point\n";
        return false;
      }
      if( !x["c1"].isArray() || !x["pt"].isArray() || !x["c2"].isArray() ||
          !y["c1"].isArray() || !y["pt"].isArray() || !y["c2"].isArray() ) {
        std::cerr << "Error: Invalid path point\n";
        return false;
      }

      PathSeg ps({x["pt"][0].asFloat(), x["pt"][1].asFloat()}, {y["pt"][0].asFloat(), y["pt"][1].asFloat()}, {x["c2"][0].asFloat(), x["c2"][1].asFloat()}, {y["c1"][0].asFloat(), y["c1"][1].asFloat()} );
      p.segments.push_back(ps);

    }
    pathList.push_back(p);

  }
  return true;
}
