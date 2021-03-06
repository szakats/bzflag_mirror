# NOTE: This Makefile.am is hideous!  We link with a bunch of files found
# in ../bzflag.  These files will be compiled to .o files in the current
# directory.  Most of these files are only linked to because the linker
# needs the symbols somewhere.  Several of the files should have all of the
# graphics stuff taken out and moved to ../game.  There's a ton of work
# ahead for this to happen.  In the meantime, this seems to be the least
# ugly of all of the solutions.

bin_PROGRAMS = bzrobots
lib_LTLIBRARIES = TestRobot.la $(libbzrobot)

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/clientbase \
	-I$(top_srcdir)/src/bzflag \
	$(SDL_CFLAGS) \
	$(LIBCURL_CPPFLAGS) \
	@X_CFLAGS@ \
	@CONF_CPPFLAGS@

PYTHON_LOADER = \
	PythonLoader.cxx	\
	PythonLoader.h

if BZROBOTS_PYTHON_ENABLED
AM_CXXFLAGS = @CONF_CXXFLAGS@ \
	$(PYTHON_CPPFLAGS) \
	-DWITH_PYTHONLOADER=1
bzrobots_LDFLAGS = -export-dynamic -static $(PYTHON_LDFLAGS)
pythonloaderfiles = $(PYTHON_LOADER)
else
AM_CXXFLAGS = @CONF_CXXFLAGS@
bzrobots_LDFLAGS = -export-dynamic -static
pythonloaderfiles =
endif

genericfiles =				\
	bzrobots.cxx			\
	botplaying.h			\
	botplaying.cxx			\
	RCLink.cxx			\
	RCLink.h			\
	RCMessage.h			\
	RCRequest.h			\
	RCRequests.cxx			\
	RCRequests_Process.cxx		\
	RCRequests.h			\
	RCReply.h			\
	RCReplies.cxx			\
	RCReplies.h			\
	RCEvent.h			\
	RCEvents.h			\
	RCEvents.cxx			\
	MessageUtilities.cxx		\
	MessageUtilities.h		\
	RCMessageFactory.h		\
	RCMessageFactory.cxx		\
	RCLinkBackend.cxx		\
	RCLinkBackend.h			\
	RCLinkFrontend.cxx		\
	RCLinkFrontend.h		\
	RCRobotPlayer.cxx		\
	RCRobotPlayer.h			\
	Tank.h				\
	Tank.cxx			\
	Shot.h				\
	Shot.cxx			\
	Frontend.cxx			\
	Frontend.h			\
	Logger.h			\
	Logger.cxx			\
	ScriptLoader.h			\
	ScriptLoaderFactory.cxx		\
	ScriptLoaderFactory.h		\
	SharedObjectLoader.cxx		\
	SharedObjectLoader.h		\
	BZAdvancedRobot.cxx		\
	BZAdvancedRobot.h		\
	AdvancedRobot.cxx		\
	AdvancedRobot.h			\
	BZRobot.cxx			\
	BZRobot.h			\
	Robot.cxx			\
	Robot.h				


if CLIENT_INCLUDED
bzrobots_SOURCES = $(genericfiles)		\
	$(pythonloaderfiles)
else
bzrobots_SOURCES = $(genericfiles)		\
	$(pythonloaderfiles)			\
	OpenGLUtils.cxx	
endif

if WIN32
RESOURCE = bzflag.res
else
RESOURCE =
endif

if MACOS
NACLIB = ../platform/MacOSX/libMac.la
endif

LDADD =	\
	../obstacle/libObstacle.la	\
	../clientbase/libClientBase.la	\
	../game/libGame.la		\
	../geometry/libGeometry.la	\
	../3D/lib3D.la			\
	../net/libNet.la		\
	$(NACLIB)			\
	../platform/libPlatform.la	\
	../ogl/libGLKit.la		\
	../scene/libSceneDB.la		\
	../mediafile/libMediaFile.la	\
	../common/libCommon.la		\
	$(LIBFTGL)			\
	$(LIBFREETYPE)			\
	$(LIBCURL)			\
	$(LIBCARES)			\
	$(LIBREGEX)			\
	$(LIBZ)				\
	$(RESOURCE)			\
	$(SDL_LIBS)			\
	$(GLIBS)			\
	$(GLEW_LIBS)			\
	../date/libDate.la

TestRobot_la_LDFLAGS = -module -avoid-version
TestRobot_la_SOURCES = \
	BZAdvancedRobot.h \
	../../bots/testbot/TestRobot.cxx \
	TestRobot.h

if BZROBOTS_PYTHON_ENABLED
libbzrobot = bzrobot_pyext.la
bzrobot_pyext_la_CXXFLAGS = -fexceptions $(SWIG_PYTHON_CPPFLAGS)
bzrobot_pyext_la_LDFLAGS = -module -avoid-version -export-dynamic
bzrobot_pyext_la_SOURCES = \
	BZAdvancedRobot.cxx \
	BZAdvancedRobot.h \
	RCRequests.cxx \
	RCRequests_ProcessSWIG.cxx \
	RCRequests.h \
	RCLink.cxx \
	RCLink.h \
	RCLinkFrontend.cxx \
	RCLinkFrontend.h \
	RCEvent.h \
	MessageUtilities.cxx \
	MessageUtilities.h \
	Tank.cxx \
	Tank.h

# FIXME -- these files seem to be missing,
#       -- commented to make the 'make dist' top level target work
#	bzrobot_python_wrap.cxx \
#	bzrobot_python_wrap.h

bzrobot_pyext_la_LIBADD = ../date/libDate.la

BUILT_SOURCES = \
	bzrobot_python_wrap.cxx \
	bzrobot_python_wrap.h \
	bzrobot_python_runtime.h

bzrobot_python_wrap.h: bzrobot_python_wrap.cxx

bzrobot_python_wrap.cxx: bzrobot.i BZAdvancedRobot.h
	$(SWIG) -Wall -c++ -python -o $@ -interface bzrobot_pyext $< # Consider using -O for 'optimized mode'

bzrobot_python_runtime.h:
	$(SWIG) -python -external-runtime $@
else
libbzrobot =
endif

bzflag.res: $(top_srcdir)/win32/bzflag.rc $(top_srcdir)/win32/bzflag.ico
	$(WINDRES) --include-dir=$(top_srcdir)/win32/ -i $(top_srcdir)/win32/bzflag.rc -o bzflag.res \
	-O coff

EXTRA_DIST = \
	$(BUILT_SOURCES) \
	$(PYTHON_LOADER) \
	OpenGLUtils.cxx	\
	RCRequests_ProcessSWIG.cxx \
	bzrobot.i

MAINTAINERCLEANFILES = \
	Makefile.in

CLEANFILES = \
	bzrobot.py \
	bzrobot_python_wrap.cxx \
	bzrobot_python_wrap.h \
	bzrobot_python_runtime.h

