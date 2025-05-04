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
      line += "{sc=" + std::to_string(x.scale.x) + "," + std::to_string(x.scale.y) + "}";
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
    sf::Vector2f scale{1,1};
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
      else if(x.id == "sc") {
        //assign size
        //are there two int args?
        if(argNum != 2) {
          std::clog << "Error: sc field requires 2 argument, "<<argNum<<" provided\n";
          return false;
        }
        scale.x = x.args[0];
        scale.y = x.args[1];

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
    obj.scale = scale;
    obj.text = text;
    obj.type = objClass;
    return true;
  }

  bool parse_db(const std::string& fn, Database& d) {

    std::ifstream read(fn);
    std::string data;
    std::string err;
    if(!read.is_open()) {
      std::cerr << "Error: could not open database\n";
      return false;
    }
    getEntireFile(read, data);
    read.close();
    //read contents of save file
    json11::Json save;
    save = save.parse(data, err);
    if(!err.empty()) {
      std::cerr << "JSON parse error. Details: "<<err<<'\n';
      return false;
    }
    //parse json -- create a table in memory
    json11::Json::object k = save.object_items();
    d.version = k["version"].string_value();

    json11::Json::array objs = k["objs"].array_items();

    //turn each element of objs to an ObjInfo struct, then use those to populate
    for(auto& x : objs) {
      ObjInfo o;
      std::string s;
      if(!x["internal_name"].is_string()) {
        std::cerr << "Couldn't read entry\n";
      }

      //populate o with fields of x
      if(!generateObjInfo(x, o, s)) {
        std::cerr << "Database Error: " << s << '\n';
        return false;
      }


      d.append(o);
    }
    return true;
  }
  bool generateObjInfo(const json11::Json& obj, ObjInfo& o, std::string& err) {
    //the obj should be flat, so this is simple

    //add checking. If fields are blank, leave them blank...
    
    o.parent = obj["parent"].string_value();
    o.type = obj["type"].string_value();
    o.size = sf::Vector2f(obj["xsize"].number_value(), obj["ysize"].number_value());
    o.shortName = obj["internal_name"].string_value();
    o.longName = obj["verbose_name"].string_value();
    o.description = obj["description"].string_value();
    o.textArg = obj["text_arg"].string_value();

    //add args, switches
    o.switches[0] = obj["SW_Appear"].string_value();
    o.switches[1] = obj["SW_Disappear"].string_value();
    o.switches[2] = obj["SW_A"].string_value();
    o.switches[3] = obj["SW_B"].string_value();
    o.switches[4] = obj["SW_C"].string_value();
    o.switches[5] = obj["SW_D"].string_value();
    o.switches[6] = obj["SW_Stat"].string_value();
    o.switches[7] = obj["SW_Remove"].string_value();

    o.args[0] = obj["arg_0"].string_value();
    o.args[1] = obj["arg_1"].string_value();
    o.args[2] = obj["arg_2"].string_value();
    o.args[3] = obj["arg_3"].string_value();
    o.args[4] = obj["arg_4"].string_value();
    o.args[5] = obj["arg_5"].string_value();
    o.args[6] = obj["arg_6"].string_value();
    o.args[7] = obj["arg_7"].string_value();

    //add misc bool flags here

    return true;
  }

  /*
    "arg0":"behavior mode: 0-follows state of SW_A; 1-permanently opens once triggered; 2-toggles state on rising edge.",
    "SW_A":"Activates door",
    "priority":0,
    "verbose_name":"Door",
    "internal_name":"door",
    "handles_use":false,
    "handles_interact":false,
    "texture_id":false,
    "invisible":true,
    */



  void Database::append(const ObjInfo& o) {
    data.emplace(o.shortName,o);
  }
  ObjInfo Database::operator[](std::string key) const {
    if(data.find(key) == data.end()) {
      std::string s = "Error: value '" + key + "' not found in database";
      throw std::invalid_argument(s);
    }
    return data.at(key);
  }
  bool Database::contains(const std::string& key) const {
    return data.find(key) != data.end();
  }
  /*
    sf::Vector2f size;
    unsigned priority;
    bool use;
    bool interact;
    bool texture;
    bool invisible;
    */
  std::string Database::get_full_entry(const std::string& key) {
    if(!contains(key)) {
      return std::string("Could not find entry...");
    }
    ObjInfo o = data[key];
    std::array<std::string, 8> swnames = {
      "Appear",
      "Disappear",
      "A",
      "B",
      "C",
      "D",
      "Stat",
      "Remove"
    };
    std::string arg;
    for(unsigned i=0;i<8;i++) {
      arg += "arg_"+std::to_string(i)+": "+
        o.args[i] + '\n';
    }

    std::string sws;
    for(unsigned i=0;i<8;i++) {
      sws += "SW_"+swnames[i]+": "+
        o.switches[i] + '\n';
    }

    std::string result =
      "Internal name: " + o.shortName +
      "\nVerbose name: " + o.longName +
      "\nParent object: " + o.parent +
      "\nObject type: " + o.type +
      "\nArguments:\n" + arg +
      "\nSwitches:\n" + sws +
      "\nDescription: " + o.description;

    return result;
  }
  //gets effective size of ObjectBase
  sf::Vector2f size(const ObjectBase& x, const Database& db) {
    sf::Vector2f osize = db[x.type].size;
    return cMult(osize, x.scale);
  }
}


