#ifndef __UI_INSPECT_H__
#define __UI_INSPECT_H__

#include "document.h"
#include "callbacks.h"

namespace UI {

class InspectorWindow {
  public:
    InspectorWindow();

    void enable_name();
    void enable_type();
    void enable_position_x();
    void enable_position_y();
    void enable_position_z();
    void enable_rotation();
    void enable_size_x();
    void enable_size_y();
    void enable_size_z();
    void enable_other_1();
    void enable_other_2();
    void setboxpyr();
    void settele();
    void setlink();
    void setbase();
    void disable_all();

    void set_name(const char *contents);
    void set_type(const char *contents);
    void set_position_x(const char *contents);
    void set_position_y(const char *contents);
    void set_position_z(const char *contents);
    void set_rotation(const char *contents);
    void set_size_x(const char *contents);
    void set_size_y(const char *contents);
    void set_size_z(const char *contents);
    void set_other_1(const char *contents);
    void set_other_2(const char *contents);

    const char *get_name();
    const char *get_position_x();
    const char *get_position_y();
    const char *get_position_z();
    const char *get_rotation();
    const char *get_size_x();
    const char *get_size_y();
    const char *get_size_z();
    const char *get_other_1();
    const char *get_other_2();

    GtkWidget *getWidget();

  private:
    GtkWidget *window;
    GtkWidget *vbox, *hbox1, *hbox3;
    GtkWidget *label_name, *label_type;
    GtkWidget *entry_name, *entry_type;
    GtkWidget *duplicate, *remove, *apply;
    GtkWidget *frame_element_info, *frame_pos;
    GtkWidget *frame_rotation, *frame_size;
    GtkWidget *frame_other;
    GtkWidget *labelX1, *labelY1, *labelZ1;
    GtkWidget *entryX1, *entryY1, *entryZ1;
    GtkWidget *entryR;
    GtkWidget *labelX2, *labelY2, *labelZ2;
    GtkWidget *entryX2, *entryY2, *entryZ2;
    GtkWidget *labelO1, *labelO2;
    GtkWidget *entryO1, *entryO2;
    GtkWidget *table, *tpos, *tsize, *tother;
};

}

#endif
