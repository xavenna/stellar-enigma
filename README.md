# stellar-enigma
A Top-Down, 2-D game engine

stellar-enigma is a Top-Down, 2-D engine, inspired by games like the original Legend Of
Zelda. It is currently a Work in Progress


## Instructions:
To use stellar, create a main.cpp file based on the template provided. Insert any
custom initialization behaviors in the marked space.

To add custom objects, create a class that extends Object. Implement the virtual
functions in Object and create a constructor.
A guide explaining this process is forthcoming. If you add additional objects, the code
will need to be built from source, as engine files will need to be modified.
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
    * Contains level data, stored in .sel files (json from OGMO), and object data, 
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
Additionally, an copy of the documentation can be found [here](http://xavenna.net/projects/stellar-docs). This is likely out-of-date, though.


## Build:
Build dependencies:
SFML, json11. Links with install instructions are in the credits section.
On Linux, just install libsfml-dev and libjson11-1-dev from your package manager

stellar-enigma builds are tested on linux; Additional steps are likely required for
other operating systems.

### Build process:

stellar-enigma can be build using CMake:\
create a build directory build/.\
From there, run 'cmake ..'\
Finally, run the build command for whichever build system you used (eg. make)

The variables in CMakeCache can be edited for different behavior

If the dynamic build was chosen. libstellar.so must be placed in the same directory as
the main executable or installed systemwide.

To build the object editor, ncurses must be installed. This doesn't currently support
Windows. Run `make editor` to build the editor.


## Credits:
stellar-enigma was created by xavenna

The SFML Framework was used for Graphics and Audio\
These libraries can be found at https://sfml-dev.org/

The Json11 library was used for json parsing.\
This can be found at https://github.com/dropbox/json11/

OGMO editor is used for level creation.\
This tool can be found at https://ogmo-editor-3.github.io/

The object list editor was built using ncurses.

stellar-enigma is released under the MIT License. see LICENSE for details.
