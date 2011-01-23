#
# Copyright (C) 2004-2006 Mekensleep
#
# Mekensleep
# 24 rue vieille du temple
# 75004 Paris
#       licensing@mekensleep.com
#
#  This package is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; version 2 dated June, 1991.
#
#  This package is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this package; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
#  MA 02110-1301, USA.
#
# Authors:
#  Loic Dachary <loic@dachary.org>
#
# 
MAINTAINERCLEANFILES = Makefile.in

INCLUDES = -I$(top_srcdir)/include

EXTRA_DIST = run.in

TESTS_ENVIRONMENT = ./run

TESTS = ${check_PROGRAMS} gcov-report

.PHONY: gcov-report

check_PROGRAMS = \
	joktest1 \
	enumtest1 \
	enumtest2 \
	enumtest3 \
	enumtest5 \
	enumtest7 \
	poker_wrapper

joktest1_SOURCES = joktest1.c
joktest1_LDADD = $(top_builddir)/lib/libpoker-eval.la
joktest1_CPPFLAGS = ${GCOV_CPPFLAGS}
joktest1_LDFLAGS = ${GCOV_LDFLAGS}

enumtest1_SOURCES = enumtest1.c
enumtest1_LDADD = $(top_builddir)/lib/libpoker-eval.la
enumtest1_CPPFLAGS = ${GCOV_CPPFLAGS}
enumtest1_LDFLAGS = ${GCOV_LDFLAGS}

enumtest2_SOURCES = enumtest2.c
enumtest2_LDADD = $(top_builddir)/lib/libpoker-eval.la
enumtest2_CPPFLAGS = ${GCOV_CPPFLAGS}
enumtest2_LDFLAGS = ${GCOV_LDFLAGS}

enumtest3_SOURCES = enumtest3.c
enumtest3_LDADD = $(top_builddir)/lib/libpoker-eval.la
enumtest3_CPPFLAGS = ${GCOV_CPPFLAGS}
enumtest3_LDFLAGS = ${GCOV_LDFLAGS}

enumtest5_SOURCES = enumtest5.c
enumtest5_LDADD = $(top_builddir)/lib/libpoker-eval.la
enumtest5_CPPFLAGS = ${GCOV_CPPFLAGS}
enumtest5_LDFLAGS = ${GCOV_LDFLAGS}

enumtest7_SOURCES = enumtest7.c
enumtest7_LDADD = $(top_builddir)/lib/libpoker-eval.la
enumtest7_CPPFLAGS = ${GCOV_CPPFLAGS}
enumtest7_LDFLAGS = ${GCOV_LDFLAGS}

poker_wrapper_SOURCES = poker_wrapper.c
poker_wrapper_LDADD = $(top_builddir)/lib/libpoker-eval.la
poker_wrapper_CPPFLAGS = ${GCOV_CPPFLAGS}
poker_wrapper_LDFLAGS = ${GCOV_LDFLAGS}

clean-local:
	-rm -f *.gcov *.gcda *.gcno
