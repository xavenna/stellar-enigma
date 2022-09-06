
#include "audio.h"
#include <iostream>

// Audio Module:
// This contains the audio player, as well as the functions used to control it

void SoundRegistry::registerSound(const std::string& name, const sf::SoundBuffer& soundBuf) {
  registry.insert(std::pair<std::string, sf::SoundBuffer>(name, soundBuf));
}


sf::SoundBuffer& SoundRegistry::getSound(std::string name) {
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
  for(int i=0;i<8;i++) {
    if(soundStatus[i] == 0) {
      slotNum = i;
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

    std::string name = dequeueSound();
    if(name == "")
      continue;
    try {
      soundPlayer[slot].setBuffer(SR.getSound(name));
      soundStatus[slot] = Assigned;
    }
    catch (...) {
      continue;
    }

  }while(!open);
}
void MusicPlayer::manageSounds() {
  for(int i=0;i<8;i++) {
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
MusicPlayer::MusicPlayer() : soundStatus{Ready, Ready, Ready, Ready, Ready, Ready, Ready, Ready} {

}
