#include "color.h"
#include <iostream>


//hue : 0..1
sf::Color tint(sf::Color base, float hue) {
  sf::Vector3f hsv = rgb2hsv(base);
  //alter the hue
  hsv.x = hue;
  return hsv2rgb(hsv);
}

//h ranges from 0 to 1
sf::Vector3f rgb2hsv(sf::Color base) {
  //convert base to hsv
  sf::Vector3f hsv;

  float r = base.r / 255.f;
  float g = base.g / 255.f;
  float b = base.b / 255.f;
  float Cmax = std::max(std::max(r,g),b);
  float Cmin = std::min(std::min(r,g),b);
  float delta = Cmax - Cmin;
  if(delta == 0) {
    hsv.x = 0;
  }
  else if(Cmax == r) {
    hsv.x = fmod((g-b)/delta,6);
  }
  else if(Cmax == g) {
    hsv.x = ((b-r)/delta) + 2;
  }
  else {
    hsv.x = ((r-g)/delta) + 4;
  }
  hsv.x = hsv.x / 6;
  hsv.y = (Cmax == 0) ? (0) : (delta / Cmax);
  hsv.z = Cmax;

  hsv.x = (hsv.x < 0) ? hsv.x+1 : hsv.x;
  hsv.y = (hsv.y < 0) ? hsv.y+1 : hsv.y;
  hsv.z = (hsv.z < 0) ? hsv.z+1 : hsv.z;
  return hsv;
}

//! Converts a hsv color (stored in a sf::Vector3f) to rgb. hsv: 0..1; rgb: 0..255
sf::Color hsv2rgb(sf::Vector3f p) {
  sf::Color o;

  float c = p.z * p.y;
  float h = p.x * 6;
  float x = c * (1 - fabs(fmod(h,2) - 1 ));
  float m = p.z - c;

  float r;
  float g;
  float b;


  if(h < 0 || h > 6) {
    h = 0; 
    std::clog << "Invalid color passed to hsv2rgb()\n";
  }

  switch(int(h)) {
  default: //case 0:
    r = c;
    g = x;
    b = 0;
    break;
  case 1:
    r = x;
    g = c;
    b = 0;
    break;
  case 2:
    r = 0;
    g = c;
    b = x;
    break;
  case 3:
    r = 0;
    g = x;
    b = c;
    break;
  case 4:
    r = x;
    g = 0;
    b = c;
    break;
  case 5:
    r = c;
    g = 0;
    b = x;
    break;
  }

  o.r = 255*(r+m);
  o.g = 255*(g+m);
  o.b = 255*(b+m);
  return o;
}
