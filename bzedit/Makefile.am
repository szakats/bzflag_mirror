INCLUDES = \
	@GNOME_CFLAGS@ \
	@GTK_CFLAGS@ \
	@GTKGL_CFLAGS@ \
	@GL_CFLAGS@

SUBDIRS = . data help

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
	data/basicon.png \
	data/boxicon.png \
	data/boxwall.tga \
	data/caution.tga \
	data/ground.tga \
	data/lnkicon.png \
	data/pyricon.png \
	data/pyrwall.tga \
	data/roof.tga \
	data/telicon.png \
	data/wall.tga \
	help/index.html \
	help/intro.html \
	help/mainwin.html \
	help/topic.dat
