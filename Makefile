CXX = g++
LIBS =
CFLAGS = -g -O0 -Wall -pedantic -ansi
LDFLAGS =
CPPFLAGS = -I../bzflag/include

FILES = \
	Generator.cxx \
	GridGenerator.cxx \
	GridMap.cxx \
	Material.cxx \
	BuildZone.cxx \
	FloorZone.cxx \
	bzwgen.cxx

OBJECTS = ${FILES:.cxx=.o}

.PHONY: all clean blather

all: blather bzwgen

#.cxx.o:
#	${CXX} ${CFLAGS} ${CPPFLAGS} -c $<

Generator.o: Generator.cxx Generator.h common.h
	${CXX} ${CFLAGS} ${CPPFLAGS} -c $<

GridGenerator.o: GridGenerator.cxx GridGenerator.h common.h
	${CXX} ${CFLAGS} ${CPPFLAGS} -c $<

GridMap.o: GridMap.cxx GridMap.h common.h
	${CXX} ${CFLAGS} ${CPPFLAGS} -c $<

Material.o: Material.cxx Material.h common.h
	${CXX} ${CFLAGS} ${CPPFLAGS} -c $<

BuildZone.o: BuildZone.cxx BuildZone.h common.h
	${CXX} ${CFLAGS} ${CPPFLAGS} -c $<

FloorZone.o: FloorZone.cxx FloorZone.h common.h
	${CXX} ${CFLAGS} ${CPPFLAGS} -c $<
    
bzwgen.o: bzwgen.cxx common.h
	${CXX} ${CFLAGS} ${CPPFLAGS} -c $<

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

