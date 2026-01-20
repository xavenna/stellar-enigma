#ifndef PATH_H
#define PATH_H

#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <json/json.h>
#include <SFML/Graphics.hpp>

#include "misc/util.h"

class PathSeg {
  public:
  sf::Vector2f a;
  sf::Vector2f b;
  sf::Vector2f c;
  sf::Vector2f d;
  sf::Vector2f inter(float t) const;
  PathSeg(sf::Vector2f, sf::Vector2f, sf::Vector2f, sf::Vector2f);
};

//add functions to build this
class Path {
public:
  sf::Vector2f inter(float t) const;
  std::vector<PathSeg> segments;

};


#endif
