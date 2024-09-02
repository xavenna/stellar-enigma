#ifndef FILE_IO_H
#define FILE_IO_H

#include <array>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <SFML/Graphics.hpp>

#include "stellar-enigma.hpp"
#include "editor/editor.h"

namespace fs = std::filesystem;

namespace ed {
  //modified from level loading code
  bool str2obj(const std::string& line, ObjectBase& obj, std::string& objType);
  int loadMutableList(std::vector<ObjectBase>& objs, const std::string& filename);

  bool writeListToFile(std::vector<ObjectBase>& objs, const std::string& filename, std::string& status);

}


#endif
