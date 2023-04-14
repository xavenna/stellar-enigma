#ifndef COLOR_H
#define COLOR_H
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <cmath>

sf::Vector3f rgb2hsv(sf::Color base);
sf::Color hsv2rgb(sf::Vector3f b);
sf::Color tint(sf::Color base, float hue);


#endif
