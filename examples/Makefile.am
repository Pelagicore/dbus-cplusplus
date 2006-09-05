EXTRA_DIST = README

INCLUDES = -I$(top_srcdir)/include 

noinst_PROGRAMS = echo_server

echo_server_SOURCES = server.cpp server.h
echo_server_LDADD = $(top_builddir)/src/libdbus-c++-0.3.la
