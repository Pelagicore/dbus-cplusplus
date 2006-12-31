EXTRA_DIST = README echo-introspect.xml

AM_CPPFLAGS = -I$(top_srcdir)/include

noinst_PROGRAMS = echo-server

echo_server_SOURCES = echo-glue.h echo-server.h echo-server.cpp
echo_server_LDADD = $(top_builddir)/src/libdbus-c++-1.la

echo-glue.h: echo-introspect.xml
	$(top_builddir)/tools/dbusxx-xml2cpp $^ --adaptor=$@

BUILT_SOURCES = echo-glue.h
CLEANFILES = $(BUILT_SOURCES)

dist-hook:
       cd $(distdir); rm -f $(BUILT_SOURCES)

MAINTAINERCLEANFILES = \
	Makefile.in
