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
	parser.cxx \
	lexer.cxx

OBJECTS = ${FILES:.cxx=.o}

.PHONY: all clean blather
.SUFFIXES: .cxx .o .l .y

all: blather bzwgen

.cxx.o: common.h
	${CXX} ${CFLAGS} ${CPPFLAGS} -c -o $@ $<
	
lexer.cxx: lexer.l
	flex -o$@ $<

parser.cxx: parser.y
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
	rm -f bzwgen ${OBJECTS}
	@echo "Done."

bzwgen: ${OBJECTS}
	@echo ""
	@echo "Building bzwgen..."
	@echo ""
	${CXX} -o $@ ${OBJECTS} ${CFLAGS} ${LDFLAGS} ${LIBS}
	@echo "Done!"
