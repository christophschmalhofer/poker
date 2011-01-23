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
EXTRA_DIST = \
	config/ccache.m4 \
	config/gcov.m4 \
	examples/Makefile.dos \
	include/Makefile.dos \
	java/Makefile.dos \
	lib/Makefile.dos \
	tests/Makefile.dos \
	$(wildcard *.vcproj) \
	poker-eval.sln \
	WHATS-HERE.Java \
	WHATS-HERE \
	$(wildcard gentoo/dev-games/poker-eval/*.ebuild) \
	$(wildcard csharp/*.cs) \
	csharp/Makefile \
	packaging-farm.bat \
	poker-eval.nsi \
	make_master.sh \
	fink/libpoker-eval.info

SUBDIRS = \
	lib \
	include \
	tests \
	examples

MAINTAINERCLEANFILES = INSTALL Makefile.in aclocal.m4 configure \
	$(filter-out %.m4,$(wildcard config/[a-z]*)) 

pkgconfigdir = ${libdir}/pkgconfig
pkgconfig_DATA = poker-eval.pc

ACLOCAL_AMFLAGS = -I config
