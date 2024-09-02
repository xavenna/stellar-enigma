#ifndef EDITOR_H
#define EDITOR_H

#include <array>
#include <string>
#include <vector>
#include <iostream>
#include <curses.h>
#include <filesystem>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include "misc/util.h"
#include "level/obj-container.h"

namespace fs=std::filesystem;

//contains only data members of object
struct ObjectBase {
  std::array<int, 8> args;
  std::array<int, 8> switches;
  sf::Vector2f pos;
  sf::Vector2f size;
  int link_id=-1;
  int texture_id=-1;
  int parent_id=-1;
  std::string text;
  std::string type;
};

std::string objToStr(ObjectBase&);

enum Mode {
  Normal,
  Command,
  Field,
  List,
};

namespace ed {
  //modified from level loading code
  bool str2obj(const std::string& line, ObjectBase& obj, std::string& objType);
  int loadMutableList(std::vector<ObjectBase>& objs, const std::string& filename);

  // editor-specific functions
  bool isValidCommand(const std::string&);
  bool executeCommand(const std::string&, std::string&, std::string&, std::vector<ObjectBase>&, std::string&, WINDOW*, int&);
  std::string objToStr(ObjectBase& o);
  std::string objToStrFull(ObjectBase& o);
  bool setAttr(ObjectBase& obj, const std::string& arg1, const std::string& arg2, std::string& status);
  bool writeListToFile(std::vector<ObjectBase>& objs, const std::string& filename, std::string& status);

  bool isValidObjType(const std::string&);
  bool verifyObjList(std::vector<ObjectBase>& objs, std::string& status, WINDOW*); 
}

#endif
