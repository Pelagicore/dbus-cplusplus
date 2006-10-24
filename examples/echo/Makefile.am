EXTRA_DIST = README echo-introspect.xml

INCLUDES = -I$(top_srcdir)/include 

noinst_PROGRAMS = echo-server

echo_server_SOURCES = echo-glue.h echo-server.h echo-server.cpp
echo_server_LDADD = $(top_builddir)/src/libdbus-c++-0.3.la

echo-glue.h: echo-introspect.xml
	$(top_builddir)/tools/dbusxx-xml2cpp $^ --adaptor=$@

BUILT_SOURCES = echo-glue.h
CLEANFILES = $(BUILT_SOURCES)
