# Autojunk script for libdbus++

AC_PREREQ(2.59)
AC_INIT([libdbus-c++], 0.3.0, [shackan@gmail.com])

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER([include/dbus-c++/config.h])

AC_CANONICAL_HOST


# Set configuration options

AC_ARG_ENABLE(doxygen-docs,
	AS_HELP_STRING([--enable-doxygen-docs],
		[build DOXYGEN documentation (requires Doxygen)]),
	enable_doxygen_docs=$enableval,enable_doxygen_docs=auto
)


# Check for programs

AC_LANG_CPLUSPLUS

AC_PROG_CC
AC_PROG_CXX

#AM_ENABLE_STATIC
#AM_DISABLE_SHARED
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

CXXFLAGS="-ggdb -O0 -DDEBUG -Wall"
#CXXFLAGS="-O3 -DDEBUG -Wall"

AC_SUBST(DEBUG_FLAGS)


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

if test x$enable_doxygen_docs = xyes; then
    if test x$have_doxygen = xno; then
	AC_MSG_ERROR([Building Doxygen docs explicitly required, but Doxygen not found])
    fi
fi

AM_CONDITIONAL(DBUS_DOXYGEN_DOCS_ENABLED, test x$enable_doxygen_docs = xyes)
AC_MSG_RESULT(yes)

# Save processed files

AC_OUTPUT(
	Makefile
	src/Makefile
	tools/Makefile
	data/Makefile
	doc/Makefile
	doc/Doxyfile
	doc/html/Makefile
	examples/Makefile
	tests/Makefile
	dbus-c++-0.3.pc
)
