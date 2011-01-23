#ifndef PSIM_HPP
#define PSIM_HPP

/* This program is free software. It comes without any warranty, to
* the extent permitted by applicable law. You can redistribute it
* and/or modify it under the terms of the Do What The Fuck You Want
* To Public License, Version 2, as published by Sam Hocevar. See
* http://sam.zoy.org/wtfpl/COPYING for more details. */


/// \file PSim is used to simulate hand strength, and return general info about a hand.
/** \file

    PSim
    (c) 2008 Timmy



    For the evaluation functions that take a hand, you can pass it in two different ways.
    In either case, at least the first two cards must be present (your hole cards).

    The first option would be to pass the hand as a string. Valid formats include "Ah Kh 9d 8s Th",
    "AhKh9d8sTh", "AhKh    9d8sTh", etc... This would mean that the player holds Ah Kh while the board
    contains 9d 8s Th.

    The second option to pass cards is as an array of exactly 7 integers. Each integer represents a card as
    0-12 2h-Ah, 13-25 2d-Ad, 26-38 2c-Ac, 39-51 2s-As, any other number means no card
    The hand in the above example would look like: int hand[7] = {12, 11, 20, 45, 8, -1, -1};
    Hands passed by integer will process slightly faster than hands passed by string (although both are very fast).
    This way of passing hands is most useful for preforming enumeration outside of PSim.



    Version History:

	March 2008 version 1.3
    Fixed bug in GetHandState() not counting outs correctly for already strong hands.
    Added IsStraightPossible and IsStraightDrawPossible to PostFlopState.
    Added the hand state stuff into the java interface. I do believe it's complete now.

    February 2008 version 1.2
    Fixed bug in SimulateHandMulti() allowing opponents to share crads. (typical error of ~ 3%)
    Removed the simulate draw function. Simulate hand already does 94%, 90%, and the current %.
	Now including compiled DLL again.
	Now including AutoIt example.

    February 2008 version 1.1
    Add int[] hand interfaces to all functions.
    Made the java stuff into a package (PSim).

    February 2008 version 1.0
    First public release
*/


///Comment out to not count the number of hands evaluated.
#define CountEvaluations


///Get our exports correct. For a shared library (recommended) don't do anything.
#ifdef BUILD_DLL
    #define EXPORT extern "C" __declspec(dllexport) __stdcall
#else
    #ifdef IMPORT_DLL
        #define EXPORT extern "C" __declspec(dllimport) __stdcall
    #else
        #define EXPORT
    #endif
#endif


///Simulation results for a hand.
struct SimResults
{
    ///What percent of hands will our hand beat right now? (enumerates/simulates all opponent cards)
    float win;
    float tie;
    float lose;

    ///What percent of hands will our hand beat at showdown? (enumerates/simulates all unknown board + opponent cards)
    float winSd;
    float tieSd;
    float loseSd;

    ///The chance that our hand will be at least as good as it is now by the showdown.
    float dNow;

    ///The chance that our hand will be able to beat or tie 94% of hands by the showdown.
    float d94;

    ///Same as D94, only 90.
    float d90;

    #ifdef CountEvaluations
        ///Will count the number of evaluations we ran for our simulation.
        int evaluations;
    #endif
};


///General info about a post flop hand.
struct PostFlopState
{
    ///Do we use both of our hole cards?
    bool UsesBoth: 1;
    bool UsesOne: 1;
    bool UsesNone: 1;

    ///What is our actual hand rank?
    bool IsNoPair: 1;
    bool IsOnePair: 1;
    bool IsTwoPair: 1;
    bool IsTrips: 1;
    bool IsStraight: 1;
    bool IsFlush: 1;
    bool IsFullHouse: 1;
    bool IsQuads: 1;
    bool IsStFlush: 1;

    bool IsTopPair: 1; ///<We paired the highest card one the board with one in our hand.
    bool IsMiddlePair: 1; ///<We paired the board, but it's not top or bottom pair. (may be 2nd best, 3rd best, etc.)
    bool IsBottomPair: 1; ///<We paired the lowest card one the board with one in our hand.
    bool IsOverPair: 1; ///<We have a pocket pair, and it's bigger than any card on the board.
    bool IsOverCards: 1; ///<We just have two nonpaired cards bigger than anything on the board.

    bool IsStraightDrawPossible: 1; ///<Could someone have 4 to a straight?
    bool IsStraightPossible: 1; ///Could someone have a straight?

    bool IsFlushDrawPossible: 1; ///<Could someone have 4 to a suit?
    bool IsFlushPossible: 1; ///<Could someone have 5 to a suit?

    bool IsFullHousePossible: 1; ///<Is a fullhouse or quads possible?

    bool Is3Flush: 1; ///<Do we have exactly 3 cards to a flush?
    bool Is4Flush: 1; ///<Do we have exactly 4 cards to a flush?

    unsigned int FlushOuts: 4; ///<How many cards will give us flush?
    unsigned int StraightOuts: 4; ///<How many cards will give us straight?
    unsigned int BetterOuts: 4; ///<How many cards will give us a fullhouse or better?
};


///Retuns some useful information about a hand.
EXPORT void GetHandState(const char* hand, PostFlopState* results);
void GetHandState(const int* hand, PostFlopState* results);


///Converts a hand state to a string telling us in plain english what we have.
EXPORT const char* GetHandStateBrief(const PostFlopState* state);


///Ranks a hand, a higher ranked hand beats any lower ranked hand.
EXPORT unsigned int RankHand(const char* hand);
unsigned int RankHand(const int* hand);


///Run ours simulations on a hand.
/**
Boards is the maximum number of boards to see with Monte Carlo simulation. If boards is 0,
or >= the total number of boards, a full enumeration is performed.
lowRange - highRange is the cards to evaluate for an oppenent. For example, setting low to .9 and high to 1
would only evaluate the best 10% of the starting hole cards for the opponent.
*/
EXPORT void SimulateHand(const char* hand, SimResults* results, float lowRange = 0, float highRange = 1, unsigned int boards = 0);
void SimulateHand(const int* hand, SimResults* results, float lowRange = 0, float highRange = 1, unsigned int boards = 0);


///Run ours simulations on a hand against mulitple opponents. (Currently doesn't figure current strength, only SD and draws)
/**
Boards is the number of boards to see with Monte Carlo simulation.
Hands is the number of hands to simulate per opponent per board.
numOpponents is the number of Opponents.
*/
EXPORT void SimulateHandMulti(const char* hand, SimResults* results, unsigned int boards = 2000, unsigned int hands = 500, unsigned int numOpponents = 9);
void SimulateHandMulti(const int* hand, SimResults* results, unsigned int boards = 2000, unsigned int hands = 500, unsigned int numOpponents = 9);

#endif
