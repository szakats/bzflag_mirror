%module Cg

/* context functions */
extern CGContext *cgCreateContext();
extern void cgDestroyContext(CGContext*);
extern bool cgIsContext(CGContext*);
extern char *cgGetLastListing(CGContext*);
