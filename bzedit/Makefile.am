INCLUDES = \
	@GNOME_CFLAGS@ \
	@GTK_CFLAGS@ \
	@GTKGL_CFLAGS@ \
	@GL_CFLAGS@

bin_PROGRAMS = bzedit
noinst_HEADERS = \
	camera.h \
	objects.h \
	element.h \
	ui_edit.h \
	ui_main.h \
	ui_list.h \
	textures.h \
	tga.h \
	world.h

bzedit_SOURCES = \
	bzedit.cpp \
	element.cpp \
	objects.cpp \
	ui_edit.cpp \
	ui_main.cpp \
	ui_list.cpp \
	textures.cpp \
	tga.cpp \
	world.cpp

bzedit_LDADD = \
	@GNOME_LIBS@ \
	@GTK_LIBS@ \
	@GTKGL_LIBS@ \
	@GL_LIBS@

EXTRA_DIST = \
	data/boxwall.tga \
	data/caution.tga \
	data/ground.tga \
	data/pyrwall.tga \
	data/roof.tga \
	data/wall.tga \
	help/bzedit/C/index.html \
	help/bzedit/C/intro.html \
	help/bzedit/C/mainwin.html \
	help/bzedit/C/topic.dat
