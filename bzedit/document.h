#ifndef __DOCUMENT_H__
#define __DOCUMENT_H__

#include <string>
#include "world.h"
#include "camera.h"
#include "texture_manager.h"

class Document {
  public:
    Document(const string&);

    void file_new();
    bool file_open(const char*);
    bool file_save();
    bool file_save(const char*);
    void file_exit();
    void edit_undo();
    void edit_redo();
    void edit_cut();
    void edit_copy();
    void edit_paste();
    void view_grid();
    void view_element_list();
    void view_element_inspector();
    void view_move_to_selection();
    void utility_convert_to_deg();
    void utility_convert_to_rad();

    void tb_zplus();
    void tb_yplus();
    void tb_yminus();
    void tb_xplus();
    void tb_xminus();
    void tb_iso();
    void tb_center();
    void tb_addbox();
    void tb_addpyr();
    void tb_addtel();
    void tb_addlnk();
    void tb_addbas();

    Camera camera;
    World world;
    World *saveaction;
    bool open;
    bool hasname;
    bool unmodified;
	bool exiting;
    std::string file;
    TextureManager texman;
    ModelManager modman;
    int listItem;

    bool lastaction;
    bool nextaction;
};

#endif
