#ifndef MESSAGE_H
#define MESSAGE_H

#include <deque>
#include <SFML/Graphics.hpp>

//! A class that handles the message component of the engine
/*!
 *  Messages can be added to the internal queue, and will be displayed when
 *  the message slot is open. Strings are displayed incrementally, rather
 *  than all at once. After a string is fully displayed, there is a cooldown
 *  before the next string is displayed.
 */
class Message : public sf::Text {
private:
  std::string message;  //!< This holds the string currently being displayed
  std::string formMessage;  //!< The part of message that has been displayed
  size_t pointer;  //!< This holds the position of the last-printed character
  const size_t width;  //!< The width of the screen, in characters. Used for wrapping text
  bool printed;  //!< Is the current string finished printing?
  const size_t maxCool;  //!< How many frames of cooldown before next string is drawn
  const size_t maxElem;  //!< How many messages can be in the queue
  size_t cooldown; //!< How many frames of cooldown remain
  std::deque<std::string> printQueue; //!< Queue of strings to display
  sf::Font courier; //!< Font the message uses
public:
  //! Get whether the current string is done printing 
  bool getPrinted() const;
  //! Adds a string to the printing queue
  void addMessage(const std::string&);
  //! Updates the displayed message
  /*!
   *  This checks if the current string is done displaying and cooled down. If
   *  so, it begins displaying the next string in the queue (if there is one).
   *  Otherwise, it takes the next character from message and adds it to 
   *  formMessage.
   */
  void handleMessages();
  //! Wrap the message stored in formMessage, and write it to text.getString()
  void wrapMessage();

  //These next functions are only for debugging, remove later
  //! Returns the currently printing message
  std::string getMessage();
  //! Prints the size of the message queue
  int getSize();
  
  //! constructor: sets width to arg1, maximum cooldown to arg2, and maxElem to arg3
  Message(unsigned, unsigned, unsigned, unsigned);
};

#endif
