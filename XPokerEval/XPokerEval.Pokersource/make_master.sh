#!/bin/bash

BUILDDIR=$1
MASTERDIR=$2
MODULE=poker-eval
CONFIGURATION=Release

SRCDIR=${BUILDDIR}
DISTDIR=${MASTERDIR}
rm -fR ${DISTDIR}
mkdir -p ${DISTDIR}

rsync -avr ${SRCDIR}/include/ ${DISTDIR}/include/ --include='*.h' --include='inlines/' --include='inlines/*.h' --exclude='**'
rsync -av ${SRCDIR}/include/poker_config.h.in ${DISTDIR}/include/poker_config.h

LIBS=${CONFIGURATION}/poker-eval.lib
rsync -av ${SRCDIR}/${LIBS} ${DISTDIR}/lib/

DOCS="\
 ${SRCDIR}/NEWS\
 ${SRCDIR}/README\
 ${SRCDIR}/TODO\
 ${SRCDIR}/AUTHORS\
 ${SRCDIR}/WHATS-HERE\
 ${SRCDIR}/debian/copyright"
rsync -av $DOCS $DISTDIR/doc/
rsync -av ${SRCDIR}/ChangeLog $DISTDIR/doc/changelog

rsync -avr ${SRCDIR}/examples/ ${DISTDIR}/examples/ --include='*.vcproj' --include='*.c' --exclude='**'
