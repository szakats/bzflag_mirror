#!/usr/bin/env python
#
# Pretty much a straight translation of the CgGL example distributed with the Cg toolkit
# this is -not- platform agnostic at the moment
#
import OpenGL.GL, sys
from BZFlag import Event
from BZFlag.UI import Viewport, HUD, Layout
from Cg import *

def CheckCGError():
    error = Cg.cgGetError()
    if error != CG_NO_ERROR:
	sys.exit('Cg error: %s' % Cg.cgGetErrorString(error))

loop = Event.EventLoop()
viewport = Viewport.OpenGLViewport(loop, (800, 600))

context = Cg.cgCreateContext()
CheckCGError()

program = cgCreateProgramFromFile(context, CG_SOURCE, 'extra_media/cgGL_vertex_example.cg', CG_PROFILE_VP20, 'main', None)
CheckCGError()

print 'LAST LISTING----%s----' % cgGetLastListing(context)
print '---- PROGRAM BEGIN ----'
programString = cgGetProgramString(program, CG_COMPILED_PROGRAM)
print programString
print '---- PROGRAM END ----'

cgGLLoadProgram(program)
CheckCGError()

Kd = cgGetNamedParameter(program, 'Kd')
CheckCGError()

modelViewProj = cgGetNamedParameter(program, 'ModelViewProj')
CheckCGError()

testColor = cgGetNamedParameter(program, 'IN.TestColor')
CheckCGError()


HUD.Text(viewport.region(Layout.Rect(viewport).margin(10)), programString, fontSize=10)

loop.run()

cgDestroyProgram(program)
cgDestroyContext(context)
