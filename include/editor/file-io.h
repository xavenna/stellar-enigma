#ifndef FILE_IO_H
#define FILE_IO_H

#include <array>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <json11.hpp>

#include <SFML/Graphics.hpp>

#include "stellar-enigma.hpp"
#include "editor/editor.h"

namespace fs = std::filesystem;

namespace ed {

  struct ObjInfo {
    std::string parent;
    std::string type;
    std::array<std::string, 8> args;
    std::array<std::string, 8> switches;
    std::string textArg;
    sf::Vector2f size;
    unsigned priority;
    std::string shortName;
    std::string longName;
    bool use;
    bool interact;
    bool texture;
    bool invisible;
    std::string description;
  };

  struct Database {
    std::string version;
    void append(const ObjInfo&);
    void reserve(unsigned);
    ObjInfo operator[](std::string);
    bool contains(const std::string&) const;
    std::string get_full_entry(const std::string&);
  private:
    std::map<std::string, ObjInfo> data;
  };

  //modified from level loading code
  bool str2obj(const std::string& line, ObjectBase& obj, std::string& objType);
  int loadMutableList(std::vector<ObjectBase>& objs, const std::string& filename);

  bool writeListToFile(std::vector<ObjectBase>& objs, const std::string& filename, std::string& status);
  bool parse_db(const std::string& fn, Database& d);

  bool generateObjInfo(const json11::Json& obj, ObjInfo& o, std::string& err);

}


#endif
