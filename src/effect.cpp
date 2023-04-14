#include "effect.h"


void Effect::setScale(float sc) {
  scale = sc;
}
void Effect::setType(Effect::Type t) {
  type = t;
}
float Effect::getScale() const {
  return scale;
}
Effect::Type Effect::getType() const {
  return type;
}

int Effect::getRemainingTime() const {
  return timer;
}
int Effect::decrementTimer() {
  return timer>0?(--timer):(timer==-1)?-1:0;
}

Effect::Effect(Effect::Type t, int time) : type{t} {
  timer = time;
}
