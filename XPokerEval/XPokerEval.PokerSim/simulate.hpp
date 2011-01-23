#ifndef SIMULATE_HPP
#define SIMULATE_HPP

#include "psim.hpp"

#include "poker_defs.h"
#include "enumerate.h"
#include "inlines/eval.h"
#include "inlines/eval_type.h"

namespace Simulate
{
    struct Hand
    {
        Hand(CardMask firstHoleCard, CardMask secondHoleCard, CardMask common, int size): firstHoleCard(firstHoleCard),
        secondHoleCard(secondHoleCard), common(common), size(size)
        {
            CardMask_OR(ours, firstHoleCard, secondHoleCard);
            CardMask_OR(dead, ours, common);
        }

        CardMask firstHoleCard, secondHoleCard;
        CardMask ours, common, dead;
        int size;
    };


    //--------------Functions that are exported are actually defined here, and take a Hand struct.
    ///Retuns some useful information about a hand.
    void GetHandState(const Hand& shand, PostFlopState* state);

    ///Converts a hand state to a string telling us in plain english what we have.
    const char* GetHandStateBrief(const PostFlopState* state);

    ///Ranks a hand, a higher ranked hand beats any lower ranked hand.
    unsigned int RankHand(const Hand& shand);

    ///Run ours simulations on a hand.
    void SimulateHand(const Hand& shand, SimResults* results, float lowRange, float highRange, unsigned int boards);

    ///Run ours simulations on a hand against mulitple opponents.
    void SimulateHandMulti(const Hand& shand, SimResults* results, unsigned int boards, unsigned int hands, unsigned int numOpponents);



    //-------------Helper functions that may be used by psim (or anyone, for that matter)
    ///Takes a card mask and returns an int with a bit set if that card rank exists (regardless of suit).
    uint32 CollaspeCardMask(CardMask cards);

    ///Zeros out a PostFlopState.
    void ZeroPostFlopState(PostFlopState* pfs);

    ///Lets us know how many boards we have to see.
    unsigned int BoardsToEnumerate(unsigned int cardsToDraw);

    ///Turns a two letter string into a cardmask.
    CardMask MakeCard(const char* s);

    ///Only simulates the current hand strength.
    void SimCurrent(const Hand& shand, SimResults* results, float lowRange, float highRange);

    ///Simulates how strong we will be at showdown. SimCurrent should be called first.
    void SimSd(const Hand& shand, SimResults* results, float lowRange, float highRange, size_t boards);

    ///Simulates how strong we will be at showdown against multiple opponents.
    void SimSdMulti(const Hand& shand, SimResults* results, unsigned int boards, unsigned int hands, unsigned int numOpponents);

}
#endif
