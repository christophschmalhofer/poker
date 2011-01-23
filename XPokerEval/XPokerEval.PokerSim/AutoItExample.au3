$PostFlopState = DllStructCreate("uint a; uint b")

$result = DllCall("psim.dll", "none", "GetHandState", "str", "8h Td  5c 6c 7c", "ptr", DllStructGetPtr($PostFlopState))
if BitAnd(DllStructGetData($PostFlopState, "a"), 2 ^ 16) then
	MsgBox(0, "state", "You have over cards!")
else
	MsgBox(0, "state", "You do not have over cards.")
endif

MsgBox(0, "state", "Straight outs: " & BitShift(BitAnd(DllStructGetData($PostFlopState, "a"), 2^28 + 2^29 + 2^30 + 2^31), 28))

$result = DllCall("psim.dll", "str", "GetHandStateBrief", "ptr", DllStructGetPtr($PostFlopState))
MsgBox(0, "state brief", $result[0])


;PSim is a 32-bit dll, so you'll want to run this script with "Run Script (x86)".

$dll = DllOpen ("psim.dll") ;The path needs to be set here.

if $dll <> -1 then
   $hand = "AhKh Td5hJh";
   
   ;Test this out with RankHand, it just takes a string and returns an unsigned int. Easy.
   $result = DllCall($dll, "uint", "RankHand", "str", $hand)
   if @error <> 0 then
      MsgBox(0, "DllCall Error", @error)
   else
      MsgBox(0, "RankHand", "Your hand " & $hand & " ranks to " & $result[0] & ".");
   endif
   
   ;Now go for the real function: SimulateHand.
   $simResultsStruct = "float win; float tie; float lose; float winSd; float tieSd; float loseSd; float d94; float d90; int evaluations"
   $simResults = DllStructCreate($simResultsStruct)

   $result = DllCall($dll, "none", "SimulateHand", "str", $hand, "ptr", DllStructGetPtr($simResults), "float", 0, "float", 1, "uint", 0)

   if @error <> 0 then
      MsgBox(0, "DllCall Error", @error)
   else
      MsgBox(0, "Simulation Results", "Your hand " & $hand & " will win at showdown " & DllStructGetData($simResults, "winSd") * 100 & "% of the time.")
   endif
   
   DllClose($dll)
else
   MsgBox(0, "error", "Couldn't load the DLL.")
endif