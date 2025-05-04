#include "editor/editor.h"

const std::string cmdlist = "This editor is used for manipulating stellar-enigma's object lists. The commands that can be used are shown below:\n list: lists all objects. No args.\n help: Displays this menu. No args\n remove: Remove selected object from list\n quit: close editor without saving. No args\n verify: Checks if mutable list is valid. No args.\n select: <int>. Selects specified object.\n add: <string>. Adds object of specified type to list. If invalid type, doesn't add.\n set: <arg1: string> <arg2>. Sets property arg1 to value arg2, if possible.\n load: <string>. Loads all objects from specified file. Searches ./assets/level/.\n write: <string>. Writes list to specified file. Uses ./assets/level as base.\n ";
const std::string fieldNames = "Editor field shorthand names:\nType (string): c\nPos (float, float): (px, py)\nSize (float, float): (sx, sy)\nLink ID (int): li\nParent ID (int): pi\nTexture ID (int): ti\nArguments (int[8]): a0, a1,...,a7\nSwitches (int[8]): sw0 - Appear, sw1 - Disappear, sw2 - A, sw3 - B, sw4 - C, sw5 - D, sw6 - Stat, sw7 - Remove\nText (string): te\n";

/* commands:
 *
 * list: list all objects
 * help: display a help menu
 * remove: delete selected object
 * close: close mutable list without saving
 * verify: Check if mutable list is valid
 * select: Select an object to edit
 *   select <obj-num>
 * add: Add an object
 * Set: set property of selected object
 * load: Load objects from a file
 * write: write mutable list to a file

 */

ObjectBase::ObjectBase() {
  for(auto& x : args) {
    x = -1;
  }
  for(auto& x : switches) {
    x = -1;
  }

}

namespace ed {
  bool isValidCommand(const std::string& buffer) {
    //no-argument commands
    if(buffer == "help" || buffer == "remove" || buffer == "quit" || buffer == "list" || buffer == "deselect" || buffer == "verify") {
      return true;
    }
    else if(buffer.substr(0,3) == "add" || buffer.substr(0,6) == "select" || buffer.substr(0,4) == "load" || buffer.substr(0,3) == "set" || buffer.substr(0,5) == "write") {
      return true;
    }
    return false;
  }

  std::string objToStr(ObjectBase& o) {
    std::string out;
    out = "Type: " + o.type + ", Pos: " + std::to_string(o.pos.x) + ", " +
      std::to_string(o.pos.y);
    return out;
  }

  std::string objToStrFull(ObjectBase& o) {
    std::string out;
    out = "Type: " + o.type + "; Pos: (" + std::to_string(o.pos.x) + ", " +
      std::to_string(o.pos.y) + "); Link ID: " + std::to_string(o.link_id) + 
      "; Parent ID: " + std::to_string(o.parent_id) + "; Texture ID: " + 
      std::to_string(o.texture_id) + "; Args: {";
    for(int i=0;i<8;i++) {
      out += std::to_string(o.args[i]);
      if(i == 7) {
        break;
      }
      out += ", ";
    }
    out += "}; Switches: {";
    for(int i=0;i<8;i++) {
      out += std::to_string(o.switches[i]);
      if(i == 7) {
        break;
      }
      out += ", ";
    }
    out += "}; Text arg: \"" + o.text + "\"";
    return out;

  }


  bool executeCommand(const std::string& com, std::string& menu, std::string& status, std::vector<ObjectBase>& objs, std::string& file, WINDOW* scr, int& currentObj) {
    //get params
    sf::Vector2i size;
    getmaxyx(stdscr, size.y, size.x);
    //execute command
    if(com == "help") {
      mvwprintw(scr, 1, 1, cmdlist.c_str());
    }
    else if(com == "list") {
      werase(scr);
      if(objs.size() == 0) {
        mvwprintw(scr, 2, 1, "No objects are loaded");
      }
      else {
        for(unsigned i=0;i<objs.size();i++) {
          mvwprintw(scr, 1+i, 1, (std::to_string(i) + ": ").c_str());
          wprintw(scr, ed::objToStr(objs[i]).c_str()); //display obj as string
        }
      }
      status = std::to_string(objs.size())+" objects listed";
      return true;
    }
    else if(com == "deselect") {
      status = "Object deselected";
      currentObj = -1;
      return true;
    }
    else if(com == "verify") {

      if(!verifyObjList(objs, status, scr)) {
        //verifyObjList sets status
        return false;
      }
      status = "Object list successfully verified";
      return true;
    }
    else if(com.substr(0, 3) == "add") {
      //add an object of specified type
      if(com.size() <= 4) {
        //no type specified
        status = "No type specified\n";
        return false;
      }
      if(!isValidObjType(com.substr(4))) {
        status = "Error: invalid object type";
        return false;
      }
      objs.push_back(ObjectBase());
      objs.back().type = com.substr(4);
      status = "Object "+objs.back().type+" added";
      return true;
    }
    else if(com.substr(0, 6) == "select") {
      //select specified object
      if(com.size() <= 7) {
        //no obj specified
        status = "The 'select' command requires an integral argument";
        return false;
      }
      else if(!isNum(com.substr(7))) {
        //arg isn't a number
        status = "Argument to 'select' must be a number";
        return false;
      }
      if(std::stoi(com.substr(7)) >= objs.size()) {
        //out of range selection
        status = "Out of range object selection.";
        return false;
      }
      currentObj = std::stoi(com.substr(7));
      status = "Object "+com.substr(7)+" selected";

      
      werase(scr);
      mvwprintw(scr, 1, 1, "Current Object:\n");
      wprintw(scr, ed::objToStrFull(objs[currentObj]).c_str());
      wprintw(scr, "\n\n");
      wprintw(scr, fieldNames.c_str());
      return true;
    }
    else if(com.substr(0, 4) == "load") {
      //load objects from specified file
      if(com.size() <= 5) {
        status = "Error: No file specified.";
        return false;
      }
      std::string fname = com.substr(5);
      fname = "assets/level/" + fname;
      if(fs::exists(fname)) {
        fs::copy_file(fname, fname + ".bak", fs::copy_options::overwrite_existing);
      }
      std::ifstream get(fname);
      if(!get.is_open()) {
        status = "Error: file could not be opened for reading.";
        return false;
      }
      get.close();
      //load file...
      if(loadMutableList(objs, fname) != 0) {
        status = "Error: file load unsuccessful.";
        return false;
      }
      file = com.substr(5);
      //DISPLAY OBJECTS
      for(unsigned i=0;i<objs.size();i++) {
        mvwprintw(scr, 1+i, 1, (std::to_string(i) + ": ").c_str());
        wprintw(scr, ed::objToStr(objs[i]).c_str()); //display obj as string
      }

      return true;
    }
    else if(com.substr(0, 5) == "write") {
      //dump files to specified file

      std::string fname = "assets/level/";
      if(com.size() <= 6) {
        //write to selected file
        status = "No file selected, writing to open file";
        fname += file;
      }
      else {
        fname += com.substr(6);
      }


      if(!ed::writeListToFile(objs, fname, status)) {
        //write failure
        return false;
      }
    }
    else if(com.substr(0, 3) == "set") {
      //set specified attribute of selected object
      //break into three pieces
      if(com.size() <= 4) {
        status = "Error: 'set' command requires two arguments.";
        return false;
      }
      std::string args = com.substr(4);
      //should be in the form of "<arg1> <value>", where value can be a string or int
      if(args.find(' ') == std::string::npos) {
        status = "Error: 'set' command requires two arguments.";
        return false;
      }
      if(currentObj == -1) {
        status = "Error: No object selected.";
        return false;
      }
      std::string arg1 = args.substr(0, args.find(' '));
      std::string arg2 = args.substr(args.find(' ') + 1);
      status = "Set " + arg1 + " to '" + arg2 + "'";

      if(!setAttr(objs[currentObj], arg1, arg2, status)) {
        //status is set by setAttr
        return false;
      }
      //refresh window listing
      werase(scr);
      mvwprintw(scr, 1, 1, "Current Object:\n");
      wprintw(scr, ed::objToStrFull(objs[currentObj]).c_str());
      wprintw(scr, "\n\n");
      wprintw(scr, fieldNames.c_str());
      return true;
    }

    return false;
  }

  //retooled to work with the updated system
  bool setAttr(ObjectBase& obj, const std::string& arg1, const std::string& arg2, std::string& status) {
    bool numArg = isNum(arg2);
    if(arg1 == "xpos") {
      if(numArg) {
        obj.pos.x = std::stof(arg2);
      }
      else {
        status = "Error: This attribute requires a numeric argument";
        return false;
      }

    }
    else if(arg1 == "ypos") {
      if(numArg) {
        obj.pos.y = std::stof(arg2);
      }
      else {
        status = "Error: This attribute requires a numeric argument";
        return false;
      }

    }
    else if(arg1 == "x_scale") {
      if(numArg) {
        obj.scale.x = std::stof(arg2);
      }
      else {
        status = "Error: This attribute requires a numeric argument";
        return false;
      }

    }
    else if(arg1 == "y_scale") {
      if(numArg) {
        obj.scale.y = std::stof(arg2);
      }
      else {
        status = "Error: This attribute requires a numeric argument";
        return false;
      }

    }
    else if(arg1 == "Link ID") {
      if(numArg) {
        obj.link_id = std::stoi(arg2);
      }
      else {
        status = "Error: This attribute requires a numeric argument";
        return false;
      }

    }
    else if(arg1 == "Parent ID") {
      if(numArg) {
        obj.parent_id = std::stoi(arg2);
      }
      else {
        status = "Error: This attribute requires a numeric argument";
        return false;
      }

    }
    else if(arg1 == "Texture ID") {
      if(numArg) {
        obj.texture_id = std::stoi(arg2);
      }
      else {
        status = "Error: This attribute requires a numeric argument";
        return false;
      }

    }
    else if(arg1 == "Text Arg") {
      if(arg2.size() <= 0) {
        obj.text = "";
      }
      status = arg2;
      obj.text = arg2;
      return true;
    }
    else if(arg1.size() > 4 && arg1.substr(0, 4) == "arg_") {
      if(arg1.size() != 5 || arg1[4] < '0' || arg1[4] > '7' || !numArg) {
        status = "Error: Invalid Arguments";
        return false;
      }
      obj.args[ (arg1[4] - '0') ] = std::stoi(arg2);
      return true;
    }
    else if(arg1.size() > 3 && arg1.substr(0,3) == "SW_") {
      std::vector<std::string> sw = {
        "Appear", "Disappear", "A", "B", "C", "D", "Stat", "Remove"
      };

      auto x = std::find(sw.begin(), sw.end(), arg1.substr(3));
      if(x == sw.end()) {
        status = "Error: Invalid switch name";
        return false;
      }
      size_t index = x - sw.begin();
      
      if(!numArg) {
        status = "Error: Invalid Arguments";
        return false;
      }
      obj.switches[index] = std::stoi(arg2);
      return true;
    }
    else {
      status = "Error: Invalid attribute";
      return false;
    }
    return true;
  }

  bool getAttr(ObjectBase& obj, const std::string& prop, std::string& val, std::string& status) {
    if(prop == "xpos") {
      val = std::to_string(obj.pos.x);
    }
    else if(prop == "ypos") {
      val = std::to_string(obj.pos.y);
    }
    else if(prop == "x_scale") {
      val = std::to_string(obj.scale.x);
    }
    else if(prop == "y_scale") {
      val = std::to_string(obj.scale.y);
    }
    else if(prop == "Link ID") {
      val = std::to_string(obj.link_id);
    }
    else if(prop == "Parent ID") {
      val = std::to_string(obj.parent_id);
    }
    else if(prop == "Texture ID") {
      val = std::to_string(obj.texture_id);
    }
    else if(prop == "Text Arg") {
      val = obj.text;
    }
    else if(prop.size() > 4 && prop.substr(0, 4) == "arg_") {
      if(prop.size() != 5 || prop[4] < '0' || prop[4] > '7') {
        status = "Error: Invalid Arguments";
        return false;
      }
      val = std::to_string(obj.args[ (prop[4] - '0') ]);
    }
    else if(prop.size() > 3 && prop.substr(0,3) == "SW_") {
      std::vector<std::string> sw = {
        "Appear", "Disappear", "A", "B", "C", "D", "Stat", "Remove"
      };

      auto x = std::find(sw.begin(), sw.end(), prop.substr(3));
      if(x == sw.end()) {
        status = "Error: Invalid switch name";
        return false;
      }
      size_t index = x - sw.begin();
      
      val = std::to_string(obj.switches[index]);
    }
    else {
      status = "Error: Invalid attribute";
      return false;
    }
    return true;
  }
  bool isValidObjType(const std::string& name) {
    std::ifstream load("obj-names.txt");
    std::string line;
    static std::vector<std::string> types;
    if(types.empty()) {
      if(!load.is_open()) {

        return false;
      }
      while(load.peek() != EOF) {
        std::getline(load, line);
        types.push_back(line);
      }
    }
    for(auto x : types) {
      if(name == x) {
        return true;
      }
    }
    return false;
  }

  bool verifyObjList(std::vector<ObjectBase>& objs, std::string& status, WINDOW* scr) {
    //load types list
    static std::vector<std::string> types;
    if(types.empty()) {
      std::ifstream load("obj-names.txt");
      std::string line;
      if(!load.is_open()) {
        status = "Error: could not load object type list";
        return false;
      }
      while(load.peek() != EOF) {
        std::getline(load, line);
        types.push_back(line);
      }
      load.close();
    }

    werase(scr);

    bool success=true;

    ObjContainer oc;
    std::string vstat;
    unsigned line=0;
    for(unsigned i=0;i<objs.size();i++) {
      ObjectBase& x = objs[i];
      //construct an object from the ObjectBase, then have the ObjContainer initialize it
      Object ob;
      ob.setLinkID(x.link_id);
      ob.setTextureID(x.texture_id);
      ob.setParentID(x.parent_id);
      ob.setPos(x.pos);
      ob.setText(x.text);
      ob.setArgs(x.args);
      ob.setSwitches(x.switches);

      if(std::find(types.begin(), types.end(), x.type) == types.end()) {
        status = "Verification failure";
        return false;
      }

      if(!oc.storeObj(ob, x.type, vstat)) {
        mvwprintw(scr, line, 0, "Verification failed on Object ");
        wprintw(scr, std::to_string(i).c_str());
        wprintw(scr, ". Type: ");
        wprintw(scr, x.type.c_str());

        success = false;
        line+=1;
      }

    }
    if(success) {
      status = "Verification Successful!";
    }
    else {
      status = "Verification failure";
    }
    return success;
  }
}
