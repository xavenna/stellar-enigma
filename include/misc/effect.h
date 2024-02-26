#ifndef EFFECT_H
#define EFFECT_H

//! A class for Status Effects
/*!
 *	A status effect lasts for a limited amount of time and changes some attribute of the
 *	mutable it applies to.
 */
class Effect {
public:
	enum Type {
    Speed,  //!< Alter the speed of the mutable (doesn't apply to objects)
    Health, //!< Change the player's health
    Null    //!< No effect on Player
  };
  void setScale(float sc); //!< Set the scale factor
  void setType(Effect::Type t); //!< Set the type
  float getScale() const; //!< Get the scale factor
  int getRemainingTime() const; //!< Gets remaining time until effect runs out
  int decrementTimer(); //!< Decrements timer, returns new value.
  Effect::Type getType() const; //!< Get the type
  Effect(Effect::Type, int); //!< Constructs effect with type and timer
private:
  float scale; //!< scale factor
  Effect::Type type;
  int timer; //!< Time remaining until the effect expires
};


#endif
