
SRCS = $(shell find . -name '*.cpp')

OBJS = $(SRCS:.cpp=.o)


INCS = -I../include

LIBS = -losg -losgViewer -lfltk -lfltk_gl -lfltk_images -ldl


all:	bzworkbench

.cpp.o:
	g++ $(INCS) -c -o $@ $<

bzworkbench:	$(OBJS)
	g++ -o bzworkbench $(OBJS) $(LIBS)
	
clean:
	find . -name '*~'  -delete
	find . -name '*.o' -delete

fresh:
	$(MAKE) clean
	$(MAKE) all
