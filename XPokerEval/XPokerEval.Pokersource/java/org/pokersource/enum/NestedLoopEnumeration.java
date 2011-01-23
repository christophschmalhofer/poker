// $Id: NestedLoopEnumeration.java,v 1.5 2002/06/19 08:21:31 mjmaurer Exp $

package org.pokersource.enum;
import java.util.Enumeration;

/** An iterator that steps through an abritrary number of nested loops, each
    starting at zero and ending at its own upper limit.  Useful when the
    number of nested loops is known only at runtime.  Example: if the
    limits are set to {2, 3, 2}, then the iterator will return the following
    values:
<pre>
	{0, 0, 0}
	{0, 0, 1}
	{0, 1, 0}
	{0, 1, 1}
	{0, 2, 0}
	{0, 2, 1}
	{1, 0, 0}
	{1, 0, 1}
	{1, 1, 0}
	{1, 1, 1}
	{1, 2, 0}
	{1, 2, 1}
</pre>
    @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class NestedLoopEnumeration implements Enumeration {
  private int[] next;       // our private copy of the next element
  private int[] elem;       // a copy of the next element we return in next()
  private int[] limits;     // loop limits for each dimension

  /** Initializes a nested loop iterator with limits.length dimensions.
      @param limits limits[i] is the upper limit of the ith nested loop (the
      loop runs from 0 to limits[i]-1 inclusive)
  */
  public NestedLoopEnumeration(int[] limits) {
    this.limits = limits;
    next = new int[limits.length];
    elem = new int[limits.length];
    for (int i=0; i<limits.length; i++) {
      if (limits[i] <= 0)
        throw new IllegalArgumentException("limits must be positive");
      next[i] = 0;
    }
  }

  public boolean hasMoreElements() {
    return (next != null);
  }

  /** Return an integer array listing the next loop indices for each
      dimension.
      @return An object of int[] type; the ith value is the loop variable
      for the ith nested loop.
  */
  public Object nextElement() {
    if (next == null)
      return null;
    for (int i=0; i<next.length; i++)
      elem[i] = next[i];
    next[limits.length-1]++;    // increment least-significant dimension
    for (int i=limits.length-1; i>=0; i--) { // check for carries
      if (next[i] == limits[i]) { // need to carry
        next[i] = 0;            // rollover to zero
        if (i>0)
          next[i-1]++;          // carry to next higher dimension
        else
          next = null;          // no more elements
      }
    }
    return elem;
  }
}
