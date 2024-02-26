#include "utility/texture-cache.h"


bool Transform::operator!=(Transform t) const {
  return !(type == t.type && args == t.args && text == t.text);
}

bool Transform::operator==(Transform t) const {
  return (type == t.type && args == t.args && text == t.text);
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

unsigned CacheNodeAttributes::src() {
  return static_cast<unsigned>(srcImg);
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

unsigned TextureCache::reverseHash(const std::string& value) const {
  for(unsigned long i=0;i<imgNameHash.size();i++) {
    if(value == imgNameHash[i]) {
      return i;
    }
  }
  return 0;
}

bool TextureCache::hasImage(const std::string& value) const {
  for(unsigned long i=0;i<imgNameHash.size();i++) {
    if(value == imgNameHash[i]) {
      return true;
    }
  }
  return false;
}

int TextureCache::searchCache(CacheNodeAttributes attr) const {
  // if texture exists, return its index. Else, return -1
  for(unsigned long i=0;i<cache[attr.srcImg].size();i++) {
    if(cache[attr.srcImg][i] == attr) {
      return static_cast<int>(i);
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


  if(index >= 0) { //index is guaranteed to be nonnegative
    // requested texture exists
    //std::clog << "Fetching texture from cache "<<attr.srcImg<<", slot " << index << "\n";
    return cache[attr.srcImg][static_cast<unsigned>(index)].tex;
  }
  else {
    //std::clog << "Cache miss in subcache " << attr.srcImg << "\n";
    //cache miss
    //generate a texture with the requested transformations, create a CacheNode using it and
    //attr, and append it to the appropriate vector in Cache
    
    // make a copy of requested image
    sf::Image finalImage(images[attr.srcImg]);
    sf::IntRect window(0, 0, static_cast<int>(finalImage.getSize().x), static_cast<int>(finalImage.getSize().y));
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
        int rotationAmount = 90*(x.args[0] % 4);
        sf::RenderTexture tex;
        tex.create(finalImage.getSize().x, finalImage.getSize().y);
        sf::Texture p;
        p.loadFromImage(finalImage);
        sf::Sprite s(p);
        s.setOrigin(finalImage.getSize().x/2.f, finalImage.getSize().y/2.f);
        s.setPosition(finalImage.getSize().x/2.f, finalImage.getSize().y/2.f);
        s.setRotation(rotationAmount);
        tex.draw(s);
        tex.display();
        finalImage = tex.getTexture().copyToImage();

      }
      break;
      case Transform::Tint_Mask:
      case Transform::And_Mask:
      case Transform::Displacement_Mask:
        //not implemented
        std::cout << "Requested transformation has not been implemented yet\n";
        break;
      case Transform::Add_Text:
      {

        // parse text for special escape sequences
        textEscape(x.text, save);
        //adds text at (arg0, arg1), with size arg2 [ and wrap width arg3, eventually]
        sf::RenderTexture tex;
        tex.create(finalImage.getSize().x, finalImage.getSize().y);

        sf::Texture p;
        p.loadFromImage(finalImage);
        sf::Sprite s(p);
        s.setPosition(0,0);

        tex.draw(s);
        sf::Text t;
        sf::Font courier;  //this is quite inefficient
        courier.loadFromFile("assets/cour.ttf");
        t.setFont(courier);
        t.setString(x.text);
        t.setFillColor(sf::Color::White);
        t.setCharacterSize(static_cast<unsigned>(x.args[2]));
        t.setPosition(x.args[0], x.args[1]);
        tex.draw(t);
        tex.display();
        finalImage = tex.getTexture().copyToImage();

        //implement wrapping later
      }
        break;
      case Transform::SubRect:
        window.left += x.args[0];
        window.top += x.args[1];
        window.width = x.args[2];
        window.height = x.args[3];
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
  if(hasImage(name)) {
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

TextureCache::TextureCache(const std::string& name, SaveController& s) : save{s} {

  std::ifstream get(name);
  std::string line;
  if(!get.is_open()) {
    throw std::invalid_argument("TextureCache::TextureCache() : File not found\n");
  }
  while(get.peek() != EOF) {
    std::getline(get, line);
    if(line.size() == 0 || line[0] == '#') {
      continue;
    }
    sf::IntRect box = getRect(line);
    std::string srcImg = getFile(line);
    std::string imgName = getName(line);
    std::string fullLine = "assets/texture/" + srcImg;
    //load image, add it to images
    if(hasImage(line)) {
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
      im.create(static_cast<unsigned>(box.width), static_cast<unsigned>(box.height));
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

  parse(line, seg, "`"); //
  if(seg.size() > 6) {
    return sf::IntRect(0,0,0,0);
  }

  try {
    sf::Vector2i pos(std::stoi(seg[2]),std::stoi(seg[3]));
    sf::Vector2i size(std::stoi(seg[4]),std::stoi(seg[5]));
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


void textEscape(std::string& str, const SaveController& save) {
  //go through the text, looking for % characters. If one appears, check is specified
  //variable exists. If so, substitute it.
  std::string result;
  std::string temp;
  result.reserve(str.size());
  for(unsigned i=0;i<str.size();i++) {
    auto x =  str[i];
    if(x == '%') {
      //attempt to parse a variable name.
      unsigned tpos = str.find('~', i+1);
      if(str.find('%', i+1) == i+1) {
        //sequence is an escaped percent sign
        result += '%';
      }
      else if(tpos == std::string::npos) {
        //no terminating tilde found
        //log an error, don't modify text
        std::clog << "Error: No terminating tilde found in text transform\n";
        return;
      }
      else {
        //terminating tilde found, get
        temp = str.substr(i+1, tpos-i);
        //attempt to parse temp as a varname
        if(temp.size() < 2) {
          //invalid varname
          //eat everything up until the terminating tilde, log an error
          std::clog << "Error: Invalid varname in escape sequence\n";
          i = tpos+1;
        }
        else {
          char t = temp[0];
          std::string n = temp.substr(1,temp.size()-2);
          if(t == 'i' && save.hasInt(n)) {
            result += std::to_string(save.getInt(n));
          }
          else if(t == 'f' && save.hasFloat(n)) {
            result += std::to_string(save.getFloat(n));
          }
          else if(t == 's' && save.hasStr(n)) {
            result += save.getStr(n);
          }
          else if(t == 'b' && save.hasBool(n)) {
            result += (save.getBool(n) ? "1" : "0");
          }
          else {
            //invalid vartype or varname
            std::clog << "Error: Invalid varname '"<<temp<<"' in escape sequence\n";
          }
        
        }
        i = tpos+1; //advance cursor to be directly after terminating tilde
      }
    }
    else {
      result += x;
    }

  }
  str = result;

}
