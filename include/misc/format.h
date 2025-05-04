#ifndef FORMAT_H
#define FORMAT_H

#include <list>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <map>
#include "misc/util.h"
// A collection of functions for parsing stellar's file formats

//! An internal utility struct for parsing Stellar Object Format lists
struct ObjAttr {
  std::string id; //!< field's ID
  std::vector<float> args; //!< Numeric arguments, if present
  std::string textArg; //!< Text argument, if present
};

//! A basic struct for parsing ObjAttr lists
struct Arg {
  std::string text; //!< Text argument
  int num; //!< Int argument
};

//! converts a line of SEF into a list of ObjAttr's
bool parseSEObjFormat(const std::string& line, std::list<ObjAttr>&);

//! converts a line of explor format into a list of ObjAttr's
bool parseExplorFormat(const std::string& line, std::list<Arg>& fields);


//TODO: Make a generic version of generateObjFromObjAttrList

//! Extract all field data from an ObjAttr List, place in the specified map
bool extractDataFromObjAttrList(const std::list<ObjAttr>& attribs, std::map<std::string, Arg>);


#endif
