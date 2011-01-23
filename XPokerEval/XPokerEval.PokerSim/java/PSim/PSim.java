package PSim;

public class PSim
{
	//A wrapper for the PSim DLL by Timmy.
	//-Timmy


	//The imported functions taking a string.
	public native static void GetHandState(String hand, PostFlopState results);
	public native static int RankHand(String hand);
	public native static void SimulateHand(String hand, SimResults results, float lowRange, float highRange, int boards);
	public native static void SimulateHandMulti(String hand, SimResults results, int boards, int hands, int numOpponents);


	//The imported functions taking an int[].
	public native static void GetHandState(int[] hand, PostFlopState results);
	public native static int RankHand(int[] hand);
	public native static void SimulateHand(int[] hand, SimResults results, float lowRange, float highRange, int boards);
	public native static void SimulateHandMulti(int[] hand, SimResults results, int boards, int hands, int numOpponents);


	public native static String GetHandStateBrief(PostFlopState results);


	//Some easier to call functions with defaults. Feel free to add more and post them online.
	public static void SimulateHand(String hand, SimResults results)
	{
		SimulateHand(hand, results, 0, 1, 0);
	}
	

	static
	{
		System.loadLibrary("psjw"); //The special java DLL name, if you rename the DLL change this.
	}
}
