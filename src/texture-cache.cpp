#include "texture-cache.h"
#include "util.h"
#include <algorithm>


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
    std::cerr << "Requested Image hasn't been registered ("<<attr.srcImg<<")\n";
    throw std::invalid_argument("TextureCache::getTexture() : Requested Image hasn't been registered");
  }

  // now, search cache[attr.srcImg] for a CacheNode that matches attr
  int index = searchCache(attr);


  if(index >= 0) {
    // requested texture exists
    //std::clog << "Fetching texture from cache "<<attr.srcImg<<", slot " << index << "\n";
    return cache[attr.srcImg][index].tex;
  }
  else {
    //std::clog << "Cache miss in subcache " << attr.srcImg << "\n";
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
      case Transform::Tint_Color:
        //tint image by specified color
        {
          float hue = x.args[0] / 360.f;
          for(unsigned i=0;i<finalImage.getSize().x;i++) {
            for(unsigned j=0;j<finalImage.getSize().y;j++) {
              sf::Color c = finalImage.getPixel(i, j);
              c = tint(c, hue);
              finalImage.setPixel(i, j, c);
            }
          }
        }
        break;
      case Transform::Rotate: {
        int rotationAmount = (x.args[0] % 4);
        rotationAmount = 90 * (rotationAmount==0?2:rotationAmount==2?0:rotationAmount);
        sf::RenderTexture tex;
        tex.create(finalImage.getSize().x, finalImage.getSize().y);
        sf::Texture p;
        p.loadFromImage(finalImage);
        sf::Sprite s(p);
        s.setOrigin(finalImage.getSize().x/2.f, finalImage.getSize().y/2.f);
        s.setPosition(finalImage.getSize().x/2.f, finalImage.getSize().y/2.f);
        s.setRotation(rotationAmount);
        tex.draw(s);
        finalImage = tex.getTexture().copyToImage();

      }
      break;
      case Transform::Tint_Mask:
      case Transform::And_Mask:
      case Transform::Displacement_Mask:
        //not implemented
        std::cout << "Requested transformation has not been implemented yet\n";
        break;
      default:
        //invalid transformation
        std::cout << "Error: invalid transformation. Skipped\n";
        break;
      }
    }
    //std::cout << "Writing texture\n";
    CacheNode c(attr);
    c.tex.loadFromImage(finalImage, window);
    //std::cout << "Placing texture into slot " << cache[attr.srcImg].size() << '\n';
    cache[attr.srcImg].push_back(c);
    //std::cout << "Creation successful\n";
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
    std::cerr << "Error: duplicate name\n";
    return false;
  }
  if(img.getSize().x == 0 || img.getSize().y == 0) {
    std::cerr << "Error: image dimensions not valid\n";
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
    if(line.size() == 0 || line[0] == '#') {
      continue;
    }
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
    sf::IntRect box = getRect(line);
    std::string srcImg = getFile(line);
    std::string imgName = getName(line);
    std::string fullLine = "assets/texture/" + srcImg;
    //add line to the appropriate listing
    if(section == 0) {
      tileListing.push_back(imgName);
    }
    else if(section == 1) {
      objectListing.push_back(imgName);
    }
    else if(section == 2) {
      entityListing.push_back(imgName);
    }
    else {
      std::cerr << "Error: invalid section\n";
    }
    //load image, add it to images
    if(reverseHash(line) >= 0) {
      //duplicate image
      //don't add to images
      continue;
    }
    sf::Image im;
    if(box.width == 0 && box.height == 0) {
      im.loadFromFile(fullLine);
    }
    else {
      sf::Image i;
      i.loadFromFile(fullLine);
      im.create(box.width, box.height);
      im.copy(i, 0, 0, box, false);
    }
    registerImage(im, imgName);

  }
}


// line format:
// filename`x,y`wid,hei

sf::IntRect getRect(std::string line) {
  if(line.find('`') == std::string::npos) {
    return sf::IntRect(0,0,0,0);
  }
  std::vector<std::string> seg;
  std::vector<std::string> p;
  std::vector<std::string> s;

  parse(line, seg, "`");
  parse(seg[2], p, ",");
  parse(seg[3], s, ",");

  try {
    sf::Vector2i pos(std::stoi(p[0]),std::stoi(p[1]));
    sf::Vector2i size(std::stoi(s[0]),std::stoi(s[1]));
    return sf::IntRect(pos,size);
  }
  catch (...) {
    return sf::IntRect(0,0,0,0);
  }
}
std::string getFile(std::string line) {
  std::vector<std::string> seg;
  parse(line, seg, "`");
  return seg[1];
}

std::string getName(std::string line) {
  std::vector<std::string> seg;
  parse(line, seg, "`");
  return seg[0];
}
