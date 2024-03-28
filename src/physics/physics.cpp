#include "physics/physics.h"

Results rectangle_collide(const Object* o1, const Object* o2) {
  //center is used here because it makes different-sized objects work properly
  sf::Vector2f offset = o1->getCenter() - o2->getCenter();

  //snap the normal vector to an orthogonal or diagonal
  sf::Vector2f normal = norm(offset);

  if(std::abs(normal.x) < 0.7071) {
    normal.x = 0;
    normal.y = sign(normal.y) * 1;
  }
  else if(std::abs(normal.x) > 0.7072) {
    normal.x = sign(normal.x) * 1;
    normal.y = 0;
  }
  else {
    normal.x = sign(normal.x) * std::sqrt(2)/2;
    normal.y = sign(normal.y) * std::sqrt(2)/2;
  }
  
  //now, determine the collision depth. Depending on which way the vector points,
  //find the appropriate offset
  sf::Vector2f o1min{o1->getPos()};
  sf::Vector2f o1max{o1min+o1->getSize()-sf::Vector2f(1,1)};
  sf::Vector2f o2min{o2->getPos()};
  sf::Vector2f o2max{o2min+o2->getSize()-sf::Vector2f(1,1)};
  float bDist = o1max.y - o2min.y;
  float tDist = o2max.y - o1min.y;
  float lDist = o2max.x - o1min.x;
  float rDist = o1max.x - o2min.x;

  float vDist = (normal.y > 0 ? tDist : (normal.y < 0 ? bDist : 0));
  float hDist = (normal.x > 0 ? lDist : (normal.x < 0 ? rDist : 0));

  float depth = magnitude(sf::Vector2f{normal.x * hDist, normal.y * vDist});
  sf::Vector2f moveDist = depth * normal;

  Results r;

  //now, just determine how far along the tangents the objects move.
  float k1 = 0; 
  float k2 = 0;
  if(o1->Type() == Object::Sliding || o1->Type() == Object::Play || o1->Type() == Object::Entity) {
    if(o2->Type() == Object::Sliding || o2->Type() == Object::Play || o2->Type() == Object::Entity) {
      //two pushables
      k1 = 0.5;
      k2 = 0.5;  //when mass is readded, this will become based on mass ratio
    }
    else if(o2->Type() == Object::Intangible) {
      //no collision
      k1 = 0;
      k2 = 0;
    }
    else {
      k1 = 1;
      k2 = 0;
    }
  }
  else if(o1->Type() == Object::Intangible) {
    //no collision
    k1 = 0;
    k2 = 0;
  }
  else {
    if(o2->Type() == Object::Sliding || o2->Type() == Object::Play || o2->Type() == Object::Entity) {
      k1 = 0;
      k2 = 1;
      
    }
    else if(o2->Type() == Object::Intangible) {
      k1 = 0;
      k2 = 0;
    }
    else {
      k1 = 0;
      k2 = 0;
    }
  }
  r.first = -1.f * moveDist * k2;
  r.second = moveDist * k1;

  return r;
}


bool rect_intersect(const Object* o1, const Object* o2) {
  sf::Vector2f omin{o1->getPos()};
  sf::Vector2f omax{omin+o1->getSize()-sf::Vector2f(1,1)};

  sf::Vector2f bmin{o2->getPos()};
  sf::Vector2f bmax{bmin+o2->getSize()-sf::Vector2f(1,1)};


  if(bmin.x > omax.x || omin.x > bmax.x || bmin.y > omax.y || omin.y > bmax.y) {
    //no interaction
  }
  else {
    return true;
  }

  return false;
}
