
if HAVE_GLUT
noinst_PROGRAMS = simple c-demo FTGLDemo FTGLMFontDemo
endif

AM_CPPFLAGS = -I$(top_srcdir)/src $(FT2_CPPFLAGS)

simple_SOURCES = \
    simple.cpp \
    $(NULL)
simple_CXXFLAGS = $(FT2_CFLAGS) $(GL_CFLAGS)
simple_LDFLAGS = $(GLUT_LIBS)
simple_LDADD = ../src/libftgl.la $(FT2_LIBS)

c_demo_SOURCES = \
    c-demo.c \
    $(NULL)
c_demo_CFLAGS = $(FT2_CFLAGS) $(GL_CFLAGS)
c_demo_LDFLAGS = $(GLUT_LIBS)
c_demo_LDADD = ../src/libftgl.la $(FT2_LIBS)
# avoid __Unwind_Resume undefined symbol on MacOS with Xcode 2.5
CCLD = $(CXX)

FTGLDemo_SOURCES = \
	FTGLDemo.cpp \
	tb.c \
	tb.h \
	trackball.c \
	trackball.h \
	$(NULL)
FTGLDemo_CXXFLAGS = $(FT2_CFLAGS) $(GL_CFLAGS)
FTGLDemo_LDFLAGS = $(GLUT_LIBS)
FTGLDemo_LDADD = ../src/libftgl.la $(FT2_LIBS)

FTGLMFontDemo_SOURCES = \
	FTGLMFontDemo.cpp \
	tb.c \
	tb.h \
	trackball.c \
	trackball.h \
	$(NULL)
FTGLMFontDemo_CXXFLAGS = $(FT2_CFLAGS) $(GL_CFLAGS)
FTGLMFontDemo_LDFLAGS = $(GLUT_LIBS)
FTGLMFontDemo_LDADD = ../src/libftgl.la $(FT2_LIBS)

NULL =
