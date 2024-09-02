#include "level/obj-container.h"

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

bool ObjContainer::hasUID(int uid) {
  for(auto* x : list) {
    if(x->getUID() == uid) {
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



bool ObjContainer::storeObj(std::string t) {
  Object o;
  std::string status;
  bool p = storeObj(o, t, status);
  if(!p) {
    std::cerr << status << '\n';
  }
  return p;
}
