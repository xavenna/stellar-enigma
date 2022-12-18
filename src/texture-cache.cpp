#include "texture-cache.h"


bool Transform::operator!=(Transform t) const {
  return !(type == t.type && args == t.args);
}

bool Transform::operator==(Transform t) const {
  return (type == t.type && args == t.args);
}

bool CacheNodeAttributes::operator==(CacheNodeAttributes attr) const {
  if(srcImg != attr.srcImg || tList.size() != attr.tList.size())
    return false;
  for(unsigned i=0;i<tList.size();i++) {
    if(tList[i] != attr.tList[i])
      return false;
  }
  return true;
}

CacheNode::CacheNode(CacheNodeAttributes attr) {
  srcImg = attr.srcImg;
  tList.resize(attr.tList.size());
  for(unsigned i=0;i<attr.tList.size();i++) {
    tList[i] = attr.tList[i];
  }
}

std::string TextureCache::hash(unsigned index) const {
  if(index < imgNameHash.size()) {
    return imgNameHash[index];
  }
  return "";
}

int TextureCache::reverseHash(const std::string& value) const {
  for(unsigned i=0;i<imgNameHash.size();i++) {
    if(value == imgNameHash[i]) {
      return i;
    }
  }
  return -1;
}

int TextureCache::objectFilenameHash(unsigned index) const {
  if(index >= objectListing.size()) {
    std::cout << "Error: requested object has no associated file\n";
    return -1;
  }
  int p = reverseHash(objectListing[index]);
  if(p < 0) {
    std::cout << "Error: reverseHash for '" << objectListing[index] << "' unsuccessful\n";
    return -1;
  }
  return p;
}
int TextureCache::tileFilenameHash(unsigned index) const {
  if(index >= tileListing.size()) {
    std::cout << "Error: requested tile has no associated file\n";
    return -1;
  }
  return reverseHash(tileListing[index]);
}
int TextureCache::entityFilenameHash(unsigned index) const {
  if(index >= entityListing.size()) {
    std::cout << "Error: requested entity has no associated file\n";
    return -1;
  }
  return reverseHash(entityListing[index]);
}

int TextureCache::searchCache(CacheNodeAttributes attr) const {
  // if texture exists, return its index. Else, return -1
  for(unsigned i=0;i<cache[attr.srcImg].size();i++) {
    if(cache[attr.srcImg][i] == attr) {
      return i;
    }
  }
  return -1;
}

sf::Texture& TextureCache::getTexture(CacheNodeAttributes attr) {
  if(attr.srcImg > imgNameHash.size()) {
    // requested image hasn't been registered yet
    throw std::invalid_argument("TextureCache::getTexture() : Requested Image hasn't been registered");
  }

  // now, search cache[attr.srcImg] for a CacheNode that matches attr
  int index = searchCache(attr);


  if(index >= 0) {
    // requested texture exists
    //std::cout << "Fetching texture from slot " << index << "\n\n";
    return cache[attr.srcImg][index].tex;
  }
  else {
    //std::cout << "Cache miss in subcache " << attr.srcImg << "\n";
    //cache miss
    //generate a texture with the requested transformations, create a CacheNode using it and
    //attr, and append it to the appropriate vector in Cache
    
    // make a copy of requested image
    sf::Image finalImage(images[attr.srcImg]);
    sf::IntRect window(0, 0, finalImage.getSize().x, finalImage.getSize().y);
    // iterate through attr.tList, apply each transformation
    for(auto x : attr.tList) {
      switch(x.type) { //apply each transformation
      case Transform::Slide_X:
	//slide the window horizontally
	window.left += x.args[0];
	break;
      case Transform::Slide_Y:
	//slide the window vertically
	window.top += x.args[0];
	break;
      case Transform::Set_Width:
	//set the width of the Texture
	window.width = x.args[0];
	break;
      case Transform::Set_Height:
	//set the height of the Texture
	window.height = x.args[0];
	break;
      default:
	//invalid transformation
	std::cout << "Error: invalid transformation. Skipped\n";
      }
    }
    //std::cout << "Writing texture\n";
    CacheNode c(attr);
    c.tex.loadFromImage(finalImage, window);
    //std::cout << "Placing texture into slot " << cache[attr.srcImg].size() << '\n';
    cache[attr.srcImg].push_back(c);
    //std::cout << "Creation successful\n\n";
    //create a CacheNode in the appropriate part of the cache. Use the final Image to make
    // the texture, and attr for the rest.

    //now return a reference to the generated texture
    return cache[attr.srcImg].back().tex;
  }

}


bool TextureCache::registerImage(sf::Image img, const std::string& name) {
  //attempt to check if image has been registered
  if(reverseHash(name) != -1) {
    //image name has already been used
    return false;
  }
  if(img.getSize().x == 0 || img.getSize().y == 0) {
    return false;
  }
  images.push_back(img);
  imgNameHash.push_back(name);
  cache.resize(cache.size()+1);
  return true;
}

TextureCache::TextureCache(const std::string& name) {
  std::ifstream get(name);
  std::string line;
  unsigned section = 0;
  if(!get.is_open()) {
    throw std::invalid_argument("TextureCache::TextureCache() : File not found\n");
  }
  while(get.peek() != EOF) {
    std::getline(get, line);
    if(line == "***") { //offset happening
      if(section == 0) {
	//switch to object
      }
      else if(section == 1) {
	//switch to entity
      }
      else {
	throw std::invalid_argument("TextureCache::TextureCache() : Extra separator found\n");
      }
      section++;
      continue;
    }
    //add line to the appropriate listing
    if(section == 0) {
      tileListing.push_back(line);
    }
    else if(section == 1) {
      objectListing.push_back(line);
    }
    else if(section == 2) {
      entityListing.push_back(line);
    }
    //load image, add it to images
    if(reverseHash(line) >= 0) {
      //duplicate image
      //don't add to images
      continue;
    }
    sf::Image im;
    //images.push_back(im);
    std::string fullLine = "assets/texture/" + line;
    im.loadFromFile(fullLine);
    registerImage(im, line);

  }
}
