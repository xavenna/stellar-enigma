#ifndef MUTABLE_H
#define MUTABLE_H

#include <SFML/Graphics.hpp>

//! Base class for things separate from the map, such as enemies and objects
/*!
 *  Extends sf::Sprite in order to be drawable.
 *  Contains other attributes common to all mutable extensions with getters 
 *  and setters
 */
class Mutable : public sf::Sprite {
protected:
  sf::Vector2f pos; //!< Current pos of the mutable
  sf::Vector2f lastPos; //!< Where the mutable was before its most recent move
  sf::Vector2f savedPos; //!< Used for properly resolving lastPos
  sf::Vector2i screen; //!< Which screen the mutable is on
  sf::Vector2f size; //!< Size of the mutable in pixels
  bool solid; //!< Whether other mutables can pass through the mutable. Deprecated
  //!< The object's mass. A higher mass means it's harder to push.
  float mass=0;
  //!< Self-initiated movement distance for the next frame
  sf::Vector2f selfPush;

public:

  //! Deprecated
  int behaviorType;
	

  //! get the position of the mutable
  sf::Vector2f getPos() const;
  //! get the center of the mutable
  sf::Vector2f getCenter() const;
  //! get the last position of the mutable as a sf::Vector2i
  sf::Vector2f getLastPos() const;
  //! get the size of the mutable as a sf::Vector2i
  sf::Vector2f getSize() const;
  //! get the screen-pos of the mutable as a sf::Vector2i
  sf::Vector2i getScreen() const;
  //! Returns position delta (pos - lastpos)
  sf::Vector2f getDelta() const;
  //! Returns mass
  float getMass() const;
  //! Returns self push
  sf::Vector2f getSelfPush() const;

  //! Save current position
  void savePos();
  //! move saved position to lastPos (to make deltas work)
  void updateDelta();

  //! set the position of the mutable as a sf::Vector2i
  void setPos(sf::Vector2f);
  //! set the last position of the mutable as a sf::Vector2i
  void setLastPos(sf::Vector2f);
  //! set the size of the mutable as a sf::Vector2i
  void setSize(sf::Vector2f);
  //! set the screen-pos of the mutable as a sf::Vector2i
  void setScreen(sf::Vector2i);
  //! sets mass
  void setMass(float);
  //! sets self push
  void setSelfPush(sf::Vector2f);

  //! set both x and y position of the mutable
  void setPos(float, float);
  //! set the x position of the mutable
  void setXPos(float);
  //! set the y position of the mutable
  void setYPos(float);
  //! set the width of the mutable
  void setWidth(float);
  //! set the height of the mutable
  void setHeight(float);
  //! set the x screen of the mutable
  void setXScreen(int);
  //! set the y screen of the mutable
  void setYScreen(int);

  //! sets mass
  void getMass(float);
  //! sets selfpush. Maybe unnecessary
  void getSelfPush(sf::Vector2f);


  //! set the solidity of the mutable
  void setSolid(bool);
  //! Get the solidity of the mutable
  bool getSolid() const;
};

#endif
