BECAUSE OF LICENSING PROBLEMS (IBM LCP and Apache are not compatible with the
GNU GPL), the Java part is not included in the distribution. 

The PokerSource library includes an optional Java layer.  You can compile the
C library on its own, or you can compile both the C library and the Java
layer.  In order to compile the Java layer, you will need the following
packages.  Jar files are provided as part of the distribution for your
convenience; you can also download the sources or jar files yourself.
    Apache Oro regular expressions (org.apache.oro.*) [tested with v2.0.6]
        http://jakarta.apache.org/oro/
        See the file LICENSE.oro for license details.  
        This product includes software developed by the
        Apache Software Foundation (http://www.apache.org/).
    JUnit test framework (junit.framework.*) [tested with v3.7]
        http://sourceforge.net/projects/junit/
        See the file LICENSE.junit for license details.
    Gnu Getopt (gnu.getopt.*) [tested with v1.0.9]
        http://www.gnu.org/software/java/packages.html
        See the file LICENSE.getopt for license details.

The Java code is documented in the java/javadoc directory.

After you have a working build, you may want to:
* add java/pokersource.jar to your CLASSPATH environment variable
* add java/jakarta-oro-2.0.6.jar to your CLASSPATH environment variable
* add java/junit-3.7.jar to your CLASSPATH environment variable
* add java/gnu-getopt-1.0.9.jar to your CLASSPATH environment variable

Who is the target audience?  

There are Java Native Interface (JNI) wrappers around some of the
library functions.  There are some demo programs which illustrate the
features of the library, but which are not (at this time) flexible
enough to be used without modification by end-users except for the
most basic of simulations.

What _exactly_ is in here?  

Evaluators: 

  See also the Java JNI wrappers around these evaluation functions (more
  info in java/javadoc).

Enumerators:

  Also see the Java JNI wrappers around the enumerate.c functions (more
  info in java/javadoc).
    enumExhaustive()
    enumSample()

  A first draft of a subjective all-in equity (SAIE) calculator is now
  part of the Java package (for more info see java/javadoc).  An example
  SAIE calculation is to compute the pot equity of "Qh Jh" against the
  set of "Sklansky/Malmulth group 1-2 hands" when the board is "Qd 4h 2s".

Example programs:

  saie:   A script wrapping the java-based SAIE calculator.  See the script
          itself for suggested usage.
