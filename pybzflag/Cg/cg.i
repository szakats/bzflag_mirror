%module cg
%{
#include <cg.h>
#include <cgGL.h>
%}

/* Cg */
// context functions
CGcontext cgCreateContext(void);
void cgDestroyContext(CGcontext);
int cgIsContext(CGcontext);
char *cgGetLastListing(CGcontext);

// program functions
CGprogram cgCreateProgram(CGcontext, CGenum, const char *, CGprofile, const char *,  const char **);
CGprogram cgCreateProgramFromFile(CGcontext, CGenum, const char *, CGprofile, const char *, const char **);
CGprogram cgCopyProgram(CGprogram);
void cgDestroyProgram(CGprogram);

CGprogram cgGetFirstProgram(CGcontext);
CGprogram cgGetNextProgram(CGprogram);
CGcontext cgGetProgramContext(CGprogram);
int cgIsProgram(CGprogram);

void cgCompileProgram(CGprogram);
int cgIsProgramCompiled(CGprogram);
const char *cgGetProgramString(CGprogram, CGenum);
CGprofile cgGetProgramProfile(CGprogram);

/* CgGL */
// profile functions
int cgGLIsProfileSupported(int profile);
