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

EXTRA_DIST = utest1 getopt_w32.c getopt_w32.h $(wildcard *.vcproj) 

#TESTS = utest1

noinst_PROGRAMS = \
	eval \
	fish \
	five_card_hands \
	hcmp2 \
	hcmpn \
	pokenum \
	seven_card_hands \
	usedecks

eval_SOURCES = eval.c
eval_LDADD = $(top_builddir)/lib/libpoker-eval.la
eval_CPPFLAGS = ${GCOV_CPPFLAGS}
eval_LDFLAGS = ${GCOV_LDFLAGS}

fish_SOURCES = fish.c
fish_LDADD = $(top_builddir)/lib/libpoker-eval.la
fish_CPPFLAGS = ${GCOV_CPPFLAGS}
fish_LDFLAGS = ${GCOV_LDFLAGS}

five_card_hands_SOURCES = five_card_hands.c
five_card_hands_LDADD = $(top_builddir)/lib/libpoker-eval.la
five_card_hands_CPPFLAGS = ${GCOV_CPPFLAGS}
five_card_hands_LDFLAGS = ${GCOV_LDFLAGS}

hcmp2_SOURCES = hcmp2.c
hcmp2_LDADD = $(top_builddir)/lib/libpoker-eval.la
hcmp2_CPPFLAGS = ${GCOV_CPPFLAGS}
hcmp2_LDFLAGS = ${GCOV_LDFLAGS}

hcmpn_SOURCES = hcmpn.c
hcmpn_LDADD = $(top_builddir)/lib/libpoker-eval.la
hcmpn_CPPFLAGS = ${GCOV_CPPFLAGS}
hcmpn_LDFLAGS = ${GCOV_LDFLAGS}

pokenum_SOURCES = pokenum.c
pokenum_LDADD = $(top_builddir)/lib/libpoker-eval.la
pokenum_CPPFLAGS = ${GCOV_CPPFLAGS}
pokenum_LDFLAGS = ${GCOV_LDFLAGS}

seven_card_hands_SOURCES = seven_card_hands.c
seven_card_hands_LDADD = $(top_builddir)/lib/libpoker-eval.la
seven_card_hands_CPPFLAGS = ${GCOV_CPPFLAGS}
seven_card_hands_LDFLAGS = ${GCOV_LDFLAGS}

usedecks_SOURCES = usedecks.c
usedecks_LDADD = $(top_builddir)/lib/libpoker-eval.la
usedecks_CPPFLAGS = ${GCOV_CPPFLAGS}
usedecks_LDFLAGS = ${GCOV_LDFLAGS}

clean-local:
	-rm -f *.gcov *.gcda *.gcno
