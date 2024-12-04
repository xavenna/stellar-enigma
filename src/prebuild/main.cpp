#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>
#include "misc/util.h"

namespace fs = std::filesystem;
// This module runs prebuild tasks
// In particular, it generates the code for ObjContainer::storeObj
// It reads all files in include/mutable/, filters out player, and creates the function

/*
 * function template:
bool ObjContainer::storeObj(Object ob, std::string t) {
  if(t == "obj") {
    list.push_back(new Object(counter));
  }
  else if(t == "name") {
    list.push_back(new Name(counter));
  }

  ...

  else {
    std::clog << "Error: Invalid object type\n";
    return false;
  }
  //copy all externally-set attributes of the passed object to list.back();
  list.back()->setLinkID(ob.getLinkID());
  list.back()->setTextureID(ob.getTextureID());
  list.back()->setParentID(ob.getParentID());
  list.back()->setPos(ob.getPos());
  list.back()->setSize(ob.getSize());
  list.back()->setSolid(ob.getSolid());
  list.back()->setText(ob.getText());
  list.back()->setStatus(Object::Normal);

  list.back()->setArgs(ob.getArgs());
  list.back()->setSwitches(ob.getSwitches());

  //now that object is set up properly, verify it.
  if(!list.back()->verify()) {
    std::clog << "Error: Invalid object configuration in object of type '";
    std::clog << list.back()->Name() << "'. Object wasn't added\n";
    removeObj(list.back());   
    return false;
  }
  counter++;
  return true;
}
*/

struct Entry {
  public:
    std::string name;  //Class name
    std::string code;  //Object's Name() return value
};

//searches for definining lines, extract information
bool parseFile(const std::string& fn, Entry& e) {
  std::ifstream load(fn);
  std::string line;
  if(!load.is_open()) {
    return false;
  }

  bool foundClass = false;
  bool foundName = false;

  while(load.peek() != EOF && !(foundClass && foundName)) {
    //check line for matches:
    std::getline(load, line);
    if(!foundClass && line.find("class ") == 0 && line.find(';') == std::string::npos) { //class must be at start of line
      //class declaration line
      size_t offset = line.find("class ") + 6; // 6 is width of "class "
      e.name = line.substr(offset, line.find(" ", offset)-offset); //gets from offset to next " "
      foundClass = true;
    }
    else if(!foundName && line.find("virtual std::string Name() const") != std::string::npos) {
      //Name() definition line
      size_t offset = line.find("return \"") + 8; //8 is width of "return \""
      e.code = line.substr(offset, line.find("\"", offset)-offset);
      foundName = true;
    }
  }
  //find line matching /class ([a-zA-Z_]*) : public [a-zA-Z_]* {/
  //  \1 is name
  //  }

  //find line matching /virtual std::string Name\(\) const {return "(.*)";}/
  //  \2 is code
  if(e.code.empty() || e.name.empty()) {
    return false;
  }

  return true;
}

int main(int argc, char** argv) {
  //should be 1 argument: output file name
  if(argc != 2) {
    return 1;
  }
  std::ofstream dump(argv[1]);
  if(!dump.is_open()) {
    std::cerr << "Error: Output file could not be opened for reading\n";
    return 1;
  }

  std::string output;

  std::string prefix = R""""(  bool ObjContainer::storeObj(Object ob, std::string t, std::string& status) {
)"""";
  const std::string suffix = R""""(else {
    status = "Error: Invalid object type";
    return false;
  }
  //copy all externally-set attributes of the passed object to list.back();
  list.back()->setLinkID(ob.getLinkID());
  list.back()->setTextureID(ob.getTextureID());
  list.back()->setParentID(ob.getParentID());
  list.back()->setPos(ob.getPos());
  list.back()->setSize(ob.getSize());
  list.back()->setSolid(ob.getSolid());
  list.back()->setText(ob.getText());
  list.back()->setStatus(Object::Normal);
  list.back()->setArgs(ob.getArgs());
  list.back()->setSwitches(ob.getSwitches());

  if(!list.back()->verify()) {
    status = "Error: Invalid object configuration in object of type '";
    status += list.back()->Name() + "'. Object wasn't added\n";
    removeObj(list.back());   
    return false;
    //object isn't added
  }
  counter++;
  return true;
})"""";

  std::string includes = "#include \"level/obj-container.h\"\n";
  const fs::path obj_dir{"../include/mutable"};

  //the first entry is an if, the others are if-else. This signals which one to use
  bool firstEntry = true;

  std::ofstream secondary("obj-names.txt");

  std::vector<std::string> objNames;

  for(auto const& entry : std::filesystem::directory_iterator{obj_dir}) {
    //player is a special case of object, and isn't built by the factory
    if(entry.path().filename() == "player.h" || entry.path().filename() =="mutable.h"
        || entry.path().extension() != ".h") { //filters out non-headers
      continue;
    }
    includes += "#include \"mutable/"+entry.path().filename().string()+"\"\n";

    Entry en;
    if(!parseFile(entry.path(), en)) {
      std::cerr << "Error: malformed class definition\n";
      return 1;
    }
    if(!firstEntry) {
      output += "else ";
    }
    else {
      firstEntry = false;
    }
    //this was broken up onto multiple lines for clarity
    output += "if(t == \"" + en.code + "\") {";
    output += " list.push_back(new " + en.name + "(counter));\n  }";

    //write a list of class types to obj-names.txt
    secondary << en.code << '\n';
    objNames.push_back(en.code);
  }
  output += suffix;

  dump << includes << prefix << output;

  //add a simple function: is_valid_obj_name(const std::string& s);
  dump << "\n\nbool is_valid_obj_name(const std::string& s) {\n\treturn(";
  for(unsigned i=0;i<objNames.size();i++) {
    dump << "s == \"" << objNames[i] << "\"";
    if(i+1 == objNames.size()) {
      dump << ");\n}";
    } else {
      dump << " || ";
    }
  }

  return 0;
}


