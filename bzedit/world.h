#ifndef __WORLD_H__
#define __WORLD_H__

#include <vector>
#include <string>
#include "element.h"
#include "texture_manager.h"
#include "ground.h"
#include "walls.h"

#ifndef M_PI
#define M_PI      3.141592653589794f
#endif
#define M_RAD2DEG 57.29577951308232087678f
#define M_DEG2RAD 0.01745329251994329577f

typedef std::vector<Element> ElementList;

class World {
  public:
    World();
    World(const World &w);
    ~World();
    World operator = (const World &w);

    void init();
    void clear();
    
    void render(double transparency = 100.0);
    void render_targets();

    void uniqueName(Element *el);
    void uniqueName(int item);

    bool load(const char *filename);
    bool append(const char *filename);
    bool write(const char *filename);

    ObjectType getElementType(Element *el);
    ObjectType getElementType(int item);

    Element *getElement(char *name);
    Element *getElement(int item);
    Element *getNthElementOfType(ObjectType type, int n);
    
    int addElement(ObjectType type);
    bool removeElement(int item);

    int getNumOfType(ObjectType type);

    int findElementWithName(std::string name);

    void sort();

    void setTextureManager(TextureManager *texman);
    void setModelManager(ModelManager *modman);

    TextureManager *texman;
    ModelManager *modman;

    ElementList list;
    int selItem;

  private:
    Ground ground;
    Walls walls;
};

#endif
