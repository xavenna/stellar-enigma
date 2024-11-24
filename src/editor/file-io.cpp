#include "editor/file-io.h"
#include "editor/editor.h"

namespace ed {
  //return code of 1 means couldn't open file
  //return code of 2 means invalid object
  int loadMutableList(std::vector<ObjectBase>& objs, const std::string& filename) {
    objs.clear();
    std::ifstream get(filename);
    std::string line;
    std::string accum;
    ObjectBase o;  //these are defined now so they don't have to be every loop
    std::string type;  //Type of requested object
    if(!get.is_open()) {
      return 1;
    }
    while(get.peek() != EOF) {
      std::getline(get, line);
      if(line.size() == 0 || line[0] == '#') {
        continue; //skip line
      }
      //parse `line' as a mutable

      //switch to using: parseSEObjFormat and generateObjFromObjAttrList here
      if(!str2obj(line, o, type)) {
        return 2;
      }
      objs.push_back(o);


    }
    return 0;

  }

  //{oa=0,0,0,0,0,0,0,0}{sw=-1,-1,-1,-1,-1,-1,-1,-1}{p=32,60}{s=32,32}{li=-1}{pi=-1}{ti=0}{c="solid"}
  bool writeListToFile(std::vector<ObjectBase>& objs, const std::string& filename, std::string& status) {
    std::ofstream write(filename);
    status = filename;
    if(!write.is_open()) {
      status = "File could not be opened for writing.";
      return false;
    }
    std::string output;
    for(auto x : objs) {
      std::string line;
      line = "{oa=";
      for(int i=0;i<8;i++) {
        line += std::to_string(x.args[i]);
        if(i==7) {
          break;
        }
        line += ",";
      }
      line += "}{sw=";
      for(int i=0;i<8;i++) {
        line += std::to_string(x.switches[i]);
        if(i==7) {
          break;
        }
        line += ",";
      }
      line += "}{p=" + std::to_string(x.pos.x) + "," + std::to_string(x.pos.y) + "}";
      line += "{s=" + std::to_string(x.size.x) + "," + std::to_string(x.size.y) + "}";
      line += "{li=" + std::to_string(x.link_id) + "}";
      line += "{pi=" + std::to_string(x.parent_id) + "}";
      line += "{ti=" + std::to_string(x.texture_id) + "}";
      line += "{c=\"" + x.type + "\"}";
      if(!x.text.empty()) {
        line += "{te=\"" + x.text + "\"}";
      }
      write << line << '\n';
    }
    write.close();
    return true;
  }

  bool str2obj(const std::string& line, ObjectBase& obj, std::string& objType) {
    //parse line. Break into bracketed sections, and interpret each as a vect of strings
    //check if each group has a valid identifier. If so, check if argument list is valid
    //If so, write valus to variables.
    //Otherwise, fail
    std::vector<std::string> ids;
    std::vector<ObjAttr> attribs;
    bool inField{false};
    bool inString{false};
    std::string accum;
    for(auto x : line) {  //parse line into bracketed definitions. IDs and Attribs contain the definitions
      if(x == '{') {
        if(inField) {
          std::clog << "Error: malformed object list : nested braces\n";
          return false;
        }
        inField = true;
      }
      else if(x == '}') {
        if(!inField) {
          std::clog << "Error: malformed object list : close brace outside of braces\n";
          return false;
        }
        inField = false;
        ids.push_back(accum);
        accum.clear();
      }
      else {
        if(!inField) {
          //ignore whitespace
          if(!isEmpty(x)) {
            std::clog << "Error: malformed object list : non-whitespace '"<<x<<"' outside of braces\n";
            return false;
          }
        }
        else {
          accum += x;
        }
      }
    }
    //process each string to extract information, check for errors
    for(auto x : ids) { //transform the bracketed string into an ObjAttr struct
                        //for each string in ids
      accum.clear();
      inField = false;
      inString = false;
      std::string name;
      attribs.push_back(ObjAttr());
      x += ','; //this ensures the last argument isn't omitted
      for(auto y : x) {  //for each char of current string...
        if(y == '=') {
          if(inField) {
            std::cerr << "Error: repeat equal in braces\n";
            return false;
          }
          attribs.back().id = accum;
          accum.clear();
          inField = true;
        }
        else if(y == '"') {
          if(inString) {
            inString = false;
            //string is over, parse it
            attribs.back().textArg = accum;
            break;
          }
          else {
            inString = true;
          }
        }
        else if(y == ',') {  // check if it's wrapped in a string
          if(inString) {
            //commas in strings are ignored
            accum += y;
            continue;
          }
          if(accum[0] == '"') { //argument is a string
            if(accum.back() != '"') {
              std::cerr << "Error: unterminated string constant in object list\n";
              std::cerr << "'"<<accum<<"'\n";
              return false;
            }
            accum=accum.substr(1);
            attribs.back().textArg = accum.substr(0,accum.size()-1);

          }
          else if(!isNum(accum)) {
            std::cerr << "Error: non-numeric arg '"<<accum<<"' in field\n";
            return false;
          }
          else {
            attribs.back().args.push_back(std::stof(accum));
          }
          accum.clear();
        }
        else {
          accum += y;
        }
      }
    }
    //now that data is extracted from bracketed parts, check for validity
    //if it's valid, assign the appropriate attribute
    //TODO: add a way to avoid duplicates
    std::array<int, 8> args;
    std::array<int, 8> switches;
    sf::Vector2f pos{0,0};
    sf::Vector2f size{0,0};
    std::string text;
    std::string objClass;
    int parent=-1;
    int link=-1;
    int tex=-1;
    int argNum;
    for(auto x : attribs) {
      argNum = x.args.size();
      if(x.id == "oa") {
        //assign obj args
        if(argNum > 8) {
          std::clog << "Error: oa field accepts at most 8 arguments, "<<argNum<<" provided\n";
          return false;
        }
        unsigned i=0;
        for(auto y : x.args) {
          args[i++] = y;
        }
      }
      else if(x.id == "sw") {
        //assign switches
        if(argNum > 8) {
          std::clog << "Error: sw field accepts at most 8 arguments, "<<argNum<<" provided\n";
          return false;
        }
        unsigned i=0;
        for(auto y : x.args) {
          switches[i++] = y;
        }
      }
      else if(x.id == "p") {
        //assign position
        if(argNum != 2) {
          std::clog << "Error: p field requires 2 arguments, "<<argNum<<" provided\n";
          return false;
        }
        pos.x = x.args[0];
        pos.y = x.args[1];
      }
      else if(x.id == "s") {
        //assign size
        //are there two int args?
        if(argNum != 2) {
          std::clog << "Error: s field requires 2 argument, "<<argNum<<" provided\n";
          return false;
        }
        size.x = x.args[0];
        size.y = x.args[1];

      }
      else if(x.id == "pi") {
        if(argNum != 1) {
          std::clog << "Error: pi field requires 1 argument, "<<argNum<<" provided\n";
          return false;
        }
        //select object type
        parent = x.args[0];
      }
      else if(x.id == "li") {
        if(argNum != 1) {
          std::clog << "Error: li field requires 1 argument, "<<argNum<<" provided\n";
          return false;
        }
        //select object type
        link = x.args[0];
      }
      else if(x.id == "ti") {
        if(argNum != 1) {
          std::clog << "Error: ti field requires 1 argument, "<<argNum<<" provided\n";
          return false;
        }
        //select object type
        tex = x.args[0];
      }
      else if(x.id == "c") {
        if(x.textArg.size() == 0) {
          //null id, fail
          std::clog << "Error: c field requires a non-empty string\n";
          return false;
        }
        objClass = x.textArg;
      }
      else if(x.id == "te") {
        if(x.textArg.size() == 0) {
          //null id, fail
          std::clog << "Error: te field requires a non-empty string\n";
          return false;
        }
        text = x.textArg;
      }
      else {
        //invalid, or text
        std::clog << "Invalid Identifier '"<<x.id<<"' in object list\n";
        return false;
      }
    }
    //now that values have been extracted, assign values to object
    obj.link_id = link;
    obj.parent_id = parent;
    obj.texture_id = tex;
    obj.args = args;
    obj.switches = switches;
    obj.pos = pos;
    obj.size = size;
    obj.text = text;
    obj.type = objClass;
    return true;
  }
}


