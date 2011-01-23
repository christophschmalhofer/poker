using System;
using XPokerEval.CactusKev.CSharp;

namespace XPokerEval.CactusKev.CSharp.Test
{
    class Program
    {
        static void Main(string[] args)
        {
            int a, b, c, d, e, j;
            ushort i;
            int [] deck= new int[52];
            int [] hand = new int[5];
            int [] freq = new int[10];

            // Init the deck in the Cactus Kev format..
            PokerCalculator.PokerLib.init_deck(deck);

            Console.WriteLine("Cactus Kev's Hand Evaluator in C#, by Mike Benson");
            Console.WriteLine("-------------------------------------------------\n");
            Console.WriteLine("Enumerating and evaluating all 2,598,960 unique 5-card poker hands...\n");

            int timeTaken = Environment.TickCount;

            // loop over every possible five-card hand
            for(a=0;a<48;a++) {
                hand[0] = deck[a];
                for(b=a+1;b<49;b++) {
                    hand[1] = deck[b];
                    for(c=b+1;c<50;c++) {
                        hand[2] = deck[c];
                        for(d=c+1;d<51;d++) {
                            hand[3] = deck[d];
                            for(e=d+1;e<52;e++) {
                                hand[4] = deck[e];

                                i = PokerCalculator.PokerLib.eval_5hand_fast(hand);
                                j = PokerCalculator.PokerLib.hand_rank(i);
                                freq[j]++;
                            }
                        }
                    }
                }
            }

            timeTaken = Environment.TickCount - timeTaken;

            Console.WriteLine("No Pair:           {0:N0}", freq[9]);
            Console.WriteLine("One Pair:          {0:N0}", freq[8]);
            Console.WriteLine("Two Pair:          {0:N0}", freq[7]);
            Console.WriteLine("Three of a Kind:   {0:N0}", freq[6]);
            Console.WriteLine("Straight:          {0:N0}", freq[5]);
            Console.WriteLine("Flush:             {0:N0}", freq[4]);
            Console.WriteLine("Full House:        {0:N0}", freq[3]);
            Console.WriteLine("Four of a Kind:    {0:N0}", freq[2]);
            Console.WriteLine("StraightFlush:     {0:N0}", freq[1]);
            Console.WriteLine("BAD!:              {0:N0}\n", freq[0]);

            int totalHands = 0;
            for (int cat = 0; cat < 10; cat++)
                totalHands += freq[cat];
            
            Console.WriteLine("Enumerated and evaluated {0:N0} hands in {1} milliseconds.", totalHands, timeTaken);

            Console.ReadLine();
        }
    }
}
