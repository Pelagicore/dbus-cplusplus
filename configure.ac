# Autojunk script for libdbus-c++

AC_PREREQ(2.59)
AC_INIT([libdbus-c++], 0.5.0, [shackan@gmail.com])

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER([include/dbus-c++/config.h])

AC_CANONICAL_HOST


# Set configuration options

AC_ARG_ENABLE(debug,
	AS_HELP_STRING([--enable-debug],
		[enable debugging support]),
	enable_debug=$enableval,enable_debug=auto
)

AC_ARG_ENABLE(glib,
	AS_HELP_STRING([--enable-glib],
		[enable glib integration]),
	enable_glib=$enableval,enable_debug=auto
)

AC_ARG_ENABLE(doxygen-docs,
	AS_HELP_STRING([--enable-doxygen-docs],
		[build DOXYGEN documentation (requires Doxygen)]),
	enable_doxygen_docs=$enableval,enable_doxygen_docs=auto
)

# Check for programs

AC_LANG_CPLUSPLUS

AC_PROG_CC
AC_PROG_CXX

CXX_FOR_BUILD=${CXX_FOR_BUILD-${CXX}}
AC_SUBST(CXX_FOR_BUILD)

AM_PROG_LIBTOOL

PKG_PROG_PKG_CONFIG


# Check for dependencies

DBUS_REQUIRED_VERSION=0.60
PKG_CHECK_MODULES([dbus], dbus-1 >= $DBUS_REQUIRED_VERSION,,
	AC_MSG_ERROR([You need the DBus libraries (version 0.6 or better)]
	[http://www.freedesktop.org/wiki/Software_2fdbus])
)
AC_SUBST(dbus_CFLAGS)
AC_SUBST(dbus_LIBS)

DBUS_API_STABLE_VERSION=1.0.0
PKG_CHECK_EXISTS(dbus-1 < $DBUS_API_STABLE_VERSION,
	AC_DEFINE(DBUS_API_SUBJECT_TO_CHANGE, , [unstable DBus])
)

DBUS_RECURSIVE_MUTEX_VERSION=0.95
PKG_CHECK_EXISTS(dbus-1 >= $DBUS_RECURSIVE_MUTEX_VERSION,
	AC_DEFINE(DBUS_HAS_RECURSIVE_MUTEX, , [DBus supports recursive mutexes (needs DBus >= 0.95)])
)

if test x$enable_glib = xyes ; then
PKG_CHECK_MODULES([glib], glib-2.0)
AC_SUBST(glib_CFLAGS)
AC_SUBST(glib_LIBS)
AM_CONDITIONAL(ENABLE_GLIB, test 1 = 1)
PKG_CHECK_MODULES([gtkmm], gtkmm-2.4,
	AM_CONDITIONAL(HAVE_GTKMM, test 1 = 1),
	AM_CONDITIONAL(HAVE_GTKMM, test 0 = 1)
)
AC_SUBST(gtkmm_CFLAGS)
AC_SUBST(gtkmm_LIBS)
else
AM_CONDITIONAL(ENABLE_GLIB, test 0 = 1)
AM_CONDITIONAL(HAVE_GTKMM, test 0 = 1)
fi

AC_CHECK_LIB([expat], XML_ParserCreate_MM,
    [ AC_CHECK_HEADERS(expat.h, have_expat=true, have_expat=false) ],
	have_expat=false)

if ! $have_expat; then
	AC_MSG_ERROR([You need the eXpat xml parser]
		[http://expat.sourceforge.net/])
fi

xml_CFLAGS=
xml_LIBS=-lexpat

AC_SUBST(xml_CFLAGS)
AC_SUBST(xml_LIBS)

if test x$enable_debug = xyes ; then
	CXXFLAGS="-Wall -ggdb -O0 -DDEBUG"
else
	CXXFLAGS="-Wall -O3"
fi


# Doxygen Documentation

AC_PATH_PROG(DOXYGEN, doxygen, no)

AC_MSG_CHECKING([whether to build Doxygen documentation])

if test x$DOXYGEN = xno ; then
    have_doxygen=no
else
    have_doxygen=yes
fi

if test x$enable_doxygen_docs = xauto ; then
    enable_doxygen_docs=no
fi

if test x$enable_doxygen_docs = xyes ; then
    if test x$have_doxygen = xno; then
	AC_MSG_ERROR([Building Doxygen docs explicitly required, but Doxygen not found])
    fi
fi

AM_CONDITIONAL(DBUS_DOXYGEN_DOCS_ENABLED, test x$enable_doxygen_docs = xyes)
AC_MSG_RESULT(yes)


# For the tools/, we need libdbus-c++ for the "build" architecture as well

AM_CONDITIONAL(CROSS_COMPILING, test $cross_compiling = yes)

AC_ARG_WITH(build-libdbus-cxx,
    AS_HELP_STRING([--with-build-libdbus-cxx],[For cross compilation: path to
    libdbus-cxx which was compiled for the 'build' system.]),
    [ BUILD_LIBDBUS_CXX_DIR=${withval} ],
    [ BUILD_LIBDBUS_CXX_DIR="\$(top_builddir)" ]
)
AC_SUBST(BUILD_LIBDBUS_CXX_DIR)


# Save processed files

AC_OUTPUT(
	Makefile
	src/Makefile
	tools/Makefile
	data/Makefile
	doc/Makefile
	doc/Doxyfile
	examples/Makefile
	examples/properties/Makefile
	examples/echo/Makefile
	examples/hal/Makefile
	examples/glib/Makefile
	dbus-c++-1.pc
)
