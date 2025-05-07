#include "editor/editor-gtk.h"
//new

static void quit_activated(GSimpleAction *action, GVariant *parameter, gpointer data) {
  ListData* ld = (ListData*) data;
  gtk_window_destroy(ld->win);
}

static void update_text_entry_cb(GtkEditable* self, gpointer user_data) {
  ListData* data = (ListData*) user_data;
  //setup textview
  GtkTextBuffer* tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->tv));

  //update the list's text
  if(*data->sel >= data->objs.size()) {
    gtk_text_buffer_insert_at_cursor(tbuf, "Invalid object selection\n", -1);
    return;
  }

  ObjectBase& selobj = (data->objs)[*data->sel];

  //determine which field changed:
  //or just update all fields

  GtkSelectionModel* mod = gtk_column_view_get_model(GTK_COLUMN_VIEW(data->cv));
  GListModel* list = gtk_no_selection_get_model(GTK_NO_SELECTION(mod));

  std::string status;
  guint len = g_list_model_get_n_items(list);
  for(guint i=0;i<len;i++) {
    PropItem* p = (PropItem*) g_list_model_get_item(G_LIST_MODEL(list), i);
    if(!ed::setAttr(selobj, p->prop, p->value, status)) {
      std::string msg = "Property: " + p->prop + "; Value: " + p->value + "\n";
      //gtk_text_buffer_insert_at_cursor(tbuf, "Error: could not set values: ", -1);
      gtk_text_buffer_insert_at_cursor(tbuf, msg.c_str(), msg.size());
    }


  }
  //iterate through the list
  gtk_widget_queue_draw(data->pre);
}

static void setup_cb(GtkSignalListItemFactory* self, GtkListItem* listitem, gpointer user_data) {
  ListData* data = (ListData*) user_data;
  GtkWidget *lb = gtk_label_new (NULL);
  gtk_list_item_set_child (listitem, lb);
  /* Because gtk_list_item_set_child sunk the floating reference of lb, releasing (unref) isn't necessary for lb. */
}

static void bind_cb(GtkSignalListItemFactory* self, GtkListItem* listitem, gpointer user_data) {
  ListData* data = (ListData*) user_data;
  GtkWidget *lb = gtk_list_item_get_child (listitem);
  /* Strobj is owned by the instance. Caller mustn't change or destroy it. */
  GtkStringObject *strobj = GTK_STRING_OBJECT(gtk_list_item_get_item(listitem));
  /* The string returned by gtk_string_object_get_string is owned by the instance. */
  gtk_label_set_text (GTK_LABEL (lb), gtk_string_object_get_string (strobj));
}

static void setup_field_cb(GtkSignalListItemFactory* self, GObject* listitem, gpointer user_data) {
  ListData* data = (ListData*) user_data;
  //setup label
  GtkWidget* lb = gtk_label_new(NULL);
  gtk_list_item_set_child(GTK_LIST_ITEM(listitem), lb);

}

static void bind_field_cb(GtkSignalListItemFactory* self, GObject* listitem, gpointer user_data) {
  ListData* data = (ListData*) user_data;
  //bind label
  GtkWidget* lb = gtk_list_item_get_child(GTK_LIST_ITEM(listitem));
  GObject* item = (GObject*) gtk_list_item_get_item(GTK_LIST_ITEM(listitem));

  gtk_label_set_text(GTK_LABEL(lb), PROP_ITEM(item)->prop.c_str());

}
static void setup_val_cb(GtkSignalListItemFactory* self, GObject* listitem, gpointer user_data) {
  ListData* data = (ListData*) user_data;
  //setup entry
  GtkWidget* en = gtk_entry_new();
  gtk_entry_set_has_frame(GTK_ENTRY(en), FALSE);
  gtk_list_item_set_child(GTK_LIST_ITEM(listitem), en);

  g_signal_connect(GTK_WIDGET(en), "activate", G_CALLBACK(update_text_entry_cb), data);
}

static void bind_val_cb(GtkSignalListItemFactory* self, GtkListItem* listitem, gpointer user_data) {
  ListData* data = (ListData*) user_data;
  GtkWidget* en = gtk_list_item_get_child(listitem);
  //bind entry

  // fix this for entry
  GtkEntryBuffer* buf = gtk_entry_get_buffer(GTK_ENTRY(en));
  PropItem* si = PROP_ITEM(gtk_list_item_get_item(listitem));
  GBinding* bind;

  gtk_entry_buffer_set_text(buf, si->value.c_str(), -1);

  bind = g_object_bind_property(buf, "text", si, "value", G_BINDING_BIDIRECTIONAL);
  g_object_set_data(G_OBJECT(listitem), "bind", NULL);
}

static void change_selected_obj_cb(GtkSelectionModel* sel, guint pos, guint len, gpointer user_data) {
  ListData* data = (ListData*) user_data;
  GtkTextBuffer* tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->tv));
  //update fields to match current object
  //get current object
  //
  //TODO: first, make sure all fields were updated...

  //update the list's text
  if(*data->sel >= data->objs.size()) {
    gtk_text_buffer_insert_at_cursor(tbuf, "Invalid object selection\n", -1);
    return;
  }

  GtkSelectionModel* mod = gtk_column_view_get_model(GTK_COLUMN_VIEW(data->cv));
  GListModel* list = gtk_no_selection_get_model(GTK_NO_SELECTION(mod));
  {
    ObjectBase& selobj = (data->objs)[*data->sel];

    //determine which field changed:
    //or just update all fields


    std::string status;
    guint l = g_list_model_get_n_items(list);
    for(guint i=0;i<l;i++) {
      PropItem* p = (PropItem*) g_list_model_get_item(G_LIST_MODEL(list), i);
      if(!ed::setAttr(selobj, p->prop, p->value, status)) {
        std::string msg = "Property: " + p->prop + "; Value: " + p->value + "\n";
        //gtk_text_buffer_insert_at_cursor(tbuf, "Error: could not set values: ", -1);
        status += '\n';
        gtk_text_buffer_insert_at_cursor(tbuf, status.c_str(), status.size());
      }


    }
  }

  //since the binding is setup, the PropItem should be up-to-date
  guint s = gtk_single_selection_get_selected(GTK_SINGLE_SELECTION(sel));
  ObjectBase& selobj = (data->objs)[s];

  *data->sel = s;

  guint le = g_list_model_get_n_items(list);//number of property columns

  std::string status;
  std::string val;
  for(guint i=0;i<le;i++) {
    PropItem* p = (PropItem*) g_list_model_get_item(G_LIST_MODEL(list), i);
    //go through, row by row, get the appropriate value, set it to the 

    ed::getAttr(selobj, p->prop, val, status);
    prop_item_set_value(p, val.c_str());
  }


  gtk_widget_queue_draw(data->pre);
}

static void add_obj_btn_cb(GtkEntry* btn, gpointer user_data) {
  ListData* data = (ListData*) user_data;
  //setup textview
  GtkTextBuffer* tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->tv));

  GtkEntryBuffer* buf = gtk_entry_get_buffer(GTK_ENTRY(data->ent));
  const char* name = gtk_entry_buffer_get_text(buf);

  std::string type{name};
  if(type.empty()) {
    gtk_text_buffer_insert_at_cursor(tbuf, "Cannot add object with null type\n", -1);
    return;
  }
  
  if(ed::isValidObjType(type)) {
    //create obj
    ObjectBase ob;
    ob.type = name;
    data->objs.push_back(ob);
    //write status
    std::string msg = "Added object of type '" + type + "'\n";
    gtk_text_buffer_insert_at_cursor(tbuf, msg.c_str(), msg.size());

    //add to listView
    gtk_string_list_append(data->list, gtk_entry_buffer_get_text(buf));
    gtk_entry_buffer_set_text(buf, "", -1);
  }
  else {
    std::string msg = "Did not add: '" + type + "' is not a valid type.\n";
    gtk_text_buffer_insert_at_cursor(tbuf, msg.c_str(), msg.size());
    gtk_entry_buffer_set_text(buf, "", -1);
  }
  gtk_widget_queue_draw(data->pre);
}

static void obj_info_entry_activate_cb(GtkEntry* ent, gpointer user_data) {
  ListData* data = (ListData*) user_data;
  //setup textviews
  GtkTextBuffer* tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->tv));
  GtkTextBuffer* obj_tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->obj_tv));

  GtkEntryBuffer* buf = gtk_entry_get_buffer(GTK_ENTRY(data->obj_ent));
  const char* rname = gtk_entry_buffer_get_text(buf);
  std::string name = rname;

  gtk_entry_buffer_set_text(buf, "", -1);
  gtk_text_buffer_set_text(obj_tbuf, "", 0);

  //now, fetch from db
  if(!data->db.contains(name)) {
    gtk_text_buffer_insert_at_cursor(obj_tbuf, "No database entry found...\n", -1);
    return;

  }

  std::string entry = data->db.get_full_entry(name);
  gtk_text_buffer_insert_at_cursor(obj_tbuf, entry.c_str(), entry.size());

}

static void obj_info_btn_cb(GtkButton* btn, gpointer user_data) {
  ListData* data = (ListData*) user_data;
  //setup textviews
  GtkTextBuffer* tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->tv));
  GtkTextBuffer* obj_tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->obj_tv));
  gtk_text_buffer_set_text(obj_tbuf, "", 0);

  GtkEntryBuffer* buf = gtk_entry_get_buffer(GTK_ENTRY(data->obj_ent));
  //get name of selected object
  if(data->objs.empty()) {
    gtk_text_buffer_insert_at_cursor(obj_tbuf, "No object selected", -1);
    return;
  }

  if(data->objs.size() <= *data->sel) {
    gtk_text_buffer_insert_at_cursor(obj_tbuf, "Error: out of bounds selection.\n", -1);
    return;

  }
  std::string name = data->objs[*(data->sel)].type;


  //now, fetch from db
  if(!data->db.contains(name)) {
    gtk_text_buffer_insert_at_cursor(obj_tbuf, "No database entry found...\n", -1);
    return;

  }

  std::string entry = data->db.get_full_entry(name);
  gtk_text_buffer_insert_at_cursor(obj_tbuf, entry.c_str(), entry.size());

}

static void sw_info_btn_cb(GtkEntry* ent, gpointer user_data) {
  std::array<std::string, 8> sw_names = {
    "Appear",
    "Disappear",
    "A",
    "B",
    "C",
    "D",
    "Stat",
    "Remove"
  };
  ListData* data = (ListData*) user_data;
  //setup textview
  GtkTextBuffer* tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->tv));
  GtkEntryBuffer* buf = gtk_entry_get_buffer(ent);

  const char* name = gtk_entry_buffer_get_text(buf);

  std::string sw{name};
  int s = -1;
  try {
    s = std::stoi(sw);
    if(s<0 || s > 255) {
      gtk_text_buffer_insert_at_cursor(tbuf, "Error: switch number out of range\n", -1);
      gtk_entry_buffer_set_text(buf, "", -1);
      return;
    }
  } catch(...) {
    gtk_text_buffer_insert_at_cursor(tbuf, "Error: input must be an integer\n", -1);
    gtk_entry_buffer_set_text(buf, "", -1);
    return;
  }
  std::string m = "Switch "+std::to_string(s)+":\n";
  gtk_text_buffer_insert_at_cursor(tbuf, m.c_str(), m.size());
  //go through all objects, and check for switch values
  for(unsigned i=0;i<data->objs.size();i++) {
    auto x = (data->objs)[i];
    for(int j=0;j<8;j++) {
      if(x.switches[j] == s) {
        //output a note
        std::string msg = "Object "+std::to_string(i)+"<type:"+x.type+"> - SW_"+sw_names[j]+"\n";
        gtk_text_buffer_insert_at_cursor(tbuf, msg.c_str(), msg.size());
      }

    }

  }
  gtk_text_buffer_insert_at_cursor(tbuf, "\n", -1);
  gtk_entry_buffer_set_text(buf, "", -1);

}

static void redraw_btn_cb(GtkButton* btn, gpointer user_data) {
  ListData* data = (ListData*) user_data;
  //setup textview
  GtkTextBuffer* tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->tv));

  gtk_widget_queue_draw(data->pre);
}

static void rem_obj_btn_cb(GtkButton* btn, gpointer user_data) {
  ListData* data = (ListData*) user_data;
  //setup textview
  GtkTextBuffer* tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->tv));

  //remove selected object
  GtkSelectionModel* mod = gtk_list_view_get_model(GTK_LIST_VIEW(data->lv));
  guint sel = gtk_single_selection_get_selected(GTK_SINGLE_SELECTION(mod));
  if(sel == GTK_INVALID_LIST_POSITION) {
    gtk_text_buffer_insert_at_cursor(tbuf, "Could not remove object, none selected\n", -1);
  }
  else {
    auto& vec = (data->objs);
    vec.erase(vec.begin() + sel);
    std::string type = vec[sel].type;
    type = "Removing object with type '"+type + "'.\n";
    gtk_text_buffer_insert_at_cursor(tbuf, type.c_str(), type.size());
    gtk_string_list_remove(data->list, sel);



  }
  gtk_widget_queue_draw(data->pre);
}

static void on_save_response(GtkDialog* dialog, int response, gpointer user_data) {
  ListData* data = (ListData*) user_data;
  GtkTextBuffer* tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->tv));

  if (response == GTK_RESPONSE_ACCEPT)
    {
      GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);

      g_autoptr(GFile) file = gtk_file_chooser_get_file (chooser);

      data->openFile = g_file_get_parse_name(file);

      std::string stat;
      ed::writeListToFile(data->objs, data->openFile, stat);
      gtk_text_buffer_insert_at_cursor(tbuf, stat.c_str(), stat.size());
    }
  gtk_window_destroy (GTK_WINDOW (dialog));
}

static void on_open_response(GtkDialog* dialog, int response, gpointer user_data) {
  ListData* data = (ListData*) user_data;
  GtkTextBuffer* tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->tv));

  if (response == GTK_RESPONSE_ACCEPT) {
      GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);

      GFile* file = gtk_file_chooser_get_file (chooser);
      read_file(data, file);
      g_object_unref(file);
    }
    gtk_window_destroy (GTK_WINDOW (dialog));


}

void read_file(ListData* data, GFile* file) {
  GtkTextBuffer* tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->tv));

  guint len = g_list_model_get_n_items(G_LIST_MODEL(data->list));

  //clear objlist
  data->objs.clear();

  //clear actual list
  for(guint i=0;i<len;i++) {
    gtk_string_list_remove(data->list, 0);
  }

  data->openFile = g_file_get_parse_name(file);
  //load from the file
  switch(ed::loadMutableList(data->objs, data->openFile)) {
  case 1:
    gtk_text_buffer_insert_at_cursor(tbuf, "Error: could not open file\n", -1);
    break;
  case 2:
    gtk_text_buffer_insert_at_cursor(tbuf, "Error: Malformed object in list\n", -1);
    break;
  default:
    gtk_text_buffer_insert_at_cursor(tbuf, "File opened successfully\n", -1);
    break;
  }
  //populate data->list with the 
  for(auto x : data->objs) {
    gtk_string_list_append(data->list, x.type.c_str());
  }

  //also, update the property pane's selected values

  if(data->objs.size() != 0) {
    ObjectBase& selobj = (data->objs)[0];

    *data->sel = 0;
    GtkSelectionModel* mod = gtk_column_view_get_model(GTK_COLUMN_VIEW(data->cv));
    GListModel* list = gtk_no_selection_get_model(GTK_NO_SELECTION(mod));

    guint le = g_list_model_get_n_items(list);//number of property columns

    std::string status;
    std::string val;
    for(guint i=0;i<le;i++) {
      PropItem* p = (PropItem*) g_list_model_get_item(G_LIST_MODEL(list), i);
      //go through, row by row, get the appropriate value, set it to the 

      ed::getAttr(selobj, p->prop, val, status);
      prop_item_set_value(p, val.c_str());
    }
  }
  gtk_widget_queue_draw(data->pre);


}

static void saveas_activated(GSimpleAction *act, GVariant *parameter, gpointer user_data){
  ListData* data = (ListData*) user_data;
  GtkTextBuffer* tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->tv));
  GtkWidget* dialog;
  GtkFileChooser* chooser;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;

  dialog = gtk_file_chooser_dialog_new ("Save File",
                                        GTK_WINDOW(data->win),
                                        action,
                                        ("_Cancel"),
                                        GTK_RESPONSE_CANCEL,
                                        ("_Save"),
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);

  gtk_window_present(GTK_WINDOW (dialog));

  g_signal_connect(dialog, "response",
                    G_CALLBACK(on_save_response),
                    data);
}

static void save_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data){
  ListData* data = (ListData*) user_data;

  GtkTextBuffer* tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->tv));
  //write to data->openFile;
  if(data->openFile.empty()) {
    //no file open, pull up dialog
    GtkWidget* dialog;
    GtkFileChooser* chooser;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;

    dialog = gtk_file_chooser_dialog_new ("Save File",
                                          GTK_WINDOW(data->win),
                                          action,
                                          ("_Cancel"),
                                          GTK_RESPONSE_CANCEL,
                                          ("_Save"),
                                          GTK_RESPONSE_ACCEPT,
                                          NULL);

    gtk_window_present(GTK_WINDOW (dialog));

    g_signal_connect(dialog, "response",
                      G_CALLBACK(on_save_response),
                      data);
  }
  else {
    //write to data->openFile;
    std::string stat;
    ed::writeListToFile(data->objs, data->openFile, stat);
    gtk_text_buffer_insert_at_cursor(tbuf, stat.c_str(), stat.size());
  }
}

static void open_activated(GSimpleAction *act, GVariant *parameter, gpointer user_data){
  ListData* data = (ListData*) user_data;
  GtkTextBuffer* tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->tv));

  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

  dialog = gtk_file_chooser_dialog_new ("Open File",
                                        data->win,
                                        action,
                                        ("_Cancel"),
                                        GTK_RESPONSE_CANCEL,
                                        ("_Open"),
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);

  gtk_window_present (GTK_WINDOW (dialog));

  g_signal_connect (dialog, "response",
                    G_CALLBACK (on_open_response),
                    data);

}
static void objdata_activated(GSimpleAction *act, GVariant *parameter, gpointer user_data){
  ListData* data = (ListData*) user_data;
  GtkTextBuffer* tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->tv));

  gtk_window_present(data->objdata);



}

static void verify_activated(GSimpleAction *act, GVariant *parameter, gpointer user_data){
  ListData* data = (ListData*) user_data;
  //setup textview
  GtkTextBuffer* tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->tv));
  //load types list
  static std::vector<std::string> types;
  if(types.empty()) {
    std::ifstream load("obj-names.txt");
    std::string line;
    if(!load.is_open()) {
      gtk_text_buffer_insert_at_cursor(tbuf, "Error: could not load object type list\n", -1);
      return;
    }
    while(load.peek() != EOF) {
      std::getline(load, line);
      types.push_back(line);
    }
    load.close();
  }


  bool success=true;

  std::set<int> link_ids;

  ObjContainer oc;
  std::string vstat;
  unsigned line=0;
  for(unsigned i=0;i<data->objs.size();i++) {
    ObjectBase& x = (data->objs)[i];

    if(x.link_id >= 0) { //check link_id if not null
      if(link_ids.count(x.link_id) != 0) {
        std::string mesg = "Verification failure: Repeated link_id " + std::to_string(x.link_id) + "\n";
        gtk_text_buffer_insert_at_cursor(tbuf, mesg.c_str(), mesg.size());
        success = false;
        line += 1;
      } else {
        link_ids.insert(x.link_id);
      }

    }


    //construct an object from the ObjectBase, then have the ObjContainer initialize it
    Object ob;
    ob.setLinkID(x.link_id);
    ob.setTextureID(x.texture_id);
    ob.setParentID(x.parent_id);
    ob.setPos(x.pos);
    ob.setScale(x.scale);
    ob.setText(x.text);
    ob.setArgs(x.args);
    ob.setSwitches(x.switches);

    if(std::find(types.begin(), types.end(), x.type) == types.end()) {
      gtk_text_buffer_insert_at_cursor(tbuf, "Verification failure\n", -1);
      return;
    }

    if(!oc.storeObj(ob, x.type, vstat)) {
      std::string mesg = "Verification failed on Object " + std::to_string(i)
        + ". Type: " + x.type + "\n";
      gtk_text_buffer_insert_at_cursor(tbuf, mesg.c_str(), mesg.size());

      success = false;
      line+=1;
    }

  }
  if(success) {
    gtk_text_buffer_insert_at_cursor(tbuf, "Verification Successful!\n", -1);
  }
  else {
    gtk_text_buffer_insert_at_cursor(tbuf, "Verification failure\n", -1);
  }
}


//eventually, this renders a preview of the objects
static void draw_function(GtkDrawingArea* area, cairo_t* cr, int width, int height, gpointer user_data) {
  ListData* data = (ListData*) user_data;
  GtkTextBuffer* tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->tv));
  //find extreme positions of objects

  if(data->objs.size() == 0) {
    //don't preview anything
    return;
  }
  float xmin=data->objs[0].pos.x;


  float xmax=data->objs[0].pos.x + size(data->objs[0], data->db).x;
  float ymin=data->objs[0].pos.y;
  float ymax=data->objs[0].pos.y + size(data->objs[0], data->db).y;

  for(auto x : data->objs) {
    if(x.pos.x < xmin) {
      xmin = x.pos.x;
    }
    if(x.pos.x + size(x, data->db).x > xmax) {
      xmax = x.pos.x+size(x, data->db).x;
    }
    if(x.pos.y < ymin) {
      ymin = x.pos.y;
    }
    if(x.pos.y + size(x, data->db).y > ymax) {
      ymax = x.pos.y+size(x, data->db).y;
    }
  }
  sf::FloatRect pane(xmin, ymin, xmax-xmin, ymax-ymin);

  //get widget size
  // 
  float w=static_cast<float>(gtk_widget_get_size(GTK_WIDGET(area), GTK_ORIENTATION_HORIZONTAL));
  float h=static_cast<float>(gtk_widget_get_size(GTK_WIDGET(area), GTK_ORIENTATION_VERTICAL));
  //TODO: make this cleaner and nicer to look at
  cairo_scale(cr, w/pane.width, h/pane.height);

  //test placeholder code
  GdkRGBA sel_color; //selected object color
  GdkRGBA obj_color; //other object color
  GdkRGBA bak_color; //background color

  gdk_rgba_parse(&sel_color, "rgb(255,255,0)");
  gdk_rgba_parse(&obj_color, "rgb(128,32,192)");
  gdk_rgba_parse(&bak_color, "rgb(31,31,31)");

  gdk_cairo_set_source_rgba(cr, &bak_color);
  cairo_paint(cr);

  //draw in region {(xmin,ymin) -> (xmax,ymax)}
  //drawing region is going to be {(-1,-1) -> (1,1)}
  //we need to put the object positions into a transform.

  for(unsigned i=0;i<data->objs.size();i++) {
    auto x = data->objs[i];
    if(*(data->sel) == i) {
      //skip, come back to later
    }

    //transform position & size to in the boundary, then draw a rectangle
    //draw selected object in special color, and do it last

    //get obj's base size from database
    sf::Vector2f osize = data->db[x.type].size;

    cairo_rectangle(cr, x.pos.x, x.pos.y, osize.x * x.scale.x, osize.y * x.scale.y);
    cairo_set_line_width(cr, 3);
    gdk_cairo_set_source_rgba (cr, &obj_color);
    cairo_stroke(cr);

  }
  //draw selected object
  auto z = data->objs[*(data->sel)];

  //get obj's base size from database
  sf::Vector2f osize = data->db[z.type].size;

  cairo_rectangle(cr, z.pos.x, z.pos.y, osize.x * z.scale.x, osize.y * z.scale.y);
  gdk_cairo_set_source_rgba(cr, &sel_color);
  cairo_set_line_width (cr, 4);
  cairo_stroke(cr);

    /*

  cairo_arc (cr,
             width / 2.0, height / 2.0,
             MIN (width, height) / 2.0,
             0, 2 * G_PI);
  */

}

ListData* d;
//open an empty file
static void app_activate(GtkApplication* app, gpointer* user_data) {
  //for now, don't do anything
}

//if more than 1 file is opened, only open first one and display a warning.
//eventually, multiple-file editing will be possible...
//open that file

static void app_open(GtkApplication* app, GFile** files, int n_files, char* hint, gpointer* user_data) {
  ListData* data = (ListData*) user_data;
  //check if a file is open
  if(n_files == 0) {
    //I doubt this is something to worry about, but just in case...
    return;
  }
  if(n_files != 1) {
    std::cerr << "Warning: multiple file opening not supported. Only the first file will be opened.\n";
  }
  if(!data->openFile.empty()) {
    std::cerr << "Warning: multiple file opening not supported\n";
    return;
  }
  read_file(data, files[0]);

}
//this sets the app up
static void app_startup(GtkApplication* app, gpointer user_data) {
  //setup data structures:
  static guint selected_obj;
  ListData* d = (ListData*) user_data;

  d->sel = &selected_obj;

  //setup css provider
  GtkCssProvider* provider = gtk_css_provider_new();
  gtk_css_provider_load_from_file(provider, g_file_new_for_path("assets/ui.css"));

  GdkDisplay *display = gdk_display_get_default ();
  gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER     
           (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  GtkBuilder *builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "assets/window.ui", NULL);

  GObject* window = gtk_builder_get_object(builder, "window");
  gtk_window_set_application(GTK_WINDOW(window), app);

  GObject* text_view = gtk_builder_get_object(builder, "StatusText");

  d->tv = GTK_WIDGET(text_view);
  d->win = GTK_WINDOW(window);

  //initialize other widgets
  //add callbacks to buttons?
  GObject* remove_btn = gtk_builder_get_object(builder, "RemoveObjectButton");
  GObject* entry = gtk_builder_get_object(builder, "ObjectTypeEntry");
  GObject* redraw_btn = gtk_builder_get_object(builder, "RedrawButton");
  GObject* sw_entry = gtk_builder_get_object(builder, "SwitchInfoEntry");

  //setup preview pane
  GObject* preview = gtk_builder_get_object(builder, "Preview");
  gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(preview), 100);
  gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(preview), 100);
  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(preview), draw_function, d, NULL);

  d->pre = GTK_WIDGET(preview);

  //setup list
  GObject* list_view = gtk_builder_get_object(builder, "ObjList");
  d->lv = GTK_WIDGET(list_view);

  GtkStringList* list = gtk_string_list_new({});
  GtkSingleSelection* sel = gtk_single_selection_new(G_LIST_MODEL(list));

  GtkListItemFactory* factory = gtk_signal_list_item_factory_new ();
  g_signal_connect(factory, "setup", G_CALLBACK(setup_cb), NULL);
  g_signal_connect(factory, "bind", G_CALLBACK(bind_cb), NULL);

  gtk_list_view_set_factory(GTK_LIST_VIEW(list_view), factory);
  gtk_list_view_set_model(GTK_LIST_VIEW(list_view), GTK_SELECTION_MODEL(sel));

  GObject* prop = gtk_builder_get_object(builder, "Properties");
  d->cv = GTK_WIDGET(prop);

  //setup properties pane
  GtkListItemFactory* c1f = gtk_signal_list_item_factory_new();
  g_signal_connect(c1f, "setup", G_CALLBACK(setup_field_cb), NULL);
  g_signal_connect(c1f, "bind", G_CALLBACK(bind_field_cb), NULL);
  GtkListItemFactory* c2f = gtk_signal_list_item_factory_new();
  g_signal_connect(c2f, "setup", G_CALLBACK(setup_val_cb), d);
  g_signal_connect(c2f, "bind", G_CALLBACK(bind_val_cb), NULL);


  GListStore* liststore = g_list_store_new(PROP_TYPE_ITEM);
  GtkNoSelection* psel = gtk_no_selection_new(G_LIST_MODEL(liststore));

  gtk_column_view_set_model(GTK_COLUMN_VIEW(prop), GTK_SELECTION_MODEL(psel));
  GtkColumnViewColumn* c1 = gtk_column_view_column_new("Field", c1f);
  GtkColumnViewColumn* c2 = gtk_column_view_column_new("Data", c2f);
  gtk_column_view_append_column(GTK_COLUMN_VIEW(prop), c1);
  gtk_column_view_append_column(GTK_COLUMN_VIEW(prop), c2);
  g_object_unref(c1);
  g_object_unref(c2);

  GListModel* cols = gtk_column_view_get_columns(GTK_COLUMN_VIEW(prop));
  GtkColumnViewColumn* col1 = GTK_COLUMN_VIEW_COLUMN(g_list_model_get_item(cols, 1));
  d->entries = col1;

  const std::vector<std::pair<std::string, std::string>> props = {
    {"xpos", "0"},
    {"ypos", "0"},
    {"x_scale", "1"},
    {"y_scale", "1"},
    {"Link ID", "-1"},
    {"Parent ID", "-1"},
    {"Texture ID", "-1"},
    {"Text Arg", "."},

    {"arg_0", "-1"},
    {"arg_1", "-1"},
    {"arg_2", "-1"},
    {"arg_3", "-1"},
    {"arg_4", "-1"},
    {"arg_5", "-1"},
    {"arg_6", "-1"},
    {"arg_7", "-1"},

    {"SW_Appear", "-1"},
    {"SW_Disappear", "-1"},
    {"SW_A", "-1"},
    {"SW_B", "-1"},
    {"SW_C", "-1"},
    {"SW_D", "-1"},
    {"SW_Stat", "-1"},
    {"SW_Remove", "-1"}
  };
  for(auto x : props) {
    PropItem* pi = prop_item_new(x.first.c_str(), x.second.c_str());
    g_list_store_append(liststore, pi);
    g_object_unref(pi);
  }

  //callbacks

  d->ent = GTK_WIDGET(entry);

  d->list = list;

  g_signal_connect(GTK_WIDGET(entry), "activate", G_CALLBACK(add_obj_btn_cb), d);
  g_signal_connect(GTK_WIDGET(remove_btn), "clicked", G_CALLBACK(rem_obj_btn_cb), d);
  g_signal_connect(GTK_WIDGET(redraw_btn), "clicked", G_CALLBACK(redraw_btn_cb), d);
  g_signal_connect(GTK_WIDGET(sw_entry), "activate", G_CALLBACK(sw_info_btn_cb), d);

  //selection model
  g_signal_connect(sel, "selection-changed", G_CALLBACK(change_selected_obj_cb), d);


  GtkBuilder* dbuilder = gtk_builder_new_from_file("assets/objdata.ui");

  GtkWidget* dialog = GTK_WIDGET(gtk_builder_get_object(dbuilder, "objdata"));
  gtk_window_set_transient_for(GTK_WINDOW(dialog), d->win);

  //load obj-db
  ed::parse_db("assets/obj.json", d->db);

  //setup callbacks for obj-db window

  GtkWidget* obj_ent = GTK_WIDGET(gtk_builder_get_object(dbuilder, "ObjectTypeEntry"));
  GtkWidget* obj_btn = GTK_WIDGET(gtk_builder_get_object(dbuilder, "AdjustButton"));
  GtkWidget* obj_tv = GTK_WIDGET(gtk_builder_get_object(dbuilder, "Output"));
  g_signal_connect(GTK_WIDGET(obj_ent), "activate", G_CALLBACK(obj_info_entry_activate_cb), d);
  g_signal_connect(GTK_WIDGET(obj_btn), "clicked", G_CALLBACK(obj_info_btn_cb), d);

  d->objdata = GTK_WINDOW(dialog);
  d->obj_tv = obj_tv;
  d->obj_ent = obj_ent;

  //menu
  GtkBuilder* mBuilder = gtk_builder_new_from_file("assets/menu.ui");
  GMenuModel* menuBar = G_MENU_MODEL(gtk_builder_get_object(mBuilder, "menuBar"));

  //setup app entries and accelerators
  const GActionEntry app_entries[] {
    {"quit", quit_activated, NULL, NULL, NULL},
    {"save", save_activated, NULL, NULL, NULL},
    {"saveas", saveas_activated, NULL, NULL, NULL},
    {"open", open_activated, NULL, NULL, NULL},
    {"verify", verify_activated, NULL, NULL, NULL},
    {"objdata", objdata_activated, NULL, NULL, NULL},
    //{"about", about_activated, NULL, NULL, NULL}
  };
  struct {
    const char *action_and_target;
    const char *accelerators[2];
  } accels[] = {
    { "app.save", { "<Control>s", NULL } },
    { "app.saveas", { "<Control><Shift>s", NULL } },
    { "app.quit", { "<Control>q", NULL } },
    { "app.open", { "<Control>o", NULL } },
    { "app.verify", {"<Alt>v", NULL} },
    { "app.objdata", {"<Alt>o", NULL} },
  };

  g_action_map_add_action_entries(G_ACTION_MAP(app), app_entries, G_N_ELEMENTS(app_entries), d);

  for (guint i = 0; i < G_N_ELEMENTS (accels); i++)
    gtk_application_set_accels_for_action (app, accels[i].action_and_target, accels[i].accelerators);

  //setup menubar
  gtk_application_set_menubar(GTK_APPLICATION(app), G_MENU_MODEL(menuBar));
  gtk_application_window_set_show_menubar(GTK_APPLICATION_WINDOW(window), TRUE);

  gtk_widget_set_visible(GTK_WIDGET(window), TRUE);
  g_object_unref(builder);
  g_object_unref(mBuilder);

}

int main (int argc, char **argv) {
  GtkApplication* app;
  int stat;
  d = new ListData;

  // add ability to handle opening file specified in arguments
  // tutorial: sections 7, 15
  // eventually, make it possible to open multiple files at once: use notebook widget
  app = gtk_application_new ("net.xavenna.stellar.editor", G_APPLICATION_HANDLES_OPEN);
  g_signal_connect(app, "startup", G_CALLBACK(app_startup), d);
  g_signal_connect(app, "activate", G_CALLBACK(app_activate), d);
  g_signal_connect(app, "open", G_CALLBACK(app_open), d);
  stat = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
  return stat;
}

