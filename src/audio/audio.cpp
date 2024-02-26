
#include "audio/audio.h"
#include <iostream>

// Audio Module:
// This contains the audio player, as well as the functions used to control it

SoundRegistry::SoundRegistry(const std::string& name) {
  std::string path = "assets/audio/";
  std::string fname = path + name;
  std::ifstream load(fname);
  std::string line;
  std::string regname;
  std::string file;
  if(!load.is_open()) {
    std::cerr << fname << '\n';
    throw std::invalid_argument("SoundRegistry::SoundRegistry() : File not found\n");
  }
  while(load.peek() != EOF) {
    std::getline(load, line);
    if(line.size() == 0 || line[0] == '#') {
      continue;
    }
    //parse this as string`filename
    unsigned pos = line.find('`');
    if(pos == std::string::npos) {
      throw std::invalid_argument("SoundRegistry::SoundRegistry() : Malformed entry\n");
    }
    regname = line.substr(0, pos);
    file = line.substr(pos+1);
    
    sf::SoundBuffer sound;
    sound.loadFromFile(path + file);

    registerSound(regname, sound);

  }
}


void SoundRegistry::registerSound(const std::string& name, sf::SoundBuffer soundBuf) {
  registry.emplace(name, soundBuf);
}


sf::SoundBuffer& SoundRegistry::getSound(std::string name) {
  if(registry.find(name) == registry.end()) {
    std::cerr << "Error: request to get unregistered sound '" << name << "' failed\n";
    return registry.begin()->second;
  }
  return registry[name];
}

int SoundRegistry::getNumSounds() {
  return registry.size();
}


void MusicPlayer::registerSound(const std::string& name, sf::SoundBuffer buffer) {
  SR.registerSound(name, buffer);
}
sf::SoundBuffer& MusicPlayer::getSound(std::string name) {
  return SR.getSound(name);
}

void MusicPlayer::playMusic(const std::string& filename) {
  std::string nfn = "assets/audio/" + filename;
  if(!music.openFromFile(nfn)) {
    std::cout << "Error: file '" << nfn << "' not found\n";
  }
  music.play();
}
bool MusicPlayer::findOpenSlot(int& slotNum) {
  for(unsigned i=0;i<8;i++) {
    if(soundStatus[i] == 0) {
      slotNum = static_cast<int>(i);
      return true;
    }
  }
  return false;
}
void MusicPlayer::queueSound(const std::string& soundName) {
  soundQueue.push_back(soundName);
}
std::string MusicPlayer::dequeueSound() {
  if(soundQueue.size() == 0)
    return "";
  std::string n = soundQueue.front();
  soundQueue.pop_front();
  return n;
}
void MusicPlayer::assignSounds() {
  int slot=-1;
  bool open;
  do {
    if(soundQueue.size() == 0)
      break;
    open = findOpenSlot(slot);
    if(!open) {
      break;
    }
    //if the code reaches here, slot is guaranteed to be positive
    std::string name = dequeueSound();
    if(name == "")
      continue;
    try {
      soundPlayer[static_cast<unsigned>(slot)].setBuffer(SR.getSound(name));
      soundStatus[static_cast<unsigned>(slot)] = Assigned;
    }
    catch (...) {
      continue;
    }

  }while(!open);
}
void MusicPlayer::manageSounds() {
  for(unsigned i=0;i<8;i++) {
    switch(soundStatus[i]) {
    case Playing:
      if(soundPlayer[i].getStatus() == sf::SoundSource::Stopped) {
        //free up slot
        soundStatus[i] = Ready;
      }
      break;
    case ReservedPlaying:
      if(soundPlayer[i].getStatus() == sf::SoundSource::Stopped) {
        //free up slot
        soundStatus[i] = Reserved;
      }
      break;
    case Assigned:
      soundPlayer[i].play();
      if(!(soundPlayer[i].getStatus() == sf::SoundSource::Playing)) {
        soundStatus[i] = Ready;
      }
      else {
        soundStatus[i] = Playing;
      }
      break;
    case ReservedAssigned:
      soundPlayer[i].play();
      soundStatus[i] = ReservedPlaying;
      break;
    default:
      //nothing needs to be done
      break;
    }
  }
}
MusicPlayer::MusicPlayer(const std::string& name) : soundStatus{Ready, Ready, Ready, Ready, Ready, Ready, Ready, Ready}, SR{name} {

}
