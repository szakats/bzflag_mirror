#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include <iostream>
#include <string>
#include "camera.h"
#include "texture_manager.h"
#include "model.h"

using namespace std;

class Object {
  public:
    Object();
    Object(const Object &o);
    ~Object();

    virtual void copyFrom(Object *o);

    void set_px(float px);
    void set_py(float py);
    void set_pz(float pz);
    void set_sx(float sx);
    void set_sy(float sy);
    void set_sz(float sz);
    void set_angle(float angle);
    void setTextureManager(TextureManager *man);
    void setModelManager(ModelManager *man);

    float get_px();
    float get_py();
    float get_pz();
    float get_sx();
    float get_sy();
    float get_sz();
    float get_angle();

    void defaultData();

    virtual void render(bool transparent = false, bool selected = false, int name = -1);
    virtual void write(ostream &stream);
    virtual void write(char *data);
    virtual bool read(char *data);
    virtual bool init();

    virtual bool setItemInfo(char *name, int data);
    virtual bool setItemInfo(char *name, float data);
    virtual bool setItemInfo(char *name, char *data);
    virtual bool getItemInfo(char *name, int &data);
    virtual bool getItemInfo(char *name, float &data);
    virtual bool getItemInfo(char *name, char *data);

    const char *getListName();

  protected:
    TextureManager *texman;
    ModelManager *modman;
    float px, py, pz;
    float sx, sy, sz;
    float angle;

    std::string listName;

    bool stdDataField(char *line);
};

#endif
