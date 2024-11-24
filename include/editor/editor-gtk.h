#ifndef EDITOR_GTK_H
#define EDITOR_GTK_H

#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib/gstdio.h>

#include "stellar-enigma.hpp"

#include "editor/file-io.h"
#include "editor/editor.h"

#define PROP_TYPE_ITEM  prop_item_get_type()
G_DECLARE_FINAL_TYPE(PropItem, prop_item, PROP, ITEM, GObject);
struct _PropItemClass {
  GObjectClass parent_class;
};

struct _PropItem {
  GObject parent;
  std::string prop;
  std::string value;
};

G_DEFINE_FINAL_TYPE(PropItem, prop_item, G_TYPE_OBJECT);
//the semicolon isn't necessary here, but vim's indentation engine gets confused if it is omitted

enum {
  PROP_0,
  PROP_PROP,
  PROP_VALUE,
  N_PROPERTIES
};

static GParamSpec *propitem_properties[N_PROPERTIES] = {NULL, };

static void prop_item_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
  PropItem *self = PROP_ITEM(object);

  if (property_id == PROP_PROP) {
    self->prop = g_value_get_string(value);
  }
  else if(property_id == PROP_VALUE) {
    self->value = g_value_get_string(value);
  }
  else {
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
  }
}

static void prop_item_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
  PropItem *self = PROP_ITEM(object);

  if (property_id == PROP_PROP) {
    g_value_set_string(value, g_strdup(self->prop.c_str()));
  }
  else if(property_id == PROP_VALUE) {
    g_value_set_string(value, g_strdup(self->value.c_str()));
  }
  else
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
}

static void prop_item_class_init(PropItemClass* cl) {
  GObjectClass* gobject_class = G_OBJECT_CLASS(cl);

  gobject_class->set_property = prop_item_set_property;
  gobject_class->get_property = prop_item_get_property;
  propitem_properties[PROP_PROP] = g_param_spec_string("prop", "prop", "Object Attribute name", "", G_PARAM_READWRITE);
  propitem_properties[PROP_VALUE] = g_param_spec_string("value", "value", "Object Attribute value", "", G_PARAM_READWRITE);
  g_object_class_install_properties(gobject_class, N_PROPERTIES, propitem_properties);

}

static void prop_item_init(PropItem* self) {

}

static PropItem* prop_item_new(const char* field, const char* value) {
  PropItem* item = (PropItem*)  g_object_new(PROP_TYPE_ITEM, NULL);
  item->prop = field;
  item->value = value;
  return item;
}

void prop_item_set_value(PropItem* self, const char* string) {
  g_object_set (G_OBJECT(self), "value", string, NULL);
  self->value = string;
}

char* prop_item_get_value(PropItem* self) {
  return g_strdup(self->value.c_str());
}

void prop_item_set_prop(PropItem* self, const char* string) {
  self->prop = string;
}

void prop_item_get_prop() {

}

// end PropItem GObject declaration

struct ListData {
  GtkWidget* ent; //!< text entry box [for adding objs]
  GtkWidget* lv; //!< list view [object list]
  GtkWidget* tv; //!< text view
  GtkWidget* cv; //!< column view
  GtkWidget* pre; //!< preview drawing area
  GtkColumnViewColumn* entries; //!< Data entries
  GtkWindow* win; //!< Window
  GtkStringList* list;
  guint* sel; //!< selected obj
  std::string openFile=""; //!< the currently opened file
  std::vector<ObjectBase> objs;
};


void read_file(ListData* data, GFile* file);
#endif
