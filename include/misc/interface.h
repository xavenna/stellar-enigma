#ifndef INTERFACE_H
#define INTERFACE_H
#include <vector>
#include <string>
#include <array>

class Object;


//! A utility struct representing an inter-object message
struct msg {
  unsigned link; //!< Link ID of object to notify.
  char type; //!< What type of message is being sent? Defined on a per-obj basis
  std::array<int, 6> data; //!< Data of message use
};

//! A utility class for object behavior function return values
/*!
 *  This serves as an API allowing the virtual functions to cause effects, such as playing
 *  cutscenes, displaying messages, etc. Originally, a std::tuple was used, but this
 *  method is more flexible
 */
class Interface {
public:
  std::vector<std::string> message;   //!< Messages to display
  std::vector<std::string> cutscene;  //!< Cutscenes to play
  std::vector<std::string> sounds; //!< Sounds to play
  std::vector<std::pair<Object, std::string>> objs; //!< Objects to create, along with type
  std::vector<msg> notifications; //!< Notifications to send.
  std::string menu; //!< Menu to open

  //! Registers message to be displayed. Overwrites previous message, if any.
  void addMessage(const std::string&);
  //! Chooses a cutscene to play. Overwrites previously stored cutscene, if any.
  void playCutscene(const std::string&);
  //! Requests selected sound to be played. Does not overwrite previous sounds
  void playSound(const std::string&);
  //! Requests to spawn an object. Spawned object will not have a link id, but will be the child of spawning obj.
  void spawnObject(Object, const std::string& type);
  //! Creates a notification to send to object with specified Link_ID
  void notify(msg);
  //! Clears the object list
  void clearObjs();
  //! Opens specified menu
  void openMenu(const std::string&);

  //! Constructs an empty Interface. Use the API functions to request actions. Eventually, the vars may be private
  Interface();

};


#endif
