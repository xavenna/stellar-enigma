#ifndef CAMERA_H
#define CAMERA_H

#include <string>
#include <fstream>
#include <json11.hpp>
#include "SFML/Graphics.hpp"
#include "level/level.h"
#include "mutable/player.h"
#include "physics/physics.h"


//! Describes the current camera configuration
/*! 
 *  For now, many of these fields will be unused, depending on mode.
 *
 *  For example, the close follow player mode doesn't use focus
 */
class Config {
public:
  enum Mode {
    FollowPlayerClose,
    FollowPlayerCoarse,
    Fixed
  };
  //! In fixed mode, the camera's focus point (in map units)
  sf::Vector2f focus;
  //! In followPlayer modes, 
  sf::Vector2f offset;
  //! Zoom scale factor
  float zoom;

  sf::Vector2f screenSize;
  float scrollDistance;
  Mode mode;

  Config();
};

//! Describes the current state of a camera movement during a cutscene
/*!
 *  
 */
class Animation {
public:
  sf::Vector2f startPos;
  sf::Vector2f posStep;
  float startScale;
  float scaleStep;
  float startAngle;
  float angleStep;

  unsigned endFrame;
  unsigned currentFrame;

  std::string nextConfig;

};

//! A struct containing the data passed to Camera when requesting an animation
struct AnimDesc {
  unsigned duration; //!< The length of the animation
  std::string configName; //!< The config on which to end the animation

};

class Camera {
public:

  float getScale(const Config& c);
  sf::Vector2f getFocus(const Config& c);

  void startAnimation(AnimDesc an);

  sf::RenderTexture& drawFrame(sf::RenderWindow&, unsigned mode, TextureCache& cache);
  void gameplayDraw(sf::RenderWindow&, unsigned mode, TextureCache& cache);
  void cutsceneDraw(sf::RenderWindow&, unsigned mode, TextureCache& cache);
  void startAnimation();
  Camera(Player&, Level&, const std::string&);
private:

  Player& p;
  Level& l;

  sf::RenderTexture frame;
  Config config;
  Animation animStatus;
  bool inAnim=false;

  //! 
  sf::Vector2f focus={0,0};
  float zoom=1.f;
  float angle=0.f;

  std::map<std::string, Config> configurations;

};

void assignTexture(sf::Sprite& s, TextureCache& cache, NodeBase n);




#endif
