lib_LTLIBRARIES = flagStay.la

AM_CPPFLAGS = @CONF_CPPFLAGS@
AM_CXXFLAGS = @CONF_CXXFLAGS@

flagStay_la_SOURCES = flagStay.cpp
flagStay_la_CPPFLAGS = -I$(top_srcdir)/include
flagStay_la_LDFLAGS = -module -avoid-version

EXTRA_DIST = \
	README.txt \
	flagStay.def \
	flagStay.sln \
	flagStay.vcproj

MAINTAINERCLEANFILES =	\
	Makefile.in
