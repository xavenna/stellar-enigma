#include "level-util.h"
#include <cmath>

// this provides some map utility functions, such as collision detection and screen scrolling

//tbh, this could probably be encapsulated within Level (or maybe player?)
int validMove(Level& l, Player& player) {
  //this needs to be modified to handle objects and entities

  //convert player coordinates to level coordinates
  int playX = int(player.getXPos() / l.getTilesizeX());
  int playY = int(player.getYPos() / l.getTilesizeY());
  int phx = player.getXPos() + player.getWidth();
  int phy = player.getYPos() + player.getHeight();
  //int proPlX = player.getXPos();  //prospective position
  //int proPlY = player.getYPos();
  bool fullMove = true;
  int moveDistance = player.getSpeed();
  int tempSpeed = player.getSpeed();


  //find destination square
  switch(player.getFacing()) {
  case 0:
    //up
    if(player.getYPos() - player.getSpeed() < 0) {
      fullMove = false;
      moveDistance = player.getYPos();
    }
    if(playY != 0) {
      int numTiles = int(player.getWidth() / l.getTilesizeX());
      int extraTile = (phx % l.getTilesizeX() == 0) ? 0 : 1;
      for(int i=0; i<numTiles+extraTile; i++) {
	int cxP = player.getXPos() + i * l.getTilesizeX();
	if(cxP > phx)
	  cxP = phx;
	if(!passableSpace(l.getNode(int(cxP/l.getTilesizeX()), int(player.getYPos()/l.getTilesizeY())-1).getId())) {
	  tempSpeed = player.getYPos() - playY * l.getTilesizeY();
	  moveDistance = moveDistance > tempSpeed ? tempSpeed : moveDistance;
	}
      }
      fullMove = false;
    }
    //test object collision
    break;
  case 1:
    if(phx + player.getSpeed() >= l.getWidth() * l.getTilesizeX()) {
      fullMove = false;
      moveDistance = l.getWidth() * l.getTilesizeX() - phx;
    }
    if(playX != l.getWidth()) {
      int numTiles = int(player.getHeight() / l.getTilesizeY());
      int extraTile = (phy % l.getTilesizeY() == 0) ? 0 : 1;
      for(int i=0;i<numTiles+extraTile; i++) {
	int cyP = player.getYPos() + i * l.getTilesizeY();
	if(cyP > phy)
	  cyP = phy;
	if(!passableSpace(l.getNode(int((phx)/l.getTilesizeX()), int(cyP/l.getTilesizeY())).getId())) {
	  tempSpeed = int(phx / l.getTilesizeX()) * l.getTilesizeX() - phx;
	  moveDistance = moveDistance > tempSpeed ? tempSpeed : moveDistance;
	}
      }
      fullMove = false;
    }
    break;
  case 2:
    if(phy + player.getSpeed() >= l.getHeight() * l.getTilesizeY()) {
      fullMove = false;
      moveDistance = l.getHeight() * l.getTilesizeY() - phy;
    }
    if(playY != l.getHeight()) {
      int numTiles = int(player.getWidth() / l.getTilesizeX());
      int extraTile = (phx % l.getTilesizeX() == 0) ? 0 : 1;
      for(int i=0; i<numTiles+extraTile; i++) {
	int cxP = player.getXPos() + i * l.getTilesizeX();
	if(cxP > phx)
	  cxP = phx;
	if(!passableSpace(l.getNode(int(cxP/l.getTilesizeX()), int((phy)/l.getTilesizeY())).getId())) {
	  tempSpeed = int(phy / l.getTilesizeY()) * l.getTilesizeY() - phy;
	  moveDistance = moveDistance > tempSpeed ? tempSpeed : moveDistance;
	}
      }
      //std::cout << moveDistance << '\n';
      fullMove = false;
    }
    break;
  case 3:
    if(player.getXPos() - player.getSpeed() < 0) {
      fullMove = false;
      moveDistance = player.getXPos();
    }
    if(playX != 0) {
      int numTiles = int(player.getHeight() / l.getTilesizeY());
      int extraTile = (phy % l.getTilesizeY() == 0) ? 0 : 1;
      for(int i=0; i<numTiles+extraTile; i++) {
	int cyP = player.getYPos() + i * l.getTilesizeY();
	if(cyP > phy)
	  cyP = phy;
	if(!passableSpace(l.getNode(int(player.getXPos()/l.getTilesizeX())-1, int(cyP/l.getTilesizeY())).getId())) {
	  tempSpeed = player.getXPos() - playX * l.getTilesizeX();
	  moveDistance = moveDistance > tempSpeed ? tempSpeed : moveDistance;
	}
      }
      fullMove = false;
    }
    break;
  }
  return fullMove ? player.getSpeed() : moveDistance;
}
bool passableSpace(int n) {
  switch(n) {
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 8:
    return false;
  default:
    return true;
  }
}
