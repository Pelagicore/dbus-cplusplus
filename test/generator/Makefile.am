EXTRA_DIST = dbuscxx_test_generator.xml

AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_builddir)/include 

noinst_PROGRAMS = dbuscxx_test_generator_server

dbuscxx_test_generator_server_SOURCES = dbuscxx_test_generator-server-glue.h \
					dbuscxx_test_generator-server.h \
					dbuscxx_test_generator-server.cpp

dbuscxx_test_generator_server_LDADD = $(top_builddir)/src/libdbus-c++-1.la @PTHREAD_LIBS@
dbuscxx_test_generator_server_CXXFLAGS = @PTHREAD_CFLAGS@

dbuscxx_test_generator-server-glue.h: dbuscxx_test_generator.xml
	$(top_builddir)/tools/dbusxx-xml2cpp $^ --adaptor=$@

######

noinst_PROGRAMS += dbuscxx_test_generator_client

dbuscxx_test_generator_client_SOURCES = dbuscxx_test_generator-client-glue.h \
					dbuscxx_test_generator-client.h \
					dbuscxx_test_generator-client.cpp

dbuscxx_test_generator_client_LDADD = $(top_builddir)/src/libdbus-c++-1.la @PTHREAD_LIBS@
dbuscxx_test_generator_client_CXXFLAGS = @PTHREAD_CFLAGS@

dbuscxx_test_generator-client-glue.h: dbuscxx_test_generator.xml
	$(top_builddir)/tools/dbusxx-xml2cpp $^ --proxy=$@

BUILT_SOURCES = dbuscxx_test_generator-server-glue.h \
		dbuscxx_test_generator-client-glue.h

CLEANFILES = $(BUILT_SOURCES)

dist-hook:
	cd $(distdir); rm -f $(BUILT_SOURCES)

MAINTAINERCLEANFILES = \
	Makefile.in
