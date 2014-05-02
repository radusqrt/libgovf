AM_CFLAGS = $(GOVF_CFLAGS) \
	    -DPKGDATADIR="\"$(pkgdatadir)\"" \
	    -DLOCALEDIR="\"$(datadir)/locale\"" \
	    -I$(top_srcdir) \
            $(NULL)

bin_PROGRAMS = govf-import

govf_import_SOURCES = govf-import.c
govf_import_LDADD = $(GOVF_LIBS) 	     \
		    $(top_builddir)/govf/libgovf-1.0.la
