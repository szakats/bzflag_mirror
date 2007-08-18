CXX = g++
LIBS = -lm
CFLAGS = -g -O0 -Wall -Werror -pedantic -ansi
LDFLAGS =

FILES = \
	BuildZone.cxx \
	FloorZone.cxx \
	Generator.cxx \
	GridGenerator.cxx \
	GridMap.cxx \
	Material.cxx \
	Mesh.cxx \
	Operation.cxx \
	Expression.cxx \
	OSFile.cxx \
	Rule.cxx \
	RuleSet.cxx \
	TextUtils.cxx \
	bzwgen.cxx \
	commandArgs.cxx \
	parser.cxx \
	lexer.cxx

OBJECTS = ${FILES:.cxx=.o}

.PHONY: all clean blather
.SUFFIXES: .cxx .o .l .y

all: blather bzwgen

.cxx.o: common.h
	${CXX} ${CFLAGS} ${CPPFLAGS} -c $<
	
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
