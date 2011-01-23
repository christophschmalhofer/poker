Well, I'm going to make my simulation library open source. This started out as a static library for my own use, but I figured I could make it a DLL and help some folks out.

Anyway, here are the features.
-Written in C++
-Uses the poker-eval library for speed and low memory usage
-Exposes a raw hand rank method
-Includes enumeration functions and Monte Carlo functions
-Can tell you about your hand, such as pair, top pair, flush draw, can an opponent have a straight, etc
-Includes usage example in java
-Includes full source code, compiled DLL (for any real language), and compiled java DLL (uh, for java ;) )
-VERY simple interface
-Can be used for anything you want (although I would like if you gave me credit)

Here are usage examples to figure out what your odds of winning a showdown are for As 9d on a flop of 8c 2h Ks.
[code]
//C code:
SimResults r;
SimulateHand("As 9d     8c 2h Ks", r);
std::cout << "This hand will beat " << (r.winSd * 100) << "% of opponents at showdown.\n";

//Java code:
SimResults r = new SimResults();
PSim.SimulateHand("As 9d     8c 2h Ks", r);
System.out.println("This hand will beat " + (r.winSd * 100) + "% of opponents at showdown.");

;AutoIt3 code - calling psim.dll
$simResultsStruct = "float win; float tie; float lose; float winSd; float tieSd; float loseSd; float d94; float d90; int evaluations"
$simResults = DllStructCreate($simResultsStruct)
$result = DllCall("psim.dll", "none", "SimulateHand", "str", "As 9d     8c 2h Ks", "ptr", DllStructGetPtr($simResults), "float", 0, "float", 1, "uint", 0)
MsgBox(0, "Simulation Results", "This hand will beat " & DllStructGetData($simResults, "winSd") * 100 & "% of opponents at showdown.")
[/code]
Tell me that's not simple! As you can see, it interfaces all hands as text (hole cards, the board cards if any, all separated by spaces or tabs. This was natural for me, because I read dealer chat. If you don't like the test interface, it'd be pretty easy to change to the poker-eval hand interface.

Ok, so to get started, unzip and take a look at the psim.hpp file. It has most structures and functions commented pretty well. The SimResults structure and the SimulateHand() function are by far the two most important areas. Then hope on over to the java directory and fire up run.bat (you'll need java installed for this). Be aware that this'll run a benchmark, and may take a minute or two. Then, if you are a java programmer take a look at PSimTest.java. Everything you need to get started is there. If you use this for a bot you may want to rename the DLL (if you do change PSim.java line 28 too).

For C++ programmers I would recommend compiling the source as a static library. Note that you will have to link against the poker-eval code to do this. Just include psim.hpp in your program.

For AutoIt users, see the included example for information on calling psim.dll.

For programmers in other languages, just call the PSim.dll in the root folder (again, might want to rename the dll). The methods that are exported are all defined in psim.hpp. If you can't figure it out, let me know and I'll see if I can help.

I will probably be adding a lot to this in the future. I may add Alberta style positive potential stats in the future. I'm currently working on adding weight tables, but I'm not sure I'll release that to the public.

OK, please let me know what you guys think.