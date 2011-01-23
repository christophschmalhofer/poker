INCORRECT BEHAVIOR
------------------
Lowball evaluation in eval_low.h is broken for certain 6- and 7-card hands, 
such as full house plus pair, and quads plus trips.

MISSING FUNCTIONALITY
---------------------
Joker decks and Asian stud decks are not supported in pokenum, thanks to
inconvenient design of the deck and card mask types.  These types and
their accompanying macros and functions need a reorganization.

NICE TO HAVE
------------
There is no makedepend support, so make doesn't know to recompile a file when
an include file changes.  And there are lots of includes!

The 'make all' and 'make test' process hasn't been tested lately on Windows
and is probably broken.

The autoconf/configure tool is not being used to create the shared libraries
in a platform-independent way.  The current configure.in hard-codes the
commands to build *.a, *.so, and *.dll.  Building *.a works on any unix
system, but building the *.so now only works with gcc.
