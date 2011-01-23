#
# Copyright (C) 2004-2006 Mekensleep
#
# Mekensleep
# 24 rue vieille du temple
# 75004 Paris
#       licensing@mekensleep.com
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
# Authors:
#  Loic Dachary <loic@dachary.org>
# 

# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.53)
AC_INIT(poker-eval, 134.0)
AC_CONFIG_AUX_DIR(config)
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()
AM_CONFIG_HEADER([include/config.h include/poker_config.h])
AC_CONFIG_SRCDIR([lib/deck.c])

# Checks for programs.
AC_PROG_MAKE_SET
AC_GNU_SOURCE
AC_PROG_CPP
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PATH_PROG([AWK], [awk])
AC_PATH_PROG([MD5SUM], [md5sum])
AC_PATH_PROG([VALGRIND], [valgrind])

#
# GCC specific debug and optimization flags
#
AC_ARG_ENABLE(debug,
    [  --enable-debug          build in debug mode (the default is to build in optimized mode)],
    [enable_debug=yes])

if test "$GCC" = "yes" -a "$ac_test_CFLAGS" != set ; then
  if test "$enable_debug" = "yes" ; then
    CFLAGS="-pipe -g -Wall -Wpointer-arith -Wstrict-prototypes"
  else
    CFLAGS="-pipe -O3 -Wall -Wpointer-arith -Wstrict-prototypes"
  fi
fi

AC_EXEEXT
AC_OBJEXT
AC_ISC_POSIX
AC_HEADER_STDC
AC_C_INLINE

AC_C_CONST
AC_C_INLINE
AC_C_BIGENDIAN
AC_CHECK_SIZEOF(long)
AC_CHECK_HEADERS(unistd.h)
AC_CHECK_HEADERS(sys/stat.h)
AC_CHECK_TYPES([long long, uint64_t, int8])

AM_PATH_CCACHE
AM_GCOV

# Generate files
AC_CONFIG_FILES([tests/run], [chmod 544 tests/run])
AC_CONFIG_FILES([Makefile
	         poker-eval.pc
		 poker-eval.spec
                 lib/Makefile
                 include/Makefile
                 examples/Makefile
                 tests/Makefile
	 ])
AC_OUTPUT
