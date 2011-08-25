#!/bin/sh
# Usage: sh -x ./autogen.sh [--libtoolize]
#
# Building Guile-SDL has been tested with the following toolset:
#  GNU Libtool 1.5.26
#  GNU Autoconf 2.61
#  GNU Automake 1.10.1
#  Guile 1.4.1.114
#  GNU gperf 3.0.2
#  GNU texinfo 4.11.94

[ -f src/misc-utils.scm ] || {
  echo "autogen.sh: run this command only in the guile-sdl directory."
  exit 1
}

set -e

######################################################################
# Guile-BAUX

guile-baux-tool import \
    c2x \
    forms-from \
    tsar \
    c-tsar \
    tsin \
    gbaux-do

######################################################################
# Libtool setup.

if [ x"$1" = x--libtoolize ] || [ ! -f ltmain.sh ] ; then
    libtoolize --force --automake
fi

######################################################################
# Invoke the auto* tools.

loc="-I `guile-config info datadir`/aclocal"
test "$loc" = "-I `aclocal --print-ac-dir`" && loc=
aclocal $loc -I build-aux
autoheader
autoconf
automake --add-missing

######################################################################
# modsup.h

cd src
ln -sf `guile-config info includedir`/guile/modsup.h

######################################################################
# Done.

: Now run configure and make.
: You must pass the "--enable-maintainer-mode" option to configure.

# autogen.sh ends here
