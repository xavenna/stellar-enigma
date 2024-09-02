#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <array>
#include <vector>
#include <SFML/Graphics.hpp>
#include "stellar-enigma.hpp"

#include "editor/file-io.h"
#include "editor/editor.h"


/* commands:
 *
 * l ist: list all objects
 * s elect: Select an object to edit
 * r emove: delete selected object
 * a dd: Add an object
 * e dit: Edit properties of selected object
 * o pen: Load objects from a file
 * c lose
 * w rite
 * v erify: Check if mutable list is valid

 */
int main(int argc, char** argv) {
  //initialize things

  std::string activeFile;

  std::vector<ObjectBase> objs;


  std::string titleBar = "stellar-enigma obj editor: v-pre"; //displayed at the top of the screen
  std::string menuBar = "Current File: N/A; Current Object: none"; //bottom of screen
                                              //
  std::string buffer; //command buffer
  std::string statusBar = "Status: Ready";

  int currentObj = -1;

  Mode mode = Normal;

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);

  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_RED,     COLOR_BLACK);
    init_pair(2, COLOR_GREEN,   COLOR_BLACK);
    init_pair(3, COLOR_YELLOW,  COLOR_BLACK);
    init_pair(4, COLOR_BLUE,    COLOR_BLACK);
    init_pair(5, COLOR_CYAN,    COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(7, COLOR_BLACK,   COLOR_WHITE);
  }

  sf::Vector2i size;
  getmaxyx(stdscr, size.y, size.x);

  //make general window
  WINDOW* mainscr = newwin(size.y - 5, size.x-2, 2, 1);

  //begin editor loop
  mvwprintw(mainscr, 2, 2, "Welcome to the stellar-enigma object editor.");
  mvwprintw(mainscr, 3, 2, "Written by xavenna.");
  mvwprintw(mainscr, 4, 2, "Press q to quit, press h for help");
  refresh();
  wrefresh(mainscr);
  while(true) {
    mvprintw(0, 1, titleBar.c_str());
    attron(COLOR_PAIR(7));
    move(1,0);
    for(int i=0;i<size.x;i++) {
      addch('=');
    }
    attroff(COLOR_PAIR(7));
    move(size.y-3, 0);
    clrtoeol();
    mvprintw(size.y-3, 1, statusBar.c_str());
    
    move(size.y-2, 0);
    clrtoeol();
    mvprintw(size.y-2, 1, buffer.c_str());

    //attron(COLOR_PAIR(7));
    addch('_');
    //attroff(COLOR_PAIR(7));

    move(size.y-1, 0);
    clrtoeol();
    mvprintw(size.y-1, 1, menuBar.c_str());
    //in main loop: 
    int ch = getch();
    if(mode == Normal) {
      if(ch >= ' ' && ch <= '~') {
        buffer += ch;
      }
      else if(ch == KEY_BACKSPACE) {
        if(buffer.size() > 0) {
          buffer.erase(buffer.end()-1);
        }
      }
      else if(ch == '\n') {
        //attempt to execute command
        if(!ed::isValidCommand(buffer)) {
          //error message
          statusBar = "Error: Invalid Command\n";
          buffer.clear();
        }
        else {
          if(buffer == "quit") {
            break;
          }
          statusBar.clear();
          ed::executeCommand(buffer, menuBar, statusBar, objs, activeFile, mainscr, currentObj);
          //execute command
          buffer.clear();
        }
      }
    }

    menuBar = "Current File: ";
    if(activeFile.empty()) {
      menuBar += "N/A";
    }
    else {
      menuBar += activeFile;
    }
    menuBar += "; Current Object: ";
    if(currentObj == -1) {
      menuBar += "none";
    }
    else {
      menuBar += std::to_string(currentObj);
    }

    wrefresh(mainscr);
    refresh();
  }
  endwin();
}


