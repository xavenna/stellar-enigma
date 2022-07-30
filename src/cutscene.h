#ifndef CUTSCENE_H
#define CUTSCENE_H

#include <vector>
#include <fstream>
#include "event.h"

//! A class that holds a cutscene as a series of events
class Cutscene {
private:
  std::vector<Event> eventList;
  std::string name;
public:
  //! Get the length of the list
  int getListLen();
  //! Get an event from the list
  Event getEvent(unsigned pos);
  //! Load a cutscene from a file (not implemented yet)
  bool loadFromFile(const std::string& filename);
};


#endif
