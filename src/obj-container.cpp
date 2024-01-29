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
void ObjContainer::clearObjects() {
  for(auto x=list.begin();x!=list.end();std::advance(x, 1)) {
    delete *x;
  }
  list.clear();
}

Object& ObjContainer::getObjRef(unsigned ind) {
  if(ind > list.size()) {
    throw std::out_of_range("Invalid array address");
  }
  return **(std::next( list.begin(), ind ));
}

Object* ObjContainer::getObjPtr(unsigned ind) {
  if(ind > list.size()) {
    throw std::out_of_range("Invalid array address");
  }
  return *(std::next( list.begin(), ind ));
}
Object ObjContainer::getObj(unsigned ind) const {
  if(ind > list.size()) {
    throw std::out_of_range("Invalid array address");
  }
  return **(std::next( list.begin(), ind ));
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

// This is inefficient, but should work.
bool ObjContainer::hasObj(int id) {
  for(auto* x : list) {
    if(x->getLinkID() == id) {
      return true;
    }
  }
  return false;
}

void ObjContainer::removeObj(unsigned ind) {
  if(ind > list.size()) {
    throw std::out_of_range("Invalid array address");
  }
  auto p = std::next(list.begin(), ind);
  if(getObjPtr(ind)->getParentID() != -1) {
    //object has a parent. Notify
    //find object
    //generate a msg
    msg m;
    m.type = 'd';
    
    getObjByID(getObjPtr(ind)->getParentID())->notify(m);
  }
  delete getObjPtr(ind);
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
    return;
  }
  
}

bool ObjContainer::notify(msg m) {
  //check if an object with m.link is present.
  //a negative link_id means that the object has no actual link_id
  for(auto x : list) {
    if(x->getLinkID() == static_cast<int>(m.link)) {
      x->notify(m);
      return true;
    }
  }
  return false;
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
  list.back()->setPos(ob.getPos());
  list.back()->setSize(ob.getSize());
  list.back()->setSolid(ob.getSolid());
  list.back()->setText(ob.getText());
  list.back()->resetStatus();
  //list.back()->setActive(ob.getActive());
  //active is set to true in the object constructor, so this isn't needed now.
  //also, active is deprecated


  list.back()->setArgs(ob.getArgs());
  list.back()->setSwitches(ob.getSwitches());

  //now that object is set up properly, verify it.
  if(!list.back()->verify()) {
    std::clog << "Error: Invalid object configuration in object of type '";
    std::clog << list.back()->Name() << "'. Object wasn't added\n";
    removeObj(list.back());   
    return false;
    //object isn't added
  }
  counter++;
  return true;
}

bool ObjContainer::storeObj(std::string t) {
  Object o;
  return storeObj(o, t);
}
