import PSim.*;

class PSimTest
{  
        public static void main(String args[])
        {
			System.out.println("This is a test of the PSim java interface.");
			System.out.println("We'll calculate some hands, and then do a benchmark.");

			
			//Setup our storage for the results.
			//A SimResults just stores things like win, tie, lose, etc.
			SimResults r = new SimResults();
			
			//A PostFlopState stores the state of our hand. This includes things like how many hole cards we're using,
			//wheter a flush draw is possible, etc.
			PostFlopState p = new PostFlopState();
			
			
			//Define some hands we'll evaluate.
			//Note that currently PSim's entire hand interface is done with strings.
			String hand1 = "Ah Ad   7s 8c Kd";
			String hand2 = "Ah Kd   Qh Th Jh";
			String hand3 = "2s 7c   7s 9h 8s 9s";
			
			
			//Write some stats for some hands.
			PSim.SimulateHand(hand1, r); //Get the results for the simulation.
			PSim.GetHandState(hand1, p); //Get the state of the hand.
			System.out.println("");
			System.out.println("Consider the hand: " + hand1);
			System.out.println(PSim.GetHandStateBrief(p));
			System.out.println("We have " + p.FlushOuts + " outs to a flush.");
			System.out.println("This hand will beat " + (r.win * 100) + "% of opponents right now.");
			System.out.println("This hand will beat " + (r.winSd * 100) + "% of opponents at showdown.");
			System.out.println("This hand has a " + (r.d94 * 100) + "% chance of being 94% strong at showdown.");
			System.out.println("This hand's raw hand rank is: " + PSim.RankHand(hand1) + ".");
			System.out.println("We did " + r.evaluations + " evaluations to enumerate this.");
			System.out.println("");
			
			
			//Write some stats for some hands.
			PSim.SimulateHand(hand2, r); //Get the results for the simulation.
			PSim.GetHandState(hand2, p); //Get the state of the hand.
			System.out.println("");
			System.out.println("Consider the hand: " + hand2);
			System.out.println(PSim.GetHandStateBrief(p));
			System.out.println("We have " + p.FlushOuts + " outs to a flush.");
			System.out.println("This hand will beat " + (r.win * 100) + "% of opponents right now.");
			System.out.println("This hand will beat " + (r.winSd * 100) + "% of opponents at showdown.");
			System.out.println("This hand has a " + (r.d94 * 100) + "% chance of being 94% strong at showdown.");
			System.out.println("This hand's raw hand rank is: " + PSim.RankHand(hand2) + ".");
			System.out.println("We did " + r.evaluations + " evaluations to enumerate this.");
			System.out.println("");
			
			
			//Write some stats for some hands.
			PSim.SimulateHand(hand3, r); //Get the results for the simulation.
			PSim.GetHandState(hand3, p); //Get the state of the hand.
			System.out.println("");
			System.out.println("Consider the hand: " + hand3);
			System.out.println(PSim.GetHandStateBrief(p));
			System.out.println("We have " + p.FlushOuts + " outs to a flush.");
			System.out.println("This hand will beat " + (r.win * 100) + "% of opponents right now.");
			System.out.println("This hand will beat " + (r.winSd * 100) + "% of opponents at showdown.");
			System.out.println("This hand has a " + (r.d94 * 100) + "% chance of being 94% strong at showdown.");
			System.out.println("This hand's raw hand rank is: " + PSim.RankHand(hand3) + ".");
			System.out.println("We did " + r.evaluations + " evaluations to enumerate this.");
			System.out.println("");
			
			
			//The hand to benchmark.
			String bhand = "Ah 9s";
			
			System.out.println("");
			System.out.println("Now we will benchmark with the hand: " + bhand);
			
			double time = System.currentTimeMillis();
			
			PSim.SimulateHand(bhand, r);
			
			time = (System.currentTimeMillis() - time);
			double handsPerSec = r.evaluations / (time / 1000);
			
			System.out.println("We took " + (time / 1000) + " seconds to do " + r.evaluations + " evaluations.");
			System.out.println(" --- Hands per second: " + handsPerSec);
			System.out.println("This hand will beat " + (r.winSd * 100) + "% of opponents at showdown.");
			System.out.println("");
			


			//The number of boards to sample.
			int boards = 200;
			
			System.out.println("");
			System.out.println("Finally, we will do the same evaluation with Monte Carlo sampling.");
			System.out.println("We'll consider all opponents' hands, but only " + boards + " random boards.");
			
			time = System.currentTimeMillis();
			
			//The 0 and 1 means that we consider all opponent hands. You could put .8 and 1 to only consider the top
			//20% of hands for what your opponent may hold. If boards is set to zero a full evaluation is done.
			
			PSim.SimulateHand(bhand, r, 0, 1, boards);
			
			time = (System.currentTimeMillis() - time);
			handsPerSec = r.evaluations / (time / 1000);
			
			System.out.println("We took " + (time / 1000) + " seconds to do " + r.evaluations + " evaluations.");
			System.out.println(" --- Hands per second: " + handsPerSec);
			System.out.println("This hand will beat " + (r.winSd * 100) + "% of opponents at showdown.");
			System.out.println("");			
			
			
			
			
			System.out.println("");
			System.out.println("Now we are going to do an enumeration in java.");
			System.out.println("It'll be the same way that indiana from pokerai.org did his benchmarks.");
			int[] hand = new int[7];
			long sum = 0;
			int evals = 0;
			time = System.currentTimeMillis();
			for (int h1 = 0; h1 < 13; h1++)
			for (int h2 = 0; h2 < 13; h2++)
			for (int h3 = 0; h3 < 13; h3++)
			for (int h4 = 0; h4 < 13; h4++)
			for (int h5 = 0; h5 < 13; h5++) if (h5 != h2)
			for (int h6 = 0; h6 < 13; h6++) if (h6 != h3)
			for (int h7 = 0; h7 < 13; h7++) if (h7 != h4)
			{
				hand[0] = h1;
				hand[1] = 13 + h2;
				hand[2] = 26 + h3;
				hand[3] = 39 + h4;
				hand[4] = 13 + h5;
				hand[5] = 26 + h6;
				hand[6] = 39 + h7;

				evals++;
				sum += PSim.RankHand(hand);
			}
			
			time = (System.currentTimeMillis() - time);
			handsPerSec = evals / (time / 1000);
			
			System.out.println("We took " + (time / 1000) + " seconds to do " + evals + " evaluations.");
			System.out.println(" --- Hands per second: " + handsPerSec);
			System.out.println("");			





			//Now we are just going to double check that the int interface works as advertised.
			System.out.println("");
			boolean error = false;
			
			hand[0] = 0;
			hand[1] = 1;
			hand[2] = 2;
			hand[3] = 3;
			hand[4] = 4;
			hand[5] = 5;
			hand[6] = 6;
			error = error || (PSim.RankHand(hand) != PSim.RankHand("2c 3c  4c 5c 6c 7c 8c"));
			error = error || (PSim.RankHand(hand) != PSim.RankHand("Ad Ah  4c 5c 6c 7c 8c"));
			error = error || (PSim.RankHand(hand) != PSim.RankHand("As 7c  4c 5c 6c 2c 8c"));

			hand[0] = 12;
			hand[1] = 11;
			hand[2] = 20;
			hand[3] = 45;
			hand[4] = 8;
			hand[5] = -1;
			hand[6] = -1;
			error = error || (PSim.RankHand(hand) != PSim.RankHand("AhKh  9d8sTh"));			
			
			
			if (error)
				System.out.println("The Int[] interface is broken!");
			else
				System.out.println("The Int[] interface appears to work.");





			System.out.println("");
			System.out.println("");
			System.out.println("THE END.");
		}
}
