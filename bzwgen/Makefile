CXX = g++
LIBS = -lm
#CFLAGS = -g -O0 -Wall -Werror -pedantic -ansi -I./inc
CFLAGS = -g -O0 -Wall -pedantic -ansi -I./inc
LDFLAGS =
 
FILES = \
	src/graph/Face.cxx \
	src/graph/Node.cxx \
	src/graph/PlanarGraph.cxx \
	src/graph/SortedEdgeList.cxx \
	src/BZWGenerator.cxx \
	src/BuildZone.cxx \
	src/FloorZone.cxx \
	src/Generator.cxx \
	src/GridGenerator.cxx \
	src/FaceGenerator.cxx \
	src/Mesh.cxx \
	src/MultiFace.cxx \
	src/Operation.cxx \
	src/Expression.cxx \
	src/OSFile.cxx \
	src/Rule.cxx \
	src/RuleSet.cxx \
	src/TextUtils.cxx \
	src/bzwgen.cxx \
	src/commandArgs.cxx \
	src/parser.cxx \
	src/lexer.cxx
 
APP_FILES = \
	src/BZWGeneratorStandalone.cxx
 
PLUGIN_FILES = \
	src/BZWGeneratorPlugin.cxx
 
OBJECTS = ${FILES:.cxx=.o}
PICOBJECTS = ${FILES:.cxx=_pic.o}
 
APP_OBJECTS = ${APP_FILES:.cxx=.o}
PLUGIN_OBJECTS = ${PLUGIN_FILES:.cxx=_pic.o}
 
.PHONY: all clean blather
.SUFFIXES: .cxx _pic.o .o .l .y
 
all: blather bzwgen
 
plugin: blather bzwgenplugin 
 
.cxx_pic.o:
	${CXX} ${CFLAGS} ${CPPFLAGS} -fpic -c -o $@ $<
 
.cxx.o:
	${CXX} ${CFLAGS} ${CPPFLAGS} -c -o $@ $<
 
src/lexer.cxx: src/lexer.l
	flex -o$@ $<
 
src/parser.cxx: src/parser.y
	bison -d -o$@ $<
 
blather:
	@echo ""
	@echo "Using the following settings:"
	@echo "  CXX=\"$(CXX)\""
	@echo "  LIBS=\"$(LIBS)\""
	@echo "  CFLAGS=\"$(CFLAGS)\""
	@echo "  LDFLAGS=\"$(LDFLAGS)\""
	@echo "  CPPFLAGS=\"$(CPPFLAGS)\""
	@echo ""
 
clean:
	@echo "Cleaning up..."
	rm -f bzwgen src/lexer.cxx src/parser.[ch]xx ${OBJECTS} ${APP_OBJECTS} ${PICOBJECTS}
	@echo "Done."
 
bzwgen: ${OBJECTS} ${APP_OBJECTS}
	@echo ""
	@echo "Linking bzwgen..."
	@echo ""
	${CXX} -o $@ ${OBJECTS} ${APP_OBJECTS} ${CFLAGS} ${LDFLAGS} ${LIBS}
	@echo "Done."
 
bzwgenplugin: ${PICOBJECTS} ${PLUGIN_OBJECTS}
	@echo ""
	@echo "Linking bzwgen as a plugin..."
	@echo ""
	${CXX} -I../bzflag/include/ -shared -o $@.so ${PICOBJECTS} ${PLUGIN_OBJECTS} ${CFLAGS} -DCOMPILE_PLUGIN ${LDFLAGS} ${LIBS}

