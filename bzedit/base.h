#ifndef __BASE_H__
#define __BASE_H__

#include "object.h"

class Base : public Object {
  public:
    Base();
    Base(const Base &b);
    Base operator = (const Base &b);
    virtual void copyFrom(Object *o);

    bool init();
    void render(bool transparent = false, bool selected = false, int name = -1);

    void write(ostream &stream);
    void write(char *data);
    bool read(char *data);

    bool setItemInfo(char *name, int data);
    bool getItemInfo(char *name, int &data);

  private:
    int color;
    int baseModel;
    int wallModel;
};

#endif
