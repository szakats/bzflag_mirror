%module cg
%{
#include <cg.h>
%}

/* Cg */
// context functions
CGcontext cgCreateContext(void);
void cgDestroyContext(CGcontext);
int cgIsContext(CGcontext);
char *cgGetLastListing(CGcontext);

/* CgGL */
// profile functions
int cgGLIsProfileSupported(int profile);
