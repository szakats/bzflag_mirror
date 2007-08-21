CXX = g++
LIBS = -lm
CFLAGS = -g -O0 -Wall -Werror -pedantic -ansi -I./inc
LDFLAGS =

FILES = \
	src/BuildZone.cxx \
	src/FloorZone.cxx \
	src/Generator.cxx \
	src/GridGenerator.cxx \
	src/GridMap.cxx \
	src/Material.cxx \
	src/Mesh.cxx \
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

OBJECTS = ${FILES:.cxx=.o}

.PHONY: all clean blather
.SUFFIXES: .cxx .o .l .y

all: blather bzwgen

.cxx.o: common.h
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
	rm -f bzwgen src/lexer.cxx src/parser.[ch]xx ${OBJECTS}
	@echo "Done."

bzwgen: ${OBJECTS}
	@echo ""
	@echo "Building bzwgen..."
	@echo ""
	${CXX} -o $@ ${OBJECTS} ${CFLAGS} ${LDFLAGS} ${LIBS}
	@echo "Done!"
