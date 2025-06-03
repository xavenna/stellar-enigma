#ifndef WATER_H
#define WATER_H
#include "mutable/object.h"

//! An animated water object. 
class Water : public Object {
public:
  virtual CacheNodeAttributes draw();
  virtual Interface behave(SwitchHandler*, Utility*);
  virtual std::string Name() const {return "water";}
  virtual sf::Vector2f BaseSize() const {return sf::Vector2f(16,16);}
  virtual Object::MotionType Type() const {return Object::Intangible;} 
  virtual bool verify();
  Water(int);
protected:
  unsigned animTimer=0;
  sf::Vector2i texSize;
  bool setup=false;
};




#endif
