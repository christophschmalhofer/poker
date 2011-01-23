using System;
using System.Collections.Generic;
using System.Text;

namespace PokerCalculator
{
    public static partial class PokerLib
    {
        private static readonly short[] STRAIGHTS = new short[7937];

        /// <summary>
        /// Replaces the Macro from the c version - returns the rank of a card
        /// </summary>
        /// <param name="card">The card to get the rank from</param>
        /// <returns>integer rank of the card</returns>
        public static int RANK(int card)
        {
            return ((card >> 8) & 0xF);
        }

        /// <summary>
        ///  perform a binary search on a pre-sorted array
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        public static int findit(int key)
        {
            int low = 0, high = 4887, mid;

            while (low <= high)
            {
                mid = (high + low) >> 1;      // divide by two
                if (key < products[mid])
                    high = mid - 1;
                else if (key > products[mid])
                    low = mid + 1;
                else
                    return (mid);
            }
            
            throw new ApplicationException("no match found; key = " + key);
        }

        /// <summary>
        ///   This routine initializes the deck.  A deck of cards is
        ///   simply an integer array of length 52 (no jokers).  This
        ///   array is populated with each card, using the following
        ///   scheme:
        ///
        ///   An integer is made up of four bytes.  The high-order
        ///   bytes are used to hold the rank bit pattern, whereas
        ///   the low-order bytes hold the suit/rank/prime value
        ///   of the card.
        ///
        ///   +--------+--------+--------+--------+
        ///   |xxxbbbbb|bbbbbbbb|cdhsrrrr|xxpppppp|
        ///   +--------+--------+--------+--------+
        ///
        ///   p = prime number of rank (deuce=2,trey=3,four=5,five=7,...,ace=41)
        ///   r = rank of card (deuce=0,trey=1,four=2,five=3,...,ace=12)
        ///   cdhs = suit of card
        ///   b = bit turned on depending on rank of card
        /// </summary>
        /// <param name="deck"></param>
        public static void init_deck(int[] deck)
        {
            int i, j, n = 0, suit = 0x8000;

            for (i = 0; i < 4; i++, suit >>= 1)
                for (j = 0; j < 13; j++, n++)
                    deck[n] = primes[j] | (j << 8) | suit | (1 << (16 + j));
        }


        /// <summary>
        ///  This routine will search a deck for a specific card
        ///  (specified by rank/suit), and return the INDEX giving
        ///  the position of the found card.  If it is not found,
        ///  then it returns -1
        /// </summary>
        /// <param name="rank">the rank of the card to find</param>
        /// <param name="suit">the suit of the cards to find</param>
        /// <param name="deck">the deck of cards to search</param>
        /// <returns>the index of card or -1 if not found</returns>
        public static int find_card(int rank, int suit, int[] deck)
        {
            int i, c;

            for (i = 0; i < 52; i++)
            {
                c = deck[i];
                if ((c & suit)!=0 && (RANK(c) == rank))
                    return (i);
            }
            return (-1);
        }

        /// <summary>
        //  This routine takes a deck and randomly mixes up
        //  the order of the cards.
        /// </summary>
        /// <param name="deck">The deck to shuffle</param>
        public static void shuffle_deck(int[] deck)
        {
            Random r = new Random();

            int i, n;
            int[] temp = new int[52];

            for ( i = 0; i < 52; i++ )
                temp[i] = deck[i];

            for (i = 0; i < 52; i++)
            {
                do
                {
                    n = r.Next(1, 52); //(51.9999999 * drand48());

                } while (temp[n] == 0);

                deck[i] = temp[n];
                temp[n] = 0;
            }
        }

        private static readonly char[] rank = new char[]{'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
        public static string print_hand( int[] hand, int n )
        {
            string returnValue = string.Empty;

            int i, r;
            char suit;
            for ( i = 0; i < n; i++ )
            {
                r = (hand[i] >> 8) & 0xF;
                if (( hand[i] & 0x8000 )== 0x8000)
                    suit = 'c';
                else if (( hand[i] & 0x4000 )==0x4000)
                    suit = 'd';
                else if (( hand[i] & 0x2000 ) == 0x200)
                    suit = 'h';
                else
                    suit = 's';
                
                returnValue += rank[r] + suit + " ";
            }

            return returnValue;
        }


        public static int hand_rank( ushort val )
        {
            if (val > 6185) return(HIGH_CARD);        // 1277 high card
            if (val > 3325) return(ONE_PAIR);         // 2860 one pair
            if (val > 2467) return(TWO_PAIR);         //  858 two pair
            if (val > 1609) return(THREE_OF_A_KIND);  //  858 three-kind
            if (val > 1599) return(STRAIGHT);         //   10 straights
            if (val > 322)  return(FLUSH);            // 1277 flushes
            if (val > 166)  return(FULL_HOUSE);       //  156 full house
            if (val > 10)   return(FOUR_OF_A_KIND);   //  156 four-kind
            return(STRAIGHT_FLUSH);                   //   10 straight-flushes
        }

        public static ushort eval_5cards(int c1, int c2, int c3, int c4, int c5)
        {
            int q;
            ushort s;

            q = (c1 | c2 | c3 | c4 | c5) >> 16;

            /* check for Flushes and StraightFlushes
            */
            if ((c1 & c2 & c3 & c4 & c5 & 0xF000)!=0)
                return (flushes[q]);

            /* check for Straights and HighCard hands
            */
            s = unique5[q];
            if (s!=0) return (s);

            /* let's do it the hard way
            */
            q = (c1 & 0xFF) * (c2 & 0xFF) * (c3 & 0xFF) * (c4 & 0xFF) * (c5 & 0xFF);
            q = findit(q);

            return (values[q]);
        }

        public static ushort eval_5hand(int[] hand)
        {
            int c1, c2, c3, c4, c5;

            c1 = hand[0];
            c2 = hand[1];
            c3 = hand[2];
            c4 = hand[3];
            c5 = hand[4];

            return (eval_5cards(c1, c2, c3, c4, c5));
        }

        // This is a non-optimized method of determining the
        // best five-card hand possible out of seven cards.
        // I am working on a faster algorithm.
        //
        public static ushort eval_7hand(int[] hand)
        {
            int i, j;
            ushort q, best = 9999;
            int[] subhand = new int[5];

	        for ( i = 0; i < 21; i++ )
	        {       
		        for ( j = 0; j < 5; j++ )
			    subhand[j] = hand[ perm7[i,j] ];
		        q = eval_5hand( subhand );
		        if ( q < best )
			        best = q;
	        }
	        
            return( best );
        }
    }
}
