#!/usr/bin/env python
#
# Pretty much a straight translation of the CgGL example distributed with the Cg toolkit
# this is -not- platform agnostic at the moment
#
import pygame, OpenGL.GL, sys
from Cg import *

def CheckCGError():
    error = Cg.cgGetError()
    if error != CG_NO_ERROR:
	sys.exit('Cg error: %s' % Cg.cgGetErrorString(error))

context = Cg.cgCreateContext()
CheckCGError()

program = cgCreateProgramFromFile(context, CG_SOURCE, 'extra_media/cgGL_vertex_example.cg', CG_PROFILE_VP20, 'main', None)
CheckCGError()

print 'LAST LISTING----%s----' % cgGetLastListing(context)
print '---- PROGRAM BEGIN ----'
print cgGetProgramString(program, CG_COMPILED_PROGRAM)
print '---- PROGRAM END ----'

cgGLLoadProgram(program)
CheckCGError()

Kd = cgGetNamedParameter(program, 'Kd')
CheckCGError()

modelViewProj = cgGetNamedParameter(program, 'ModelViewProj')
CheckCGError()

testColor = cgGetNamedParameter(program, 'IN.TestColor')
CheckCGError()


# do drawing loop

cgDestroyProgram(program)
cgDestroyContext(context)
