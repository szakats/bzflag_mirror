#ifndef __TELEPORTER_H__
#define __TELEPORTER_H__

#include "object.h"

class Teleporter : public Object {
  public:
    Teleporter();
    Teleporter(const Teleporter &t);
    Teleporter operator = (const Teleporter &t);
    void copyFrom(Object *o);

    bool init();
    void render(bool transparent = false, bool selected = false, int name = -1);

    void write(ostream &stream);
    void write(char *data);
    bool read(char *data);

    bool setItemInfo(char *name, float data);
    bool getItemInfo(char *name, float &data);

  private:
    float border;
    int texture;
    int fieldModel;
    int frameModel;
};

#endif
