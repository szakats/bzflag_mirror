#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include <string>
#include <map>
#include <vector>

typedef struct {
  unsigned int x, y;
  bool alpha;
  unsigned int glID;
  int manID;
  char file[512];
  char name[64];
} TextureInfo;

#ifdef WIN32
inline bool operator < (const std::string &s1, const std::string &s2) { return (s1.compare(s2) < 0); }
#endif

typedef std::map<std::string, int> TextureNameMap;
typedef std::vector<TextureInfo *> TextureList;

class TextureManager {
  public:
    TextureManager();
    ~TextureManager();

    int load(const char *path, bool skin = false);
    int loadAll(const char *path, bool skin = false);
    int getID(const char *name);
    bool getInfo(int id, TextureInfo *info);
    bool bind(int id);
    bool flush(int id = -1);
  private:
    int loadBMP(const char *path, TextureInfo *info, bool skin = false);
    int loadTGA(const char *path, TextureInfo *info, bool skin = false);

    TextureNameMap textureMap;
    TextureList textureList;

    int lastBindID;
};

#endif
