lib_LTLIBRARIES = irclink.la

AM_CPPFLAGS = @CONF_CPPFLAGS@
AM_CXXFLAGS = @CONF_CXXFLAGS@

irclink_la_SOURCES = irclink.cpp
irclink_la_LDFLAGS = -module -avoid-version
irclink_la_CXXFLAGS= -I$(top_srcdir)/plugin_utils
irclink_la_LIBADD  = $(top_builddir)/plugin_utils/libplugin_utils.la

EXTRA_DIST = \
	README.txt \
	irclink.def \
	irclink.sln \
	irclink.vcproj

MAINTAINERCLEANFILES =	\
	Makefile.in
