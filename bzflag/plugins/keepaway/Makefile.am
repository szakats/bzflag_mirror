lib_LTLIBRARIES = keepaway.la

AM_CPPFLAGS = @CONF_CPPFLAGS@
AM_CXXFLAGS = @CONF_CXXFLAGS@

keepaway_la_SOURCES = keepaway.cpp
keepaway_la_LDFLAGS = -module -avoid-version

EXTRA_DIST = \
	README.txt \
	keepaway.def \
	keepaway.sln \
	keepaway.vcproj \
	keepaway_admin.txt \
	keepaway_rules.txt

MAINTAINERCLEANFILES =	\
	Makefile.in
