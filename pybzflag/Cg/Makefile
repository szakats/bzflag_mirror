all:	_cg.so

_cg.so:	cg.i
	swig -python cg.i
	gcc -c cg_wrap.c -I/usr/include/Cg -I/usr/include/python2.2
	ld -shared cg_wrap.c -o _cg.so
