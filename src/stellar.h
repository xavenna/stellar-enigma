#ifndef STELLAR_H
#define STELLAR_H
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <deque>
#include <SFML/Graphics.hpp>
#include "util.h"

//this is the primary header file for the stellar enigma engine
//It contains definitions of integral classes, as well as other useful functions

enum Direction {Up=0, Right, Down, Left};

//!  A class responsible for assigning textures
/*!  A texturemap is responsible for assigning textures to sf::Sprites.
 *   It contains a vector of sf::Textures, which can be assigned using
 *   encapsulated methods. It loads the textures from images, using a
 *   texturemap file (*.tm). This is a plain-text file that is composed
 *   of a list of filenames, which are loaded in order. When setting a 
 *   texture for a Node, the idTh texture is chosen.
 */
class TextureMap {
  //contains texture mappings for use when assigning textures to sprites
public:
  std::vector<sf::Texture> mapping; //!< the vector that stores Textures
  //This should probably be encapsulated...

  //! returns a reference to the texture with number specified by argument
  sf::Texture& getTexture(const int&);

  //! sets a texture in mapping to supplied texture
  /*!
   *  I don't believe this is used anywhere, as textures need to be references.
   *  So don't use this for now
   */
  void assign(int, sf::Texture);
  //! Sets up the texturemap using the supplied filename
  /*!
   *  The file is assumed to be located in ./assets/texturemap/
   */
  bool initialize(const std::string&);
  //! A function which returns the number of textures stored
  int size() const;

  //! Constructor that initializes the textures with supplied argument
  TextureMap(const std::string&);
  //! The default constructor, takes no arguments
  TextureMap();
};

//! A class that handles the message component of the engine
/*!
 *  Messages can be added to the internal queue, and will be displayed when
 *  the message slot is open. Strings are displayed incrementally, rather
 *  than all at once. After a string is fully displayed, there is a cooldown
 *  before the next string is displayed.
 */
class Message {
private:
  std::string message;  //!< This holds the string currently being displayed
  size_t pointer;  //!< This holds the position of the last-printed character
  const size_t width;  //!< The width of the screen, in characters. Used for wrapping text
  bool printed;  //!< Is the current string finished printing?
  const size_t maxCool;  //!< How many frames of cooldown before next string is drawn
  size_t cooldown; //!< How many frames of cooldown remain
  std::deque<std::string> printQueue; //!< Queue of strings to display
public:
  //! Get whether the current string is done printing 
  bool getPrinted() const;
  //! Adds a string to the printing queue
  void addMessage(const std::string&);
  //! Updates the displayed message
  /*!
   *  This checks if the current string is done displaying and cooled down. If
   *  so, it begins displaying the next string in the queue (if there is one)
   */
  void handleMessages();
  //! constructor: sets width to arg1, and maximum cooldown to arg2
  Message(size_t, size_t);
};

//! Base class for things separate from the map, such as enemies and objects
/*!
 *  Contains a sf::Sprite for drawing, as well as base attributes with getters 
 *  and setters
 */
class Mutable {  //this class is the base class for things that can move around
protected:
  int xpos; //!< X position of the mutable
  int ypos; //!< Y position of the mutable
  int width; //!< width of the mutable (I forget what the units are)
  int height; //!< height of the mutable
  bool solid; //!< whether the mutable can be passed through (I think)
public:
  //! Get the x position of the mutable
  int getXPos() const;
  //! Get the y position of the mutable
  int getYPos() const;
  //! Get the width of the mutable
  int getWidth() const;
  //! Get the height of the mutable
  int getHeight() const;
  //! Get the solidity of the mutable
  bool getSolid() const;
  //! set the x position of the mutable
  void setXPos(const int&);
  //! set the y position of the mutable
  void setYPos(const int&);
  //! set both x and y position of the mutable
  void setPos(const int&, const int&);
  //! set the width of the mutable
  void setWidth(const int&);
  //! set the height of the mutable
  void setHeight(const int&);
  //! set the solidity of the mutable
  void setSolid(const bool&);
  sf::Sprite area; //!< the sprite that is drawn to the map
};

//! A class for objects the player can interact with, extends Mutable
/*!
 *  Extends mutable with additional attributes specific to objects
 */
class Object : public Mutable {
protected:
  int id; //!< the internal id of the object
  bool collectable; //!< Whether the object can be collected by the player
public:
  //! get the id of the object 
  int getId() const;
  //! get the collectability of the object
  bool getCollectable() const;
  //! set the id of the object
  void setId(const int&);
  //! set the collectability of the object
  void setCollectable(const bool&);
};

//! A class for entities capable of moving around. Extends mutable
/*! 
 * Extends Mutable with additional attributes specific to entities. 
 * Used for things such as NPCs and Enemies
 */
class Entity : public Mutable {
protected:
  int type;  //!< The type of entity
  int speed;  //!< How many pixels the entity can move each frame
public:
  //! Gets the type of the entity
  int getType() const;
  //! Gets the speed of the entity
  int getSpeed() const;
  //! Sets the speed of the entity
  void setType(const int&);
  //! Sets the type of the entity
  void setSpeed(int);
  
};


//! Class used for player
/*!
 *  A special extension of mutable that works specifically for player.
 */
class Player : public Mutable {
protected:
  sf::Texture picture; //!< I don't remember what this is used for
  int speed; //!< How many pixels the player can move per frame
  Direction facingDir; //!< Which direction the player is facing
public:
  //! Initialized the player, This should be rewritten into a constructor
  void initialize();  //this should be made into a constructor
  //! Updates the player's sprite based on position
  /*!
   *  Sets the sprite's position based on where the player is located
   */
  void update();
  //! Gets the speed of the player
  int getSpeed() const;
  //! Gets the direction of the player
  Direction getFacing() const;
  //! Sets the speed of the player 
  void setSpeed(int);
  //! Sets the direction of the player
  void setFacing(Direction);
};

//! A class that represents a single node of the map
/*!
 *
 */
class MapNode {
private:
  int id;  //!< The identifier of which tile exists here
public:
  //! Gets the id of the node
  int getId() const;
  //! Sets the id of the node
  void setId(const int&);
  //! The sprite that can be drawn to the map
  /*! 
   *  This isn't initialized with a texture, as textures are assigned during
   *  Program execution, right before display drawing.
   */
  sf::Sprite area;
  //! The basic constructor. Sets id to 0 and area position to (0,0)
  MapNode();
};

//! The class that holds a complete level
/*!
 *  This is supposed to be fully encapsulated
 *  Mutable loading isn't complete yet
 */
class Level {
private:
  std::vector<std::vector<MapNode>> mapBase; //!< The level data of the map
  std::vector<Object> spriteList; //!< A list of objects found in the map
  std::vector<Entity> entityList; //!< A list of entities found in the map
  int tilesizeX;  //!< The width of a tile, in pixels? currently unused?
  int tilesizeY;  //!< The height of a tile, in pixels? currently unused?
public:
  //! returns a copy of specified node
  MapNode getNode(const int&, const int&) const;
  //! I'm not sure what this does
  void updateNode(const int&, const int&, const MapNode&);
  //! Uses a texturemap to assign a texture to specified node
  void assignTextureToNode(const int&, const int&, TextureMap&);
  //! Loads mapBase from file, specified by argument.
  /*! 
   *  Searches for file in /levels/
   */
  int loadLevel(const std::string&);
  //! Loads mutables from file, specified by argument.
  /*! 
   *  Searches for file in /levels/
   */
  void loadMutables(const std::string&);
  //! Gets map height
  int getHeight() const;
  //! Gets map width
  int getWidth() const;
  //! gets tile width
  int getTilesizeX() const;
  //! gets tile height
  int getTilesizeY() const;
  //! A constructor that sets the size of mapBase to (arg1, arg2)
  /*!
   *  Doesn't set tilesize
   */
  Level(const size_t&, const size_t&);
  //! The basic constructor, initializes mapBase to size 0
  /*!
   *  Doesn't set tilesize
   */
  Level();
};

//! A class that will eventually handle the playing of cutscenes
/*! 
 *  It has not been written yet, but will be able to display video-style
 *  cutscenes, which have no player input, and interactive cutscenes, which
 *  can (at least) print messages which the player can respond to
 */
class CutscenePlayer {
public:
  //! Doesn't do anything yet
  void PlayCutscene();
};

//! A class that is used to display menus
/*!
 *  Maybe I should add more stuff to this eventually
 */
class Menu {
public:
  //! The sprite that is drawn when the menu is activated
  sf::Sprite splash;
  //! The texture that splash is set to
  sf::Texture spT;

};
//! parses a string containing a node, uses nodify
bool nodify(std::string&, MapNode&);
//! creates a node from a string representation of a node
bool strToNode(const std::string&, MapNode&);





#endif
