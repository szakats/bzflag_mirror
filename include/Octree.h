
#ifndef __OCTREE_H__
#define __OCTREE_H__

#include "SceneNode.h"
#include "Frustum.h"


// for visualizing and debugging, equivalent to GL_LINE_STRIP
typedef void (*DrawLineFunc)(int vertices, const float points[][3], bool partial);


class OctreeNode {

  public:

    OctreeNode(const float* center, float width, unsigned char depth,
               SceneNode** list, int listSize);
    ~OctreeNode();

    void getFrustumList () const;
    void getFullyVisible () const;
    OctreeNode* getChild (int child);

    int getCount();    // number of nodes in this and subnodes
    int getChildren(); // number of children 
    int getListSize(); // number of nodes in this node
    SceneNode** getList();     // list of nodes

    void draw (DrawLineFunc drawLines);

  private:
  
    enum CullLevel {
      NoCull,
      PartialCull,
      FullCull
    };

    float width;
    unsigned char depth;
    float mins[3];
    float maxs[3];
    unsigned char childCount;
    OctreeNode* children[8];
    int count;  // number of nodes in this and subnodes
    int listSize;
    SceneNode** list;
};

inline int OctreeNode::getCount()
{
  return count;
}

inline SceneNode** OctreeNode::getList()
{
  return list;
}

inline int OctreeNode::getListSize()
{
  return listSize;
}

inline int OctreeNode::getChildren()
{
  return childCount;
}

inline OctreeNode* OctreeNode::getChild (int child)
{
  return children[child];
}


class Octree {

  public:

    Octree();
    ~Octree();

    void addNodes (SceneNode** list, int listSize, int depth, int elements);
    int getFrustumList (SceneNode** list, int listSize,
                        const Frustum* frustum) const;
    void clear ();
    void draw (DrawLineFunc drawLines);
    
  private:
  
    void getExtents(float* mins, float* maxs, float& width,
                    SceneNode** list, int listSize);
  
    OctreeNode* root;
};


#endif // __OCTREE_H__
