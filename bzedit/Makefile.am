INCLUDES = \
	@GNOME_CFLAGS@ \
	@GTK_CFLAGS@ \
	@GTKGL_CFLAGS@ \
	-DDATADIR=\""$(datadir)"\"

SUBDIRS = . data help

bin_PROGRAMS = bzedit
noinst_HEADERS = \
	util.h \
	texture_manager.h \
	camera.h \
	object.h \
	box.h \
	pyramid.h \
	teleporter.h \
	link.h \
	base.h \
	ground.h \
	walls.h \
	element.h \
	tga.h \
	bmp.h \
	world.h \
	document.h \
	menus.h \
	toolbar.h \
	ui_main.h \
	ui_list.h \
	ui_inspect.h \
	callbacks.h \
	globals.h \
	model.h \
	3dtypes.h

bzedit_SOURCES = \
	bzedit.cpp \
	camera.cpp \
	util.cpp \
	texture_manager.cpp \
	element.cpp \
	object.cpp \
	box.cpp \
	pyramid.cpp \
	teleporter.cpp \
	link.cpp \
	base.cpp \
	ground.cpp \
	walls.cpp \
	tga.cpp \
	world.cpp \
	document.cpp \
	toolbar.cpp \
	ui_main.cpp \
	ui_list.cpp \
	ui_inspect.cpp \
	callbacks.cpp \
	globals.cpp \
	model.cpp \
	3dtypes.cpp


bzedit_LDADD = \
	@GNOME_LIBS@ \
	@GTK_LIBS@ \
	@GTKGL_LIBS@

EXTRA_DIST = \
	data/addbase.xpm \
	data/addbox.xpm \
	data/addlink.xpm \
	data/addpyr.xpm \
	data/addtele.xpm \
	data/basicon.xpm \
	data/boxicon.xpm \
	data/boxwall.tga \
	data/caution.tga \
	data/ground.tga \
	data/iso.xpm \
	data/links.tga \
	data/lnkicon.xpm \
	data/pyricon.xpm \
	data/pyrwall.tga \
	data/roof.tga \
	data/stub.xpm \
	data/target.xpm \
	data/telicon.xpm \
	data/wall.tga \
	data/xminus.xpm \
	data/xplus.xpm \
	data/yminus.xpm \
	data/yplus.xpm \
	data/zplus.xpm \
	help/index.html \
	help/intro.html \
	help/mainwin.html \
	help/topic.dat
