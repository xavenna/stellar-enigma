#ifndef COLOR_H
#define COLOR_H
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <cmath>

//! Converts a rgb color to a hsv color (represented as a Vector3)
sf::Vector3f rgb2hsv(sf::Color base);
//! Converts an HSV color (as Vector3) to an RGB color
sf::Color hsv2rgb(sf::Vector3f b);
//! Modifies passed RGB color so its hue matches passed value
sf::Color tint(sf::Color base, float hue);


#endif
