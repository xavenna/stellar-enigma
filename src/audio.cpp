
#include "audio.h"

// Audio Module:
// This contains the audio player, as well as the functions used to control it

void SoundRegistry::registerSound(std::string name, sf::SoundBuffer buffer) {
  registry.insert(std::pair<std::string, sf::SoundBuffer>(name, buffer));
}
sf::SoundBuffer& SoundRegistry::getSound(std::string name) {
  return registry[name];
}

void MusicPlayer::registerSound(std::string name, sf::SoundBuffer buffer) {
  SR.registerSound(name, buffer);
}
sf::SoundBuffer& MusicPlayer::getSound(std::string name) {
  return SR.getSound(name);
}

bool MusicPlayer::findOpenSlot(int& n) {
  for(int i=0;i<8;i++) {
    if(soundStatus[i] == 0) {
      n = i;
      return true;
    }
  }
  return false;
}
void MusicPlayer::queueSound(std::string fn) {
  soundQueue.push_back(fn);
}
std::string MusicPlayer::dequeueSound() {
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
    soundPlayer[slot].setBuffer(SR.getSound(name));
    soundStatus[slot] = Assigned;
  }while(!open);
}
void MusicPlayer::manageSounds() {
  for(int i=0;i<8;i++) {
    if(soundStatus[i] == Playing) {
      if(soundPlayer[i].getStatus() == sf::SoundSource::Stopped) {
	//free up slot
	soundStatus[i] = Ready;
      }
    }
    else if(soundStatus[i] == ReservedPlaying) {
      if(soundPlayer[i].getStatus() == sf::SoundSource::Stopped) {
	//free up slot
	soundStatus[i] = Reserved;
      }
    }
    else if(soundStatus[i] == Assigned) {
      soundPlayer[i].play();
      if(!(soundPlayer[i].getStatus() == sf::SoundSource::Playing)) {
	soundStatus[i] = Ready;
      }
      else {
	soundStatus[i] = Playing;
      }
    }
    else if(soundStatus[i] == ReservedAssigned) {
      soundPlayer[i].play();
      soundStatus[i] = ReservedPlaying;
    }
  }
}
MusicPlayer::MusicPlayer() : soundStatus{Ready, Ready, Ready, Ready, Ready, Ready, Ready, Ready} {

}
