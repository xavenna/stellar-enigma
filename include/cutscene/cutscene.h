#ifndef CUTSCENE_H
#define CUTSCENE_H

#include <vector>
#include <fstream>
#include "cutscene/event.h"

//! A class that holds a cutscene as a series of events
class Cutscene {
private:
  std::vector<Event> eventList;
  std::string name;
public:
  //! Get the number of cutscenes stored
  int getListLen();
  //! Gets specified event from eventList
  Event getEvent(unsigned pos);
  //! Loads a cutscene from a file.
  /*!
   *  Returns false if an error is detected in the file.
   *  If the operation fails, eventList is emptied (to avoid unexpected
   *  behavior from partially loaded cutscenes)
   */
  bool loadFromFile(const std::string& filename);
};


#endif
