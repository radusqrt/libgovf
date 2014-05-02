ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

SUBDIRS = govf tools

pkgconfig_DATA= govf-1.0.pc
pkgconfigdir = $(libdir)/pkgconfig

EXTRA_DIST = govf-1.0.pc.in govf-1.0-uninstalled.pc.in

DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc --enable-introspection

# Extra clean files so that maintainer-clean removes *everything*
MAINTAINERCLEANFILES = aclocal.m4 compile config.guess config.sub configure depcomp install-sh ltmain.sh Makefile.in missing config.h.in
