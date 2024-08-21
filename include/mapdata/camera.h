#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>

//! Defines camera parameters
class CameraConfig {
public:
  //! Camera type (for modes other than cutscenes)
  enum Mode {
    Fixed,
    Close_Follow,
    Screen_Follow
  };
  float zoomFactor;
  sf::Vector2f focusPoint;
  Mode mode;



};

//! A utility class for handling gameplay drawing
/*!
 * 
 */
class Camera {
public:
  enum Mode {
    Cutscene,
    Gameplay,
    Worldmap
  };
  //! Renders and draws the current frame
  void drawFrame(sf::RenderWindow& win);

  void setMode(Mode);

  bool loadCameraConfig(const std::string& name);


private:

};





#endif
