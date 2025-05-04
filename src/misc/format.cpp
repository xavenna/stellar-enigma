#include "misc/format.h"


bool parseSEObjFormat(const std::string& line, std::list<ObjAttr>& fields) {
  //parse line. Break into bracketed sections, and interpret each as a vect of strings
  //check if each group has a valid identifier. If so, check if argument list is valid
  //If so, write valus to variables.
  //Otherwise, fail
  std::vector<std::string> ids;
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
    fields.push_back(ObjAttr());
    x += ','; //this ensures the last argument isn't omitted
    for(auto y : x) {  //for each char of current string...
      if(y == '=') {
        if(inField) {
          std::cerr << "Error: repeat equal in braces\n";
          return false;
        }
        fields.back().id = accum;
        accum.clear();
        inField = true;
      }
      else if(y == '"') {
        if(inString) {
          inString = false;
          //string is over, parse it
          fields.back().textArg = accum;
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
          fields.back().textArg = accum.substr(0,accum.size()-1);

        }
        else if(!isNum(accum)) {
          std::cerr << "Error: non-numeric arg '"<<accum<<"' in field\n";
          return false;
        }
        else {
          fields.back().args.push_back(std::stof(accum));
        }
        accum.clear();
      }
      else {
        accum += y;
      }
    }
  }
  return true;
}


bool parseExplorFormat(const std::string& line, std::list<Arg>& fields) {
  //treat line as a backtick-delimited sequence of strings. Get each one, check if it's an int. If so, populate field with  

  bool inField;
  std::string accum;
  std::vector<std::string> args;

  parse(line, args, "`"); //each element of args now contains a field.
  //process each string to extract information, check for errors
  for(auto x : args) {
    fields.push_back(Arg());
    //check if x is an int:
    if(isNum(x)) {
      fields.back().num = std::stoi(x);
    }
    else {
      fields.back().text = x;
    }
  }
  return true;

}
