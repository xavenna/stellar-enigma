#include "mapdata/menu.h"




MenuTemplate::MenuTemplate(const std::string& str) {
  //eventually, make this use json instead
  //init template from str

  std::vector<std::string> ids;
  std::vector<ObjAttr> attribs;
  bool inField{false};
  std::string accum;

  //the first section of this should be wrapped into a function, as it's used in several places
  for(auto x : str) {  //parse line into bracketed definitions. IDs and Attribs contain the definitions
    if(x == '{') {
      if(inField) {
        std::clog << "Error: malformed menu list : nested braces\n";
        return;
      }
      inField = true;
    }
    else if(x == '}') {
      if(!inField) {
        std::clog << "Error: malformed menu list : close brace outside of braces\n";
        return;
      }
      inField = false;
      ids.push_back(accum);
      accum.clear();
    }
    else {
      if(!inField) {
        //ignore whitespace
        if(!isEmpty(x)) {
          std::clog << "Error: malformed menu list : non-whitespace '"<<x<<"' outside of braces\n";
          break;
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
    attribs.push_back(ObjAttr());
    x += ','; //this ensures the last argument isn't omitted
    for(auto y : x) {  //for each char of current string...
      if(y == '=') {
        if(inField) {
          std::cerr << "Error: repeat colon in braces\n";
          break;
        }
        attribs.back().id = accum;
        accum.clear();
        inField = true;
      }
      else if(y == ',') {  // check if it's wrapped in a string
        if(!inField) {
          //text field
          //the entire field, except the final char, is placed in id
          attribs.back().id = x.substr(0, x.size()-1);
          break;
        }
        if(accum[0] == '"') { //argument is a string
          if(accum.back() != '"') {
            std::cerr << "Error: unterminated string constant in object list\n";
            break;
          }
          accum=accum.substr(1);
          attribs.back().textArg = accum.substr(0,accum.size()-1);

        }
        else if(!isNum(accum)) {
          std::cerr << "Error: non-numeric arg '"<<accum<<"' in field\n";
          break;
        }
        else {
          attribs.back().args.push_back(std::stoi(accum));
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

  int argNum;
  for(auto x : attribs) {
    argNum = x.args.size();
    if(x.id == "ex") {
      //assign exit codes
      if(argNum > 7) {
        std::clog << "Error: oa field accepts at most 7 arguments, "<<argNum<<" provided\n";
        break;
      }
      unsigned i=0;
      for(auto y : x.args) {
        exitBehavior[i++] = y;
      }
    }
    else if(x.id.size() > 1 && x.id[0] == 'e') {
      //check if x[1] is 0-6. If so, it's an exit code string arg
      if(x.id[1] >= '0' && x.id[1] < '7') {
        //it's an arg:
        exitStrArgs[static_cast<unsigned>(x.id[1]-'0')] = x.textArg; //this feels sketchy, but I think it's safe
      }
    }
    else if(x.id.size() > 2 && x.id[0] == 't') {
      //transform data
      if(!isNum(x.id.substr(1,x.id.size()-2))) {
        std::cerr << "Error: Invalid transform data id number\n";
        return;
      }
      int numS = std::stoi(x.id.substr(1,x.id.size()-2));
      if(numS < 0) {
        std::cerr << "Error: Negative transform number not permitted\n";
        return;
      }
      unsigned num = static_cast<unsigned>(numS);
      if(tlist.size() < num+1) {
        tlist.resize(num+1);
      }
      if(x.id.back() == 't') {
        //transform text
        tlist[num].text = x.textArg;
        fixEscape(tlist[num].text);
      }
      else if(x.id.back() == 'a') {
        if(argNum > 8) {
          std::clog << "Error: "<<x.id<<" field accepts at most 8 arguments, "<<argNum<<" provided\n";
          break;
        }
        unsigned i=0;
        for(auto y : x.args) {
          tlist[num].args[i++] = y;
        }

      }
      else if(x.id.back() == 'i') {
        if(argNum != 1) {
          std::clog << "Error: "<<x.id<<" field requires 1 argument, "<<argNum<<" provided\n";
          break;
        }
        //select object type
        tlist[num].type = static_cast<Transform::Type>(x.args[0]);

      }
      else {
        std::clog << "Error: invalid transform data field name \n";
        break;
      }

    }
    else if(x.id == "n") {
      if(x.textArg.size() == 0) {
        //null id, fail
        std::clog << "Error: n field requires a non-empty string\n";
        break;
      }
      name = x.textArg;
    }
    else if(x.id == "img") {
      if(x.textArg.size() == 0) {
        //null id, fail
        std::clog << "Error: img field requires a non-empty string\n";
        break;
      }
      baseImage = x.textArg;
    }
    else {
      std::clog << "Invalid Identifier '"<<x.id<<"' in object list\n";
      break;
    }
  }
  /*
  std::cerr << "Constructed Template as such:\nimg="<<baseImage<<";n="<<name<<";tlist:";
  std::cerr << tlist.size() << "tfs\n";
  if(tlist.size() > 0) {
    std::cerr << "tid="<<tlist[0].type<<";args=";
    for(auto x : tlist[0].args) {std::cerr << x << ',';}
    std::cerr << '\n';
    for(auto x : exitStrArgs) {std::cerr << x << ',';}
    std::cerr << '\n';
  }
  */
}

bool Menu::initialize() {
  // load menu templates from the file
  std::string complevel = "assets/menu/menus.txt";  //menu templates list
  std::ifstream get(complevel);
  std::string line;
  std::string accum;
  if(!get.is_open()) {
    return false;
  }
  while(get.peek() != EOF) {
    std::getline(get, line);
    if(line.size() == 0 || line[0] == '#') {
      continue; //skip line
    }

    // create a Menu Template
    MenuTemplate t(line);
    menus.emplace(t.name, t);
    
  }
  return true;
}


void Menu::assignTexture(TextureCache& cache) {
  CacheNodeAttributes cna;
  cna.srcImg = cache.reverseHash(menus.find(currentTemplate)->second.baseImage);
  cna.tList = menus.find(currentTemplate)->second.tlist;
  setTexture(cache.getTexture(cna));
}


bool Menu::loadTemplate(const std::string& name) {
  //check if specified template exists
  if(menus.find(name) == menus.end()) {
    std::cout << "Error: Menu '"<<name<<"' not found.\n";
    return false;
  }
  currentTemplate = name;
  return true;
}

int Menu::getCode(unsigned i) const {
  if(i > 6) {
    //error: invalid code
    std::clog << "Error: invalid menu exit code index request\n";
    return 0; //if an invalid code is requested, do nothing
  }

  return menus.find(currentTemplate)->second.exitBehavior[i];
}

std::string Menu::getTextArg(unsigned i) const {
  if(i > 6) {
    //error: invalid code
    std::clog << "Error: invalid menu exit code index request\n";
    return 0; //if an invalid code is requested, do nothing
  }
  return menus.find(currentTemplate)->second.exitStrArgs[i];
}

void fixEscape(std::string& s) {
  std::string out;
  out.reserve(s.size());
  for(unsigned i=0;i<s.size();i++) {
    auto x = s[i];
    if(x == '\\') {
      if(s[i+1] == 'n') {
        out += '\n';
        i++;
      }
      else if(s[i+1] == '\\') {
        out += '\\';
        i++;
      }
      else { //if a backslash isn't part of an escape, treat it normally
        out += '\\';
      }
    }
    else {
      out += x;
    }

  }
  s = out;


}


