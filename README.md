# stellar-enigma
A game engine that uses the SFML framework.

stellar-enigma is a Top-Down, 2-D engine, inspired by games like the original Legend Of
Zelda. It is currently a Work in Progress

## Features:
stellar-enigma has many different components that provide functionality:
* Level - holds the map, objects, and manages collision
* CutscenePlayer - handles playing of cutscenes
* ModeSwitcher - handles input and switching modes, as well as triggering program exit
* InterfaceManager - manages the overall user interface, including border and panel
* MusicPlayer - manages playing of sounds and music
* Message - manages text being displayed to user


## Instructions:
To use stellar, create a main.cpp file based on the template provided. Insert any
custom initialization behaviors in the marked space. A level editor will be created
eventually.

To add custom objects, create a class that extends Object. Implement the virtual
functions in Object, create a constructor, and add an entry to ObjContainer::storeObj.
A guide explaining this process is forthcoming.

The engine needs several files to be provided in the assets/ folder, such as the level file, object list, and texturemap. These files are not included in this repository. Details for this will be included in the tutorial, once it is ready.

## Documentation:
Documentaion can be generated using Doxygen. The documentation is currently incomplete.
Additionally, an copy of the documentation can be found [here](https://xavenna.github.io/projects/stellar-docs). This is likely out-of-date, though.


## Build:
Build dependencies:
SFML, json11. Links with install instructions are in the credits section.
On Linux, just install libsfml-dev and libjson11-1-dev from your package manager

stellar-enigma builds are tested on linux; The makefile may require tweaking to work on
other operating systems. An option to build stellar as a static/dynamic library will be
available eventually.

## Credits:
stellar-enigma was created by xavenna

The SFML Framework was used for Graphics and Audio
These libraries can be found at https://sfml-dev.org/

The Json11 library was used for json parsing.
This can be found at https://github.com/dropbox/json11/
