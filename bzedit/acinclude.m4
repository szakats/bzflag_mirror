# configure paths for GtkGLArea 2.0
# David Trowbridge 2002-27-8

# AM_PATH_GTKGL_2_0([ACTION-IF-FOUND [,ACTION-IF-NOT-FOUND]])
AC_DEFUN(AM_PATH_GTKGL_2_0,
[
AC_REQUIRE([AM_PATH_GTK_2_0])
AC_PROVIDE([AM_PATH_GTKGL_2_0])

AC_ARG_WITH(gl-prefix,    [  --with-gl-prefix=PFX   Prefix where OpenGL or Mesa is installed],
 gl_prefix="$withval",
 gl_prefix="")

AC_ARG_WITH(gtkgl-prefix, [  --with-gtkgl-prefix=PFX Prefix where GtkGLArea is installed],
 gtkgl_prefix="$withval",
 gtkgl_prefix="")


if test x$gl_prefix != x ; then
 GL_CFLAGS="-I$gl_prefix/include"
 GL_LDOPTS="-L$gl_prefix/lib"
else
 GL_CFLAGS=""
 GL_LDOPTS=""
fi

saved_LIBS="$LIBS"
saved_CFLAGS="$CFLAGS"

# test for plain OpenGL
AC_MSG_CHECKING([for GL])
LIBS="$saved_LIBS $GTK_LIBS $GL_LDOPTS -lGLU -lGL"
AC_TRY_LINK( ,[ char glBegin(); glBegin(); ], have_GL=yes, have_GL=no)
AC_MSG_RESULT([$have_GL])
if test x$have_GL = xyes; then
 GL_LIBS="-lGLU -lGL"
else
 # test for Mesa
 AC_MSG_CHECKING([for Mesa])
 LIBS="$saved_LIBS $GTK_LIBS $GL_LDOPTS -lMesaGLU -lMesaGL"
 AC_TRY_LINK( ,[ char glBegin(); glBegin(); ], have_Mesa=yes, have_Mesa=no)
 AC_MSG_RESULT([$have_Mesa])
 if test x$have_Mesa = xyes; then
  GL_LIBS="-lMesaGLU -lMesaGL"
 else
  # test for Mesa with threads
  AC_MSG_CHECKING([for Mesa with pthreads])
  LIBS="$saved_LIBS $GTK_LIBS $GL_LDOPTS -lMesaGLU -lMesaGL -lpthread"
  AC_TRY_LINK( ,[ char glBegin(); glBegin(); ], have_Mesa_pthread=yes, have_Mesa_pthread=no)
  AC_MSG_RESULT([$have_Mesa_pthread])
  if test x$have_Mesa_pthread = xyes; then
   GL_LIBS="-lMesaGLU -lMesaGL -lpthread";
  else
   # all failed
   LIBS="$saved_LIBS"
   CFLAGS="$saved_CFLAGS"
   GTKGL_LIBS=""
   GTKGL_CFLAGS=""
   ifelse([$2], , :, [$2])

  fi
 fi
fi

# PKG_CONFIG is already defined, from the GTK check

if test x$gtkgl_prefix != x; then
 GTKGL_CFLAGS="-I$gtkgl_prefix/include"
 GTKGL_LDOPTS="-L$gtkgl_prefix/lib"
else
 GTKGL_CFLAGS="`$PKG_CONFIG --cflags gtkgl-2.0`"
 GTKGL_LDOPTS="`$PKG_CONFIG --libs gtkgl-2.0`"
fi

AC_MSG_CHECKING([for GtkGLArea])
LIBS="$saved_LIBS $GTKGL_LDOPTS $GL_LDOPTS $GL_LIBS $GTK_LIBS"
AC_TRY_LINK( ,[ char gtk_gl_area_new(); gtk_gl_area_new(); ], have_gtkgl=yes, have_gtkgl=no)
AC_MSG_RESULT([$have_gtkgl])

if test x$have_gtkgl = xyes; then
 LIBS="$saved_LIBS"
 CFLAGS="$saved_CFLAGS"
 GTKGL_CFLAGS="$GTKGL_CFLAGS $GL_CFLAGS"
 GTKGL_LIBS="$GTKGL_LDOPTS $GL_LDOPTS $GL_LIBS"
 ifelse([$1], , :, [$1])
 
else
 LIBS="$saved_LIBS"
 CFLAGS="$saved_CFLAGS"
 GTKGL_LIBS=""
 GTKGL_CFLAGS=""
 ifelse([$2], , :, [$2])

fi

AC_SUBST(GTKGL_CFLAGS)
AC_SUBST(GTKGL_LIBS)

])

