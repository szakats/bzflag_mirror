# This macro tries to find curses, and defines HAVE_CURSES_H or HAVE_NCURSES_H
# if any of those headers are found. It also defines CURSES_LIB.
AC_DEFUN([MP_WITH_CURSES],
  [AC_ARG_WITH(ncurses, [  --with-ncurses          Force the use of ncurses over curses],,)
  AC_ARG_WITH(xcurses, [  --with-xcurses          Force the use of XCurses over curses],,)
    mp_save_LIBS="$LIBS"
   CURSES_LIB=""
   if test "$with_ncurses" != yes -a "$with_xcurses" != yes
   then
     AC_CACHE_CHECK([for working curses], mp_cv_curses,
       [LIBS="$LIBS -lcurses"
        AC_TRY_LINK(
          [#include <curses.h>],
          [chtype a; int b=A_STANDOUT, c=KEY_LEFT; initscr(); ],
          mp_cv_curses=yes, mp_cv_curses=no)])
     if test "$mp_cv_curses" = yes
     then
       AC_DEFINE(HAVE_CURSES_H, , [Use the header file curses.h])
       CURSES_LIB="-lcurses"
     fi
   fi
   if test ! "$CURSES_LIB" -a "$with_xcurses" != yes
   then
     AC_CACHE_CHECK([for working ncurses], mp_cv_ncurses,
       [LIBS="$mp_save_LIBS -lncurses"
        AC_TRY_LINK(
          [#include <ncurses.h>],
          [chtype a; int b=A_STANDOUT, c=KEY_LEFT; initscr(); ],
          mp_cv_ncurses=yes, mp_cv_ncurses=no)])
     if test "$mp_cv_ncurses" = yes
     then
       AC_DEFINE(HAVE_NCURSES_H, , [Use the header file ncurses.h])
       CURSES_LIB="-lncurses"
     fi
   fi
   if test ! "$CURSES_LIB" -a "$with_ncurses" != yes
   then
     xcurses_deplibs="-L$x_libraries -lXaw -lXmu -lXt -lX11 -lSM -lICE -lXext"
     AC_CACHE_CHECK([for working XCurses], mp_cv_xcurses,
       [LIBS="$mp_save_LIBS -lXCurses $xcurses_deplibs"
        AC_TRY_LINK(
          [#include <xcurses.h>],
          [chtype a; int b=A_STANDOUT, c=KEY_LEFT; initscr(); ],
          mp_cv_xcurses=yes, mp_cv_xcurses=no)])
     if test "$mp_cv_xcurses" = yes
     then
       AC_DEFINE(HAVE_XCURSES_H, , [Use the header file xcurses.h])
       CURSES_LIB="-lXCurses $xcurses_deplibs"
     fi
   fi
   LIBS="$mp_save_LIBS"
])dnl
