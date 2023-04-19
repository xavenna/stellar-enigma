# stellar-enigma
A game engine that uses the SFML framework.

stellar-enigma is a Top-Down, 2-D engine, inspired by games like the original Legend Of
Zelda. It is currently a WIP

## Features:
stellar-enigma has many different components that provide functionality:
* Level - holds the map, objects, and manages collision
* CutscenePlayer - handles playing of cutscenes
* ModeSwitcher - handles input and switching modes, as well as triggering program exit
* InterfaceManager - manages the overall user interface, including border and panel
* MusicPlayer - manages playing of sounds and music
* Message - manages text being displayed to user


## Instructions:
To add custom behavior to the engine, modify the functions found in custom.cpp
These functions allow custom code to mostly be contained to one file



## Documentation:
Documentaion can be generated using Doxygen. Currently, only parts of the code
are documented

## Build:
stellar-enigma builds are tested on linux; The makefile may require tweaking to work on
other operating systems.

## Credits:
stellar-enigma was created by xavenna

I used the SFML framework for graphics and audio.
These libraries can be found at https://sfml-dev.org/
