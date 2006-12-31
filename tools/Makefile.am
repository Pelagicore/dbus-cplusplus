AM_CPPFLAGS = \
	$(dbus_CFLAGS) \
	-I$(top_srcdir)/include \
	-DDBUS_API_SUBJECT_TO_CHANGE

bin_PROGRAMS = dbusxx-xml2cpp dbusxx-introspect

dbusxx_xml2cpp_SOURCES = xml2cpp.h xml2cpp.cpp
dbusxx_xml2cpp_LDADD = $(dbus_LIBS) $(top_builddir)/src/libdbus-c++-1.la

dbusxx_introspect_SOURCES = introspect.h introspect.cpp
dbusxx_introspect_LDADD = $(top_builddir)/src/libdbus-c++-1.la

MAINTAINERCLEANFILES = \
	Makefile.in
