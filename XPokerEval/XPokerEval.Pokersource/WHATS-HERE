What is this package?  

This package is a free (GNU GPL) toolkit for writing programs which
simulate or analyze poker games.


Who is the target audience?  

Mostly programmers.  The programs are all written in C, but there is
a binding for Python. A binding for Java also exists but is not
shipped with the distribution because it requires maintainance. It
can be found in the CVS tree. 


Where did it come from?  

Cliff Matthews wrote and released (under GPL) a fast poker hand
evaluator several years ago, and this code is derived from that work.
It has been improved over the years, with contributions from Mat
Hostetter, Keith Miyake, Brian Goetz, Michael Maurer, Tim Showalter,
Loic Dachary and others. This code is now
maintained by Loic Dachary <loic@dachary.org>, Tim Showalter <tjs@psaux.com>, 
Michael Maurer <mjmaurer@yahoo.com> and improvements and
suggestions can be sent to them.  The latest version of the software
can be found at http://gna.org/projects/pokersource.


What's in here?  

Lots of cool stuff, including: 
 - A general framework for writing program about poker (including
   nonstandard poker games, such as games with nonstandard decks, wild
   cards, etc)
 - several different types of hand evaluators for common games
   (straight poker, asian stud, lowball, omaha)
 - some sample programs which give a flavor for what can be done.

Why so many evaluators?

Partially historical, partially practical.  Different evaluators have
different characteristics (speed, memory footprint, degree of
generality), although over time the differences between them have
gotten smaller.  The original evaluator was straightforward and
general, but not as fast as it could be.  The exactly-{5,7}-cards
(EVAL_X5, EVAL_X7) evaluators were introduced to be much faster, but
less general and using much more memory.  However, subsequent
refinements of the general evaluator (EVAL_N) proved to be smaller,
general, and as fast as EVAL_X{5,7}.  There is also a fast, small
evaluator which only evaluates the hand type (EVAL_TYPE) and which
might be small enough (3K code / 8K data) to fit into L1-cache on many
processors.

There are also evaluators for nonstandard games and decks, including
asian stud, standard poker with a joker (bug), and lowball.  Others
will be added.  In addition, a more generalized framework for
evaluation, which will be slower but will allow for runtime
polymorphism, is in the works.

Most of the basic evaluators are provided in both C-inline versions
(for highest performance, since the call overhead is considerable) and
callable versions (which could theoretically be used from other
languages.)  


What else is cool about this package?

Lots of useful macros which make it easier to write poker programs
without compromising performance.  

Cool macro-based polymorphism that provides flexibility without
compromising performance.

Highly portable -- uses GNU Autoconf on Unix and can also be compiled
with Microsoft Visual C++.  


Where do I start?

Start by looking at the example programs in the examples/ directory (or
somewhere in the documentation directory of the development package).
In there, you will find several programs which do interesting and not
so interesting things, and you will likely find these to be useful
starting points for your own programs.


What _exactly_ is in here?  

Evaluators: There are several of them.  Evaluator names are composed
from the name of the deck type they use (StdDeck is the standard 52
card deck, JokerDeck is the 52 card deck with the bug, etc) and the
rule set they use (StdRules are the commonly used poker hand rules.)
Depending on what game you've selected, convenience macros are
provided so you can use shorter names; by default, Hand_EVAL_N is a
shortcut for StdDeck_StdRules_EVAL_N and CardMask is a shortcut for
StdDeck_CardMask.  

Evaluators return a 32-bit integer which can be compared using integer
operations; if eval(a) > eval(b), then according to the rules the
evaluator uses, then hand a beats hand b.  Similarly, if eval(a) ==
eval(b), then a ties b.  However, not all evaluators have the same
structure to their return type -- if you will be taking apart the
return type or doing anything other than comparing or printing the
result, you will need to use the appropriate set of macros for taking
apart the hand value.

Evaluators: 

  StdDeck_StdRules_EVAL_N: A fast inline evaluator, which takes as
          arguments a standard card mask and the number of cards in
          the hand.  It uses precomputed lookup tables for many
          operations.  Size: ~4K code, ~80K data.

  StdDeck_StdRules_EVAL_TYPE: A fast small inline evaluator which
          evaluates only the hand type.  Size: ~3K code, ~8K data.

  StdDeck_StdRules_EVAL_X{5,7}: A fast, much larger inline evaluator
          (uses many large lookup tables) which is only useful for
          evaluating hands of exactly 5/7 cards and is basically not
          human-readable.  Not as useful now that EVAL_N is so much
          faster than it used to be, although it is likely that these
          routines could be further improved to make them faster than
          EVAL_N.

  AStudDeck_AStudRules_EVAL_N: An inline evaluator for Asian Stud.  

  StdDeck_Lowball8_EVAL_N: An inline evaluator for 5 card lowball,
          with an implicit 8-or-better qualifier.  (When comparing hand
          values, remember that the _worst_ hand wins, so that for
          hand a to beat hand b, eval(a) will be LESS than eval(b).)

  StdDeck_Lowball_EVAL_N: An inline evaluator for 5 card lowball.  

  JokerDeck_Lowball_EVAL_N: An inline evaluator for lowball using a
          deck with a joker.

Enumerators:
  See enumerate.h for macros that enumerate every possible hand outcome
  and accumulate wins, losses, ties, and EV.  
    DECK_ENUMERATE_x_CARDS
    DECK_ENUMERATE_x_CARDS_D
    DECK_MONTECARLO_N_CARDS_D
    DECK_ENUMERATE_COMBINATIONS_D
    DECK_ENUMERATE_PERMUTATIONS_D
    DECK_MONTECARLO_PERMUTATIONS_D

  See enumerate.c for convenient functions that take advantage of these.

Example programs:

  five_card_hands: Enumerate all possible five card hands, evaluate them, 
          and tabulate the results by hand type.  Since this uses the 
          standard macros, by simply defining the right game type macros, 
          the program will enumerate the five card hands for that game 
          without any other modification.

  seven_card_hands: Same as above, but enumerates all seven card hands.  
          May take a long time on slow machines or with slow evaluators.  

  hcmp2:  Compares two hold-em hands against each other, at any point
          (before the flop, after the flop, on the turn) and tabulates
          the number of times each hand wins, loses, or ties, and
          tabulates all-in pot equity.

  hcmpn:  Similar to hcmp2, but for more than two players.  

  eval:   Evaluates a hand and prints out the value.  Useful for testing
          new evaluators.

  fish:   A new version of an old program that's been swimming around the
          net for a long time.  Given a number of cards to be dealt and an
          optional set of dead cards, compute how many ways there are to 
          make each type of hand.  The five_card_hands program above is
          equivalent to "fish -n 5".

  pokenum: Similar to hcmpn, but for more games than just holdem.  Also, uses
          higher level enumeration functions that wrap the low level
          enumeration macros.

  usedecks: Shows one way to use different decks and evaluation rules
          within the same program.
