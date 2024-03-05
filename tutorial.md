# Stellar Enigma Tutorial

## Getting Started:
For a basic project, either obtain a build of the library for your system, or download
the source and build it yourself.

Open src/main-template.cpp, and add any custom initialization behavior in the marked
spot.

Now, follow the build instructions to build the executable.

## Asset Guide:

Before the engine can be used, the assets need to be set up. Create the folder structure
described in the readme.

Here's a description of how to initialize each folder.

* Audio
  * audiomap.txt
  * Any audio files for the program to use.
* Cutscene
  * cutlist.txt
  * all cutscene files (.sec extension)

* Interface

* Level
  * All level files
  * All Object list files

* Menu
  * menus.txt

* Texture
  * All image files for the program to use

* Texturemap
  * default.tm



## File Formats:

### Stellar Object Format
The stellar object format stores data in key-value pairs. key is a string, value can be
an int, string, or comma-delimited list of integers.\
Example pairs:\
* {oa:2,1,3,4} - field "oa" contains a comma-delimited list of integers
* {txt:"hello"} - field "txt" contains the string "hello"
* {p:3} - field "p" contains the number 3

Each line corresponds to an entry, and all attributes for that entry are concatenated.

### Explor Format
The explor format is a legacy format from stellar-enigma's predecessor, explor-v2. Data
is stored in a backtick (`) delimited list with no field names.\
A backtick is not required after the final element.\
Lines prefixed with '#' are ignored.


### JSON
Some files are stored in JSON, in order to be more compatible with other software


## Files

### audiomap.txt
Audiomap.txt stores cutscene data in the explor format. Each line corresponds to a sound.
Each line contains the sound's internal name and the filename. Files are looked for in
assets/audio/\
Example entry:\
    coin`coin.wav\
This loads coin.wav and assigns it the internal name of 'coin'.


### cutlist.txt
Cutlist.txt contains a list of all cutscenes to load. Each cutscene is on its own line.

### icons.txt
Uses the explor format


### levels (.sel files)
Levels are now stored in json, and produced by OGMO editor

The level consists of Five (for now) layers:
* "Base Layer" : The tile layer
* "[Direction] Solidity" : Direction replaced with Top, Bottom, Left, or Right
  * A grid layer, whether the tile is solid on specified side


### object lists (.sml files)
A .sml file contains all preloaded objects, and uses the stellar object format. Each line
contains the attributes of one object:
 * oa - object arguments. Up to 8 ints. Object-specific arguments. See specific object
 for more details
 * sw - switches. Up to 8 ints. switch assignments.
 * p - position. Two ints. X and Y position of object
 * s - size. Two ints. X and Y size of object's hitbox
 * li - link id. Int. Used to specify a specific object, such as in cutscenes.
 * pi - Parent id. Int. Link ID of object's parent.
 * ti - Texture id. Int. Used by objects to specify which texture to use.
 * c - Class. String - Which object class to use.
 * te - Text. String - Text argument for object.

### menus.txt
menus.txt describes the menus used within the game. It uses the stellar object format.\
Each entry describes a single menu.

### default.tm
default.tm is the texturemap. This specifies each texture used, and assigns it an
internal name. Uses the explor format.\
Each entry contains the internal name, image to source from, and the size and offset of 
the subrectangle to use

It follows the form:\
string-name`source-file`subrect-width`subrect-height`subrect-x-offset`subrect-y-offset

