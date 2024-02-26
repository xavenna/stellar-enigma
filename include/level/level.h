#ifndef LEVEL_H
#define LEVEL_H

#include "misc/format.h"
#include "level/mapnode.h"
#include "mutable/object.h"
#include "utility/texture-cache.h"
#include "mutable/player.h"
#include <iostream>
#include <fstream>
#include "level/field.h"
#include "level/obj-container.h"
#include "utility/switch-handler.h"
#include "mapdata/message.h"
#include "utility/utility.h"
#include "misc/util.h"
#include "utility/save.h"




//! A utility class that represents an interaction
/*!
 *  Used during the interaction handling process
 */
class Inter {
public:
  Inter(Object*, const Player&); //!< Use for player-object interactions
  Inter(Object*, Object*);  //!< Use for object-object interactions
  void calculatePriority();  //!< Calculates the priority based on the interaction
  int priority;   //!< Used for ranking interactions, priority of higher ranked actor
  int subpriority;  //!< priority of lower ranked actor
  //signify which two things are interacting
  bool player1; //is the player object 1?
  bool player2; //is the player object 2?
  Object* o1;  //!< The first object involved, always should be populated
  Object* o2;  //if player is object1, should be nullptr 
  Player p;  //!< If player is involved in interaction, 
};

//! A class that manages the level field and objects
/*!
 *  Contains all tiles and objects
 *  Handles tile collision
 */
class Level {
private:
  ObjContainer objects; //!< The object factory/container
  unsigned tilesizeX;  //!< The width of a tile, in pixels
  unsigned tilesizeY;  //!< The height of a tile, in pixels
  int winOffX = 0;
  int winOffY = 0;
  int frameCount = 0; //!< The current frame, modulo 1800 (used for animations)
public:
  Field field; //!< The tile field
  //! returns a copy of specified node
  NodeBase getNode(int, int) const;
  //! returns a copy of specified object
  Object getObj(unsigned index) const;
  //! returns a reference to specified object
  Object& getObjRef(unsigned);
  //! returns a pointer to specified object
  Object* getObjPtr(unsigned);
  //! returns pointer to object with specified link id
  Object* getObjLinkPtr(int);
  //! determines if obj-container has specified object
  bool hasObj(int);
  //! sends notification to object
  bool notifyObj(msg);
  //! a fixed-size map that is actually drawn to the screen
  std::array<std::array<MapNode, WINDOW_HEIGHT>, WINDOW_WIDTH> window;
  //! overwrites the mapnode at position x,y with node
  void updateNode(int x, int y, const NodeBase& node);
  //! copies the necessary nodes over to window in order to set it up
  void readyWindow(int xscr, int yscr);
  //! sets the positions of the sprites of the window
  void updateWindowPos();
  //! Uses a texturemap to assign a texture to specified node in window
  void assignTextureToWinNode(sf::Vector2i, TextureCache&);
  //! Uses a texturemap to assign a texture to the specified object
  void assignTextureToObject(unsigned, TextureCache&);
  //! Loads mapBase from file, specified by argument.
  /*! 
   *  Searches for file in assets/level/
   */
  int loadLevel(const std::string&);
  //! Loads mutables from file, specified by argument.
  /*! 
   *  Searches for file in /assets/level/
   */
  bool loadMutables(const std::string&);
  //! Gets map height
  int getHeight() const;
  //! Gets map width
  int getWidth() const;
  //! gets tile width
  int getTilesizeX() const;
  //! gets tile height
  int getTilesizeY() const;
  //! gets tilesize as a sf::Vector2i
  sf::Vector2i getTilesize() const;
  //! gets the number of objects
  unsigned getObjNum() const;

  //! Adds passed Object to objectList, as specified type.
  void addObject(const Object& ob, const std::string&);
  //! Removes specified object from object list
  void removeObject(unsigned index);
  //! Remove object by pointer to it
  void removeObject(Object* ob);

  //void handleInteractions();

  //! Handles objects
  /*!
   *  Updates entity tile positions, handles object behavior, etc.
   */
  Interface handleObjects(sf::Vector2i pos, sf::Vector2i size, SwitchHandler*, Utility*);
  //! Resets last pos for objects
  void resetObjDeltas();
  //! Tells whether object is on the correct screen to be displayed
  bool displayObject(unsigned index, sf::Vector2i ppos, sf::Vector2i size) const;

  //! Determines how far the player can move before running into an obstacle
  sf::Vector2i validMove(sf::Vector2i pos, sf::Vector2i size, sf::Vector2i speed) const;
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
  //! This can be set to true to request a display update
  bool displayUpdate = true;
  //! Advance internal frame count
  int advanceFrameCount();
};

//! creates an object from a string representation of an object. Kept for compatibility
bool str2obj(const std::string& line, Object& node);
//! new version of str2obj2, for new object format.
bool str2obj2(const std::string& line, Object& node, std::string&);

bool generateObjFromObjAttrList(const std::list<ObjAttr>& attribs, Object& obj, std::string& objType);

bool checkInteraction(Object*, Object*);
#endif
