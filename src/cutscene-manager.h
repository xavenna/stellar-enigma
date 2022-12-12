#ifndef CUTSCENE_MANAGER_H
#define CUTSCENE_MANAGER_H
#include "cutscene.h"
#include <map>
#include <stdexcept>
#include <fstream>

//! A class that loads and stores cutscenes for use by a CutscenePlayer
class CutsceneManager {
private:
  std::map<std::string, Cutscene> cutlist;
public:
  //! Loads all cutscene files listed in specified file.
  /*!
   *  Returns false if file not found.
   */
  bool loadCutscenes(const std::string& filename);
  //! Returns cutscene with specified name
  /*!
   *  If specified cutscene is not found, throws std::out_of_range.
   */
  Cutscene getCutscene(const std::string& name);
  //! Returns whether or not specified cutscene exists
  bool cutsceneExists(const std::string& name) const;
};


#endif
