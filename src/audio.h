#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include <vector>
#include <array>
#include <deque>
#include <string>
#include <map>

//Documented?

//! An enum for status of a sound slot
enum Status {
  Ready=0,  //!< The slot is ready for a new sound to be assigned
  Playing,  //!< The slot is currently playing a sound
  Assigned,  //!< The slot has been assigned a sound, but has not played it yet
  Reserved,  //!< The slot is reserved and not playing
  ReservedPlaying,  //!< The slot is reserved and playing
  ReservedAssigned  //!< The slot is reserved and assigned, but not playing
};

//! A class that handles a registry of sf::SoundBuffers to names
/*!
 *  MusicPlayer uses SoundRegistry to handle sounds, so a sound's name can
 *  be used to play it
 */
class SoundRegistry {
private:
  std::map<std::string, sf::SoundBuffer> registry; //!< The internal map of names and sf::SoundBuffers
public:
  //! Get the number of sounds in registry
  int getNumSounds();
  //! Adds a pair of a string and a soundbuffer to the registry
  void registerSound(const std::string&, const sf::SoundBuffer&);
  //! Get a reference to the sound corresponding to the name supplied
  /*!
   *  If the sound with the requested name is found in the registry, a
   *  reference to the SoundBuffer is returned.
   *  Otherwise, an out_of_range exception is thrown
   */
  sf::SoundBuffer& getSound(std::string name);
};

//! The class that handles sound playing
/*!
 *  Eventually, it will handle music as well, but this capability doesn't exist
 *  yet.
 */
class MusicPlayer {
private:
  std::array<sf::Sound, 8> soundPlayer; //!< An array of Sound Players
  std::array<Status, 8> soundStatus; //!< The status of each sound player
  std::deque<std::string> soundQueue; //!< The queue of sounds to be played
  SoundRegistry SR; //!< An internal registry of soundBuffers and names

  sf::Music music; //!< Music player
public:
  //! Attempts to assign queued sounds to sound Players
  /*!
   *  Begins by checking if any sounds are queued. If not, then it exits
   *  Next, it checks if any slots are open. If so, it unqueues a sound
   *  and assigns it to an open slot. This proccess repeats until there
   *  are either no more sounds or no more open slots.
   */
  void assignSounds();
  //! Adds a sound to the queue
  /*!
   *  This adds the name of a sound to the queue. If this name isn't found in
   *  the registry, then nothing will play, and the slot will be immediately
   *  marked as ready
   */
  void queueSound(const std::string& soundName);
  //! Gets the name of a sound that has been queued
  /*!
   *  If no sounds are queued when this method is called, the program will
   *  crash. This is a problem, but I haven't got around to fixing it yet.
   */
  std::string dequeueSound();
  //! finds the number of an open slot, returns false if no open slot
  /*!
   *  If an open slot was found, slotNum is set to the number of the open
   *  slot, and the function returns true. Otherwise, it returns false and
   *  slotNum is equal to what it was before the function was called.
   */ 
  bool findOpenSlot(int& slotNum);
  //! Manages the sounds
  /*!
   *  First, checks the status of each sound player. If any player is done
   *  playing, then mark it as ready. If any is assigned, then then start
   *  playing it.
   */
  void manageSounds();
  //! Adds a sound to the Sound Registry
  void registerSound(const std::string& name, sf::SoundBuffer soundBuf);
  //! Chooses a new music file
  void playMusic(const std::string& filename);
  //! Gets a sound buffer from a sound
  /*!
   *  Returns a reference to the soundBuffer associated with the supplied file
   *  name. If the name isn't found, throws std::out_of_range.
   */
  sf::SoundBuffer& getSound(std::string name);
  //! constructs the Music player
  MusicPlayer();
};


#endif
