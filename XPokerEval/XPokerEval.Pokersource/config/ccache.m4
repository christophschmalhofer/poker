# AM_PATH_CCACHE
#
# Use ccache (http://ccache.samba.org) to boost compilation.

AC_DEFUN([AM_PATH_CCACHE],
[
AC_ARG_WITH( ccache,
    [  --with-ccache           use ccache for compiling (default=auto)],
    [ using_ccache=$with_ccache ]
)

AC_PATH_PROG(CCACHE, ccache)
if test "$CCACHE" -a "$using_ccache" != "no"
then
    CC="ccache $CC"
fi
])
