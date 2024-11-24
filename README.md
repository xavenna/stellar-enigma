# stellar-enigma
A Top-Down, 2-D game engine

stellar-enigma is a Top-Down, 2-D engine, inspired by games like the original Legend Of
Zelda. It is currently a Work In Progress


## Instructions:
To use stellar, create a main.cpp file based on the template provided. Insert any
custom initialization behaviors in the marked space.

To add custom objects, create a class that extends Object. Implement the virtual
functions in Object and create a constructor.
A guide explaining this process is forthcoming. If you add additional objects, the code
will need to be built from source, as engine files will need to be modified.

Note: See the build section for details

### Interface

Objects communicate with the engine through an Interface (See documentation for details).

To use this to communicate, begin the object's virtuals by creating an empty Interface.\
Use the interface api to request events:

Example: Display a message\
    Interface inter;
    inter.addMessage(messageToDisplay);

The engine needs several files to be provided in the assets/ folder, such as the level
file, object list, and texturemap. These files are not included in this repository.
Details for this will be included in the tutorial.


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
* camera/ - contains camera configuration files
    * Configs are stored in config.json, in JSON format.
* interface/ - contains interface files [Deprecated, will be removed in a later release]


Additionally, there should be a save folder in the project root directory.
This folder contains save files, stored in the json format, with the extension .sesave

## Documentation:
Documentaion can be generated using Doxygen. The documentation is currently incomplete.
Additionally, an copy of the documentation can be found [here](http://xavenna.net/projects/stellar-docs). This is likely out-of-date, though.


## Build
### Dependencies

stellar relies on SFML & json11. 
The object editor relies on gtk4.

On linux, these can be installed from your package manager. On debian, run
    sudo apt install libsfml-dev libjson11-1-dev libgtk-4-dev

On other systems, the process is more complicated; it is not covered in this document. 

stellar-enigma builds are tested on linux; Additional steps are likely required for
other operating systems.

### Build process:

stellar-enigma is built using CMake:\
create a build directory `build/`\
From there, run `cmake ..`\
Finally, run the build command for whichever build system you used (eg. make)

The variables in CMakeCache can be edited for different behavior

* set RAND\_TIME to ON to seed rng with time (fixes a bug with MinGW on Windows)
* set RAW\_IN to ON to use raw input, rather than OS Events
* set STATIC\_BUILD to ON to build libstellar as static, rather than dynamic

If the dynamic build was chosen. libstellar.so must be placed in the same directory as
the main executable or installed systemwide.

After adding new custom objects, repeat the build configuration step:\
    cmake ..

This adds any new files to the build list


To build the object editor, gtk4 must be installed.
Run `make editor` to build the editor.


## Credits:
stellar-enigma was created by xavenna

The SFML Framework was used for Graphics and Audio\
These libraries can be found at https://sfml-dev.org/

The Json11 library was used for json parsing.\
This can be found at https://github.com/dropbox/json11/

OGMO editor is used for level creation.\
This tool can be found at https://ogmo-editor-3.github.io/

The object list editor was built using GTK4.\
This library can be found at https://www.gtk.org

stellar-enigma is released under the MIT License. see LICENSE for details.
