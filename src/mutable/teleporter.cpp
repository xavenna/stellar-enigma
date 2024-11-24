#include "mutable/teleporter.h"

Teleporter::Teleporter(int uid) : Object(uid) {
  cooldown = 0;
  chargeup = 0;
}

bool Teleporter::verify() {
  if(args[0] < 0) {
    return false;
  }
  if(args[1] < 0 || args[1] > 2) {
    return false;
  }
  if(args[2] < 0 || args[2] > 3) {
    return false;
  }
  if(args[3] < 0 || args[3] > 2) {
    return false;
  }
  if(args[4] < 0) {
    return false;
  }

  //fix this: SW_A only needs to be set if arg[2] & 2 == 1
  if(!((args[2] & 0x2) >> 1) && (switches[SW::A] < 0 || switches[SW::A] > 255)) {
    return false;
  }

  return true;
}



CacheNodeAttributes Teleporter::draw(const TextureCache* cache) {
  CacheNodeAttributes cna;
  if(args[3] == 0) {
    cna.srcImg = cache->reverseHash("null");
  }
  else if(args[3] == 1) {
    cna.srcImg = cache->reverseHash("warp-area");
  }

  //maybe eventually scale the texture by object's size
  return cna;
}
//this will do things, probably
Interface Teleporter::behave(SwitchHandler*, Utility*) {
  Interface i;
  if(cooldown != 0) {
    cooldown--;
  }
  return i;
}

Interface Teleporter::interact(Object* o, Field*, SwitchHandler* sh) {
  Interface in;
  if(cooldown != 0) {
    return in;
  }
  //this should play a sound effect upon teleporting...
  //check if o->Type() is compatible with args[1]

  if((args[1] == 1 && o->Type() != Object::Play) || (args[1] == 2 && (o->Type() != Object::Play || o->Type() != Object::Entity))) {
    return in;
  }
  if(!((args[2] & 0x2) >> 1) || sh->read(switches[SW::A] == true) ) {
    std::cerr << chargeup << ',' << cooldown << '\n';
    //teleport object
    chargeup++;
    if(static_cast<int>(chargeup) == args[0]) {
      o->setPos(sf::Vector2f(args[6], args[7]));
      sh->write(switches[SW::B], true);
      chargeup = 0;
      cooldown = args[4];
    }
  }

  if(args[2] & 0x1) {
    //despawn self
    status = Object::Destroy;

  }
  return in;
}
