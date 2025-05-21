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
  //! In FollowPlayerCoarse, how far from the edge scrolling happens
  sf::Vector2f offset;
  //! Zoom scale factor
  float zoom;

  //! Rotation of view (measured in degrees)
  float angle;

  //! how large the screen is for a coarseFollow
  sf::Vector2f screenSize;
  //! 
  float scrollDistance;
  //! Camera Mode
  Mode mode;

  Config();
};

//! Describes the current state of a camera movement during a cutscene
/*!
 *  Currently, the way this is setup only works for linear animations.
 *  Fix this to handle logistic slides better
 */
class Animation {
public:
  enum Type {
    LinSlide, //!< Slide using Linear interpolation
    LogSlide, //!< Logistic slide
  };
  sf::Vector2f startPos;
  float startScale;
  float startAngle;

  //used for linear slides
  sf::Vector2f posStep;
  float scaleStep;
  float angleStep;

  //for logistic slides, step varies by frame.
  //these variables are used for calculating the motion
  //find my notes for logistic slides...

  Type type; //which animation type

  unsigned endFrame;
  unsigned currentFrame;

  std::string nextConfig;

  //these functions all give the current status. This allows different animation
  //styles to work
  //The float argument is the end point of the animation. This is used to fix camera
  //jumping when panning to a followClose config in mode 1. 
  float currentScale(float) const;
  float currentAngle(float) const;
  sf::Vector2f currentPos(sf::Vector2f) const;

};

//! A struct containing the data passed to Camera when requesting an animation
struct AnimDesc {
  unsigned duration; //!< The length of the animation
  std::string configName; //!< The config on which to end the animation
  Animation::Type type; //!< Animation type

};

class Camera {
public:

  float getScale(const Config& c);
  sf::Vector2f getFocus(const Config& c);
  float getAngle(const Config& c);

  void startAnimation(AnimDesc an);

  //! Checks if specified camera config is registered.
  bool configExists(const std::string& c);
  //! Switches to specified camera config. Return value indicates success.
  bool selectConfig(const std::string& c);
  sf::RenderTexture& drawFrame(sf::RenderWindow&, unsigned mode, TextureCache& cache);
  void gameplayDraw(sf::RenderWindow&, unsigned mode, TextureCache& cache);
  void cutsceneDraw(sf::RenderWindow&, unsigned mode, TextureCache& cache);
  //void startAnimation();
  Camera(Player&, Level&, const std::string&);
private:

  Player& p;
  Level& l;

  sf::RenderTexture frame;
  Config config;
  std::string currentConfig;
  Animation animStatus;
  bool inAnim=false;

  //! 
  sf::Vector2f focus={0,0};
  float zoom=1.f;
  float angle=0.f;

  std::map<std::string, Config> configurations;

};

void assignTexture(sf::Sprite& s, TextureCache& cache, NodeBase n);

bool generateConfig(json11::Json ob, Config& c, std::string&);
bool isValidConfigType(const std::string&);

Config::Mode mode(const std::string& n);

float lin_inter(float origin, float vec, float dist);
sf::Vector2f lin_inter(sf::Vector2f origin, sf::Vector2f vec, float dist);

float log_inter(float origin, float fin, float dist);
sf::Vector2f log_inter(sf::Vector2f origin, sf::Vector2f d, float dist);

#endif
