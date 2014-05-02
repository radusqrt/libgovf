NULL =

lib_LTLIBRARIES = libgovf-1.0.la

libgovf_1_0_ladir = $(includedir)/libgovf-1.0/govf
libgovf_1_0_la_HEADERS = \
			govf.h \
			govf-importer.h
libgovf_1_0_la_SOURCES = \
			$(libgovf_1_0_la_HEADERS) \
			govf-importer.c
libgovf_1_0_la_CFLAGS = \
			-DLIBVIRT_GLIB_BUILD \
			-DG_LOG_DOMAIN="\"GOVF\"" \
			-DLOCALEDIR="\"$(datadir)/locale\"" \
			$(COVERAGE_CFLAGS) \
			-I$(top_srcdir) \
			$(GOVF_CFLAGS) \
			$(WARN_CFLAGS)
libgovf_1_0_la_LIBADD = $(GOVF_LIBS)

-include $(INTROSPECTION_MAKEFILE)

GOVF-1.0.gir: libgovf-1.0.la
GOVF_1_0_gir_INCLUDES = libxml2-2.0 GObject-2.0 LibvirtGObject-1.0
GOVF_1_0_gir_LIBS = libgovf-1.0.la
GOVF_1_0_gir_FILES = $(libgovf_1_0_la_SOURCES)
GOVF_1_0_gir_CFLAGS = $(libgovf_1_0_la_CFLAGS)
GOVF_1_0_gir_SCANNERFLAGS = \
	--identifier-prefix=GOVF \
	--symbol-prefix=govf \
	--c-include="govf/govf.h" \
	$(NULL)
GOVF_1_0_gir_NAMESPACE = GOVF
GOVF_1_0_gir_VERSION = 1.0

INTROSPECTION_GIRS = GOVF-1.0.gir

girdir = $(datadir)/gir-1.0
gir_DATA = GOVF-1.0.gir
typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(gir_DATA:.gir=.typelib)

if WITH_VALA
vapidir = $(datadir)/vala/vapi
vapi_DATA = govf-1.0.vapi

govf-1.0.vapi: GOVF-1.0.gir
	$(AM_V_GEN)$(VAPIGEN)							 \
		--vapidir=$(builddir)						 \
		--pkg gobject-2.0						 \
		--pkg gio-2.0							 \
		--pkg libvirt-gobject-1.0					 \
		--library govf-1.0						 \
		--metadatadir=$(srcdir)						 \
		$<

endif

CLEANFILES =		\
	$(gir_DATA)     \
	$(typelib_DATA)	\
	govf-1.0.vapi	\
	$(NULL)

EXTRA_DIST = govf-1.0.deps
