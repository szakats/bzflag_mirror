lib_LTLIBRARIES = killall.la

AM_CPPFLAGS = @CONF_CPPFLAGS@
AM_CXXFLAGS = @CONF_CXXFLAGS@

killall_la_SOURCES = killall.cpp
killall_la_LDFLAGS = -module -avoid-version

EXTRA_DIST = \
	killall.def \
	killall.sln \
	killall.vcproj \
	README.txt

MAINTAINERCLEANFILES =	\
	Makefile.in
