#ifndef PHYSICS_H
#define PHYSICS_H

#include "mutable/object.h"
// the overarching physics include

typedef std::pair<sf::Vector2f, sf::Vector2f> Results;


bool rect_intersect(const Object* o1, const Object* o2);
bool circle_intersect(const Object* o1, const Object* o2);
Results circle_collide(const Object* o1, const Object* o2);
Results rectangle_collide(const Object* o1, const Object* o2);




#endif
