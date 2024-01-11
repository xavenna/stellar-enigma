#include "obj-container.h"

std::size_t ObjContainer::size() const {
  return list.size();
}

ObjContainer::ObjContainer() {

}

ObjContainer::~ObjContainer() {
  for(auto x : list) {
    delete x;
  }
}

Object& ObjContainer::getObjRef(unsigned ind) {
  if(ind > list.size()) {
    throw std::out_of_range("Invalid array address");
  }
  return *(list[ind]);
}

Object* ObjContainer::getObjPtr(unsigned ind) {
  if(ind > list.size()) {
    throw std::out_of_range("Invalid array address");
  }
  return (list[ind]);
}
Object ObjContainer::getObj(unsigned ind) const {
  if(ind > list.size()) {
    throw std::out_of_range("Invalid array address");
  }
  return *(list[ind]);
}

Object* ObjContainer::getObjByID(int lid) {
  if(lid < 0) {
    throw std::invalid_argument("Error: Null link id request invalid");
  }
  for(auto* x : list) {
    if(x->getLinkID() == lid) {
      return x;
    }
  }
  throw std::invalid_argument("Error: Requested Object could not be found");
}


void ObjContainer::removeObj(unsigned ind) {
  if(ind > list.size()) {
    throw std::out_of_range("Invalid array address");
  }
  auto p = list.begin() + ind;
  if(list[ind]->getParentID() != -1) {
    //object has a parent. Notify
    //find object
    //generate a msg
    msg m;
    m.type = 'd';
    
    getObjByID(list[ind]->getParentID())->notify(m);
  }
  delete list[ind];
  list.erase(p);
}


void ObjContainer::removeObj(Object* ob) {

  
  auto p = std::find(list.begin(), list.end(),ob);
  if (p!=list.end()) {
    //object exists within vector
    //check if ob has a parent.
    //if so, notify parent.
    if(ob->getParentID() != -1) {
      //object has a parent. Notify
      //find object
      //generate a msg
      msg m;
      m.type = 'd';
      
      ob->notify(m);
    }
    delete ob;
    list.erase(p);
  }
  
}

bool ObjContainer::notify(msg m) {
  //check if an object with m.link is present.
  for(auto x : list) {
    if(x->getLinkID() == m.link) {
      x->notify(m);
      return true;
    }
  }
  return false;
}


// this is kind of janky...see if there's a better way to do this
// Deprecated
ObjContainer::Type getType(unsigned p) {
  switch(p) {
  case 0:
    return ObjContainer::solid;
  case 1:
    return ObjContainer::pushable;
  case 2:
    return ObjContainer::key;
  case 3:
    return ObjContainer::board;
  case 4:
    return ObjContainer::cutscene_player;
  case 5:
    return ObjContainer::spike;
  case 6:
    return ObjContainer::toggle_block;
  case 7:
    return ObjContainer::door;
  default:
    return ObjContainer::obj;
  }
}

bool ObjContainer::storeObj(Object ob, std::string t) {
  if(t == "obj") {
    list.push_back(new Object(counter));
  }
  else if(t == "solid") {
    list.push_back(new Solid(counter));
  }
  else if(t == "pushable") {
    list.push_back(new Pushable(counter));
  }
  else if(t == "key") {
    list.push_back(new Key(counter));
  }
  else if(t == "board") {
    list.push_back(new Board(counter));
  }
  else if(t == "cutscene_player") {
    list.push_back(new CutPlay(counter));
  }
  else if(t == "spike") {
    list.push_back(new Spike(counter));
  }
  else if(t == "toggle_block") {
    list.push_back(new ToggleBlock(counter));
  }
  else if(t == "door") {
    list.push_back(new Door(counter));
  }
  else {
    std::clog << "Error: Invalid object type\n";
    return false;
  }
  //copy all externally-set attributes of the passed object to list.back();
  list.back()->setLinkID(ob.getLinkID());
  list.back()->setTextureID(ob.getTextureID());
  list.back()->setParentID(ob.getParentID());
  list.back()->setId(ob.getId());
  
  list.back()->setPos(ob.getPos());
  list.back()->setSize(ob.getSize());
  list.back()->setSolid(ob.getSolid());
  list.back()->setText(ob.getText());
  //list.back()->setActive(ob.getActive());
  //active is set to true in the object constructor, so this isn't needed now.


  list.back()->setArgs(ob.getArgs());
  list.back()->setSwitches(ob.getSwitches());
  counter++;
  return true;
}

bool ObjContainer::storeObj(std::string t) {
  Object o;
  return storeObj(o, t);
}
