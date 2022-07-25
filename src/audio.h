#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include <vector>
#include <array>
#include <deque>
#include <string>
#include <map>

enum Status {
  Ready=0, Playing, Assigned, Reserved, ReservedPlaying, ReservedAssigned
};

class SoundRegistry {
public:
  void registerSound(std::string, sf::SoundBuffer);
  sf::SoundBuffer& getSound(std::string);
private:
  std::map<std::string, sf::SoundBuffer> registry;
};

class MusicPlayer {
public:
  void assignSounds();
  void queueSound(std::string);
  std::string dequeueSound();
  bool findOpenSlot(int&);
  void manageSounds();
  void registerSound(std::string, sf::SoundBuffer);
  sf::SoundBuffer& getSound(std::string);
  MusicPlayer();
  sf::Music* music;
private:
  std::array<sf::Sound, 8> soundPlayer;
  std::array<Status, 8> soundStatus;
  std::deque<std::string> soundQueue;
  SoundRegistry SR;
};


#endif
