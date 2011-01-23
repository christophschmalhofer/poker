using System;
using XPokerEval.PokerSource.KeithRule;

namespace XPokerEval.PokerSource.KeithRule.Test
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Keith Rule's Evaluator (C# Port of Pokersource)");
            Console.WriteLine("-----------------------------------------------\n");
            Console.WriteLine("Enumerating and evaluating all X unique 7-card poker hands...\n");

            int c0, c1, c2, c3, c4, c5, c6;
            int count = 0;
            int [] freq = new int[10];
            for (int i = 0; i < 10; i++)
                freq[i] = 0;
            
            ulong [] deck = new ulong[52];
            ulong [] virginDeck = new ulong[52];
            for (int c = 0; c < 52; c++)
                virginDeck[c] = HoldemHand.Hand.Mask(c);// StdDeck_MASK(c);

            ulong h0, h1, h2, h3, h4, h5, h6;

            int timeTaken = System.Environment.TickCount;

            for (c0 = 0; c0 < 46; c0++)
            {
                h0 = virginDeck[c0];
                for (c1 = c0 + 1; c1 < 47; c1++)
                {
                    //h1.cards_n = h0.cards_n | virginDeck[c1].cards_n;
                    h1 = h0 | virginDeck[c1];
                    for (c2 = c1 + 1; c2 < 48; c2++)
                    {
                        //h2.cards_n = h1.cards_n | virginDeck[c2].cards_n;
                        h2 = h1 | virginDeck[c2];
                        for (c3 = c2 + 1; c3 < 49; c3++)
                        {
                            //h3.cards_n = h2.cards_n | virginDeck[c3].cards_n;
                            h3 = h2 | virginDeck[c3];
                            for (c4 = c3 + 1; c4 < 50; c4++)
                            {
                                //h4.cards_n = h3.cards_n | virginDeck[c4].cards_n;
                                h4 = h3 | virginDeck[c4];
                                for (c5 = c4 + 1; c5 < 51; c5++)
                                {
                                    //h5.cards_n = h4.cards_n | virginDeck[c5].cards_n;
                                    h5 = h4 | virginDeck[c5];
                                    for (c6 = c5 + 1; c6 < 52; c6++)
                                    {
                                        h6 = h5 | virginDeck[c6];

                                        //handTypeSum[HandVal_HANDTYPE(StdDeck_StdRules_EVAL_N(h6, 7)) + 1]++;
                                        HoldemHand.Hand.HandTypes ht = HoldemHand.Hand.EvaluateType(h6, 7);
                                        freq[(int)ht]++;

                                        count++;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            timeTaken = System.Environment.TickCount - timeTaken;

            Console.WriteLine("No Pair:           {0:N0}", freq[0]);
            Console.WriteLine("One Pair:          {0:N0}", freq[1]);
            Console.WriteLine("Two Pair:          {0:N0}", freq[2]);
            Console.WriteLine("Three of a Kind:   {0:N0}", freq[3]);
            Console.WriteLine("Straight:          {0:N0}", freq[4]);
            Console.WriteLine("Flush:             {0:N0}", freq[5]);
            Console.WriteLine("Full House:        {0:N0}", freq[6]);
            Console.WriteLine("Four of a Kind:    {0:N0}", freq[7]);
            Console.WriteLine("StraightFlush:     {0:N0}", freq[8]);
            //Console.WriteLine("BAD!:              {0:N0}\n", freq[0]);

            Console.WriteLine("\nEnumerated and evaluated {0} hands in {1} milliseconds!", count, timeTaken);

            // Pocket Aces, with a board of 2d 3d 4d 5d Ac (A2345 straight flush)
            HoldemHand.Hand hand = new HoldemHand.Hand("Ah Ad", "2d 3d 4d 5d Ac");
            uint handVal = hand.HandValue;

            // Worst possible 7-card hand:
            HoldemHand.Hand badHand = new HoldemHand.Hand("2c 3d", "4h 5s 7d 8c 9h");
            uint worstHandVal = badHand.HandValue;

            // 2nd-Worst possible 7-card hand:
            HoldemHand.Hand badHand2 = new HoldemHand.Hand("2c 3d", "4h 6s 7d 8c 9h");
            uint worstHandVal2 = badHand2.HandValue;

            string[] pockets = { "AdAh", "Tc2c" };
            long[] wins = new long[2];
            long[] ties = new long[2];
            long[] losses = new long[2];
            long totalHands = 0;

            // Compare the equity of AhAd vs. Tc2c preflop
            Console.WriteLine("Evaluating [AhAd] vs. [Tc2c] preflop...");
            HoldemHand.Hand.HandOdds(pockets, "", "AdAh2cTc", wins, ties, losses, ref totalHands);
            Console.WriteLine(String.Format("{0} hands enumerated.", totalHands));
            Console.WriteLine("Hand    Wins   Losses   Ties");
            Console.WriteLine(String.Format("{0}     {1}    {2}      {3}", pockets[0], wins[0], losses[0], ties[0]));
            Console.WriteLine(String.Format("{0}     {1}    {2}      {3}", pockets[1], wins[1], losses[1], ties[1]));

            // Compare the equity of AhAd vs. Tc2c with an AcKcQc flop
            Console.WriteLine("\nEvaluating [AhAd] vs. [Tc2c] with [AcKcQc] flop");
            HoldemHand.Hand.HandOdds(pockets, "QcKcAc", "QcKcAcAdAh2cTc", wins, ties, losses, ref totalHands);
            Console.WriteLine(String.Format("{0} hands enumerated.", totalHands));
            Console.WriteLine("Hand    Wins   Losses   Ties");
            Console.WriteLine(String.Format("{0}     {1}    {2}      {3}", pockets[0], wins[0], losses[0], ties[0]));
            Console.WriteLine(String.Format("{0}     {1}    {2}      {3}", pockets[1], wins[1], losses[1], ties[1]));

            Console.ReadLine();


        }
    }
}
