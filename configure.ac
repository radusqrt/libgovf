AC_INIT(govf, 0.0.1)
AC_CONFIG_SRCDIR(govf/govf.h)
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
dnl Make automake keep quiet about wildcards & other GNUmake-isms
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([1.11 tar-ustar no-dist-gzip dist-xz])
AM_MAINTAINER_MODE([enable])

AM_SILENT_RULES([yes])

GLIB2_REQUIRED=2.35.1
LIBVIRT_GOBJECT_REQUIRED=0.1.9
GOBJECT_INTROSPECTION_REQUIRED=0.10.8
LIBXML2_REQUIRED=2.0.0

GOVF_MAJOR_VERSION=`echo $VERSION | awk -F. '{print $1}'`
GOVF_MINOR_VERSION=`echo $VERSION | awk -F. '{print $2}'`
GOVF_MICRO_VERSION=`echo $VERSION | awk -F. '{print $3}'`
GOVF_VERSION=$GOVF_MAJOR_VERSION.$GOVF_MINOR_VERSION.$GOVF_MICRO_VERSION$GOVF_MICRO_VERSION_SUFFIX
GOVF_VERSION_NUMBER=`expr $GOVF_MAJOR_VERSION \* 1000000 + $GOVF_MINOR_VERSION \* 1000 + $GOVF_MICRO_VERSION`

# In libtool terminology we need to figure out:
#
# CURRENT
#     The most recent interface number that this library implements.
#
# REVISION
#     The implementation number of the CURRENT interface.
#
# AGE
#     The difference between the newest and oldest interfaces that this
#     library implements.
#
# In other words, the library implements all the interface numbers
# in the range from number `CURRENT - AGE' to `CURRENT'.
#
# Libtool assigns the soname version from `CURRENT - AGE', and we
# don't want that to ever change in govf, unless we need
# to break ABI for some rason.
#
# We would, however, like the govf version number reflected
# in the so version'd symlinks, and this is based on AGE.REVISION
# eg  libgovf.so.0.AGE.REVISION
#
# Assuming we do ever want to break soname version, this can
# toggled. But seriously, don't ever touch this.
GOVF_SONUM=0

# The following examples show what libtool will do
#
# Input: 0.9.14 ->   libgovf.so.0.9.14
# Input: 1.0.0  ->   libgovf.so.0.1000.0
# Input: 2.5.8  ->   libgovf.so.0.2005.8
#
AGE=`expr $GOVF_MAJOR_VERSION '*' 1000 + $GOVF_MINOR_VERSION`
REVISION=$GOVF_MICRO_VERSION
CURRENT=`expr $GOVF_SONUM + $AGE`
GOVF_VERSION_INFO=$CURRENT:$REVISION:$AGE

AC_SUBST([GOVF_MAJOR_VERSION])
AC_SUBST([GOVF_MINOR_VERSION])
AC_SUBST([GOVF_MICRO_VERSION])
AC_SUBST([GOVF_VERSION])
AC_SUBST([GOVF_VERSION_INFO])
AC_SUBST([GOVF_VERSION_NUMBER])

AC_PROG_CC

# Initialize libtool
LT_PREREQ([2.2])
LT_INIT

PKG_CHECK_MODULES(GOVF,
                  gobject-2.0 >= $GLIB2_REQUIRED \
                  gio-2.0 >= $GLIB2_REQUIRED \
                  libxml-2.0 >= $LIBXML2_REQUIRED \
                  libvirt-gobject-1.0 >= $LIBVIRT_GOBJECT_REQUIRED)

GTK_DOC_CHECK([1.10],[--flavour no-tmpl])

# Setup GLIB_MKENUMS to use glib-mkenums even if GLib is uninstalled.
GLIB_MKENUMS=`$PKG_CONFIG --variable=glib_mkenums glib-2.0`
AC_SUBST(GLIB_MKENUMS)

GOBJECT_INTROSPECTION_CHECK([$GOBJECT_INTROSPECTION_REQUIRED])

AC_ARG_ENABLE([vala],
              AS_HELP_STRING([--enable-vala], [enable Vala binding generation]),
              [], [enable_vala=check])
if test "x$enable_introspection" = "xyes" ; then
        if test "x$enable_vala" != "xno" ; then
                AC_PATH_PROG(VAPIGEN, vapigen, no)
                if test "x$VAPIGEN" == "xno"; then
                        if test "x$enable_vala" == "xcheck" ; then
                                enable_vala=no
                        else
                                AC_MSG_ERROR([Cannot find the "vapigen" binary in your PATH])
                        fi
                fi
                AC_SUBST(VAPIGEN)
        fi
        if test "x$enable_vala" != "xno" ; then
                AC_MSG_CHECKING([$VAPIGEN support for --metadatadir])
                $VAPIGEN --help | grep metadatadir 1>/dev/null 2>&1
                if test $? != 0 ; then
                        if test "x$enable_vala" == "xcheck" ; then
                                enable_vala=no
                                AC_MSG_RESULT([no])
                        else
                                AC_MSG_ERROR(["vapigen" binary in your PATH is too old (< 0.13)])
                        fi
                else
                        AC_MSG_RESULT([no])
                fi
        fi
        if test "x$enable_vala" = "xcheck" ; then
                enable_vala=yes
        fi
fi
AM_CONDITIONAL([WITH_VALA], [test "x$enable_vala" = "xyes"])

AC_OUTPUT(Makefile
          govf/Makefile
          tools/Makefile
          govf-1.0.pc)
