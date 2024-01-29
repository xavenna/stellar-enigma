# stellar-enigma
A Top-Down, 2-D game engine

stellar-enigma is a Top-Down, 2-D engine, inspired by games like the original Legend Of
Zelda. It is currently a Work in Progress


## Instructions:
To use stellar, create a main.cpp file based on the template provided. Insert any
custom initialization behaviors in the marked space.

To add custom objects, create a class that extends Object. Implement the virtual
functions in Object, create a constructor, and add an entry to ObjContainer::storeObj.
A guide explaining this process is forthcoming. If you add additional objects, as engine
files will need to be modified, the code will need to be built from source, or the SDK
version of the libraries need to be obtained.

### Interface

Objects communicate with the engine through an Interface (See documentation for details).
To use this to communicate, open the object's virtuals by creating an empty Interface.
Use the interface api to request events:
Example: Display a message
    Interface inter;
    inter.addMessage(messageToDisplay);

The engine needs several files to be provided in the assets/ folder, such as the level
file, object list, and texturemap. These files are not included in this repository.
Details for this will be included in the tutorial, once it is ready. A level editor will 
be provided eventually.

## Assets:
The assets/ folder contains several required subfolders:

* texture/ - Contains textures and spritesheets
    * should contain image files
* texturemap/ - Contains texturemap files
    * Contains a texturemap file. See documentation for TextureCache for format
* level/ - contains level and object data
    * Contains level data, stored in .sel files (explor format), and object data, 
    stored in .sml files (stellar object format).
* audio/ - contains sound and music files, as well as the audio file list
    * Contains audiomap.txt - a list of all audio files, stored in the explor format.
    Also contains all sound files
* cutscene/ - contains the cutscene list and all cutscene files
    * default.csl is the cutscene list. Each line contains a cutscene to load. Cutscenes
    are stored in a .sec file. It is stored in the explor format
* menu/ - contains menu files
    * Menus are stored in menus.txt, in the stellar object format
* interface/ - contains interface files [Deprecated, will be removed in a later release]

Additionally, there should be a save folder in the project root directory.
This folder contains save files, stored in the json format, with the extension .sesave

## Documentation:
Documentaion can be generated using Doxygen. The documentation is currently incomplete.
Additionally, an copy of the documentation can be found [here](https://xavenna.github.io/projects/stellar-docs). This is likely out-of-date, though.


## Build:
Build dependencies:
SFML, json11. Links with install instructions are in the credits section.
On Linux, just install libsfml-dev and libjson11-1-dev from your package manager

stellar-enigma builds are tested on linux; The makefile may require tweaking to work on
other operating systems.

### Build process:

For a standard build, just run make.\
To use a dynamic library, add DYNAMIC=1 to the make command.\
To build in SDK Mode (see the SDK Mode section for more details), add SDK=1.\
To build in release (should be done by end users), add REL=1.\

If the dynamic build was chosen. libstellar.so must be placed in the same directory as
the main executable or installed systemwide.



## Credits:
stellar-enigma was created by xavenna

The SFML Framework was used for Graphics and Audio
These libraries can be found at https://sfml-dev.org/

The Json11 library was used for json parsing.
This can be found at https://github.com/dropbox/json11/

stellar-enigma was released under the MIT License. see LICENSE for details.
