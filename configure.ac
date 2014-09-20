AC_CONFIG_MACRO_DIR([aclocal])

dnl Bookkeeping
AC_PREREQ(2.69)

dnl Initialize autoconf and automake
AC_INIT([PROJECT], [0.0.0])
AC_SUBST([PACKAGE_NAME])dnl

AM_INIT_AUTOMAKE([subdir-objects])
AM_SILENT_RULES([yes])
AC_CONFIG_HEADERS([defs.h])

dnl Various required programs
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_LIBTOOL_DLOPEN
AC_DISABLE_STATIC
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

dnl Header checking
AC_HEADER_STDC
AC_CHECK_HEADERS([libintl.h limits.h stdlib.h])
AC_CHECK_HEADERS([sys/types.h])

dnl Typedef checking
AC_CHECK_TYPES([int8_t,  int16_t,  int32_t,  int64_t,
                u_int8_t, u_int16_t, u_int32_t, u_int64_t],,
                AC_MSG_ERROR([portable type not found.]))

dnl Library function checking
AC_FUNC_MALLOC
AC_CHECK_FUNCS([getopt_long])

dnl Miscellaneousness
AC_C_BIGENDIAN

dnl Flags
AC_ARG_ENABLE(debug, [  --enable-debug          enable debugging output [[default=no]]],
[
    if test $enableval = "yes";
     then test -z "$CFLAGS" && CFLAGS="-O0 -g3 -Wall -Werror"
     else CFLAGS="$CFLAGS -DNDEBUG"
    fi
], [
    CFLAGS="$CFLAGS -DNDEBUG"
])

dnl Libraries
AC_LANG(C++)
AX_CHECK_LIBRARY([LIBLEAP], [Leap.h], [Leap], [],
                 [AC_MSG_ERROR([Leap.h or libLeap.so not found.  Did you install the LeapMotion SDK?])])

dnl Massive hack for Qt config stuff
AC_ARG_WITH([qt5],[  --with-qt5=/path/to/qmake  Enable Qt5 tray icon/gui (default: check)],
            [qmake_path=$withval],[qmake_path=yes])
req_qt_ver=5.3.0

dnl Qt Part 1
if test "x$qmake_path" != "xno"; then
   AC_MSG_CHECKING([for Qt])

   if test "x$qmake_path" = "xyes"; then
      qmake_path=`which qmake`
   fi

   if test "x$qmake_path" = "x" || test ! -x $qmake_path; then
      AC_MSG_RESULT([no, qmake path "$qmake_path" not executable (did you specify one?)])
   else
      moc=`dirname $qmake_path`/moc
      if test ! -x $moc; then
         AC_MSG_RESULT([no, can't find $moc])
      else
         qtver=`$qmake_path -v 2>&1 | sed 's/.*Qt version \(\([[0-9]]\{1,\}\.\{0,\}\)\{0,\}\).*/\1/;t;D'`
         AX_COMPARE_VERSION([$qtver],[ge],[$req_qt_ver],
                [AC_MSG_RESULT([yes, $qtver])
                has_qt=yes
                QMAKE=$qmake_path
                MOC=$moc
                RCC=`dirname $qmake_path`/rcc
                UIC=`dirname $qmake_path`/uic
                AC_DEFINE([HAVE_QT],[1],[Define to 1 if Qt5 is present.])],
                [AC_MSG_RESULT([no (need >= $req_qt_ver, found $qtver)])])
      fi         
   fi
fi

AC_SUBST(QMAKE)
AC_SUBST(MOC)
AC_SUBST(RCC)
AC_SUBST(UIC)
AM_CONDITIONAL([WITH_QT],[test x$has_qt != x])

dnl Qt Part 2
# Huge hack to get Qt includes etc
if test "x$has_qt" != "x"; then
   oldir=`pwd`
   cd `dirname $0`

   rm -f qt.pro qt.mk
   $QMAKE -project -recursive -o qt.pro
   $QMAKE -makefile -recursive -o qt.mk qt.pro

   qtdefines=`cat qt.mk | sed 's/^DEFINES *= \(.*\)/\1/;t;D'`
   qtcflags=`cat qt.mk | sed 's/^CFLAGS *= \(.*\)/\1/;t;D'`
   qtinclude=`cat qt.mk | sed 's/^INCPATH *= \(.*\)/\1/;t;D'`
   qtlibs=`cat qt.mk | sed 's/^LIBS *= \(.*\)/\1/;t;D'`

   qtcflags=`echo $qtcflags | sed "s/\\$(DEFINES)/$qtdefines/"`

   QT_CFLAGS=$qtcflags
   QT_DEFINES=$qtdefines
   QT_INCLUDE=$qtinclude
   QT_LIBS=$qtlibs

   cd $oldir
fi

AC_SUBST([QT_CFLAGS])
AC_SUBST([QT_DEFINES])
AC_SUBST([QT_INCLUDE])
AC_SUBST([QT_LIBS])

dnl And output
AC_CONFIG_FILES([Makefile src/include/Makefile src/Makefile src/test/Makefile 
               ])

AC_OUTPUT

