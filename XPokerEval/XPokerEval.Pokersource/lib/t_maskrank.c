#include "poker_defs.h"

/* 
 * Table StdDeck_maskRankTable
 */

/*
StdDeck_masksRanksTable[].  Maps card ranks (2..A) to a cardmask which
has all the bits set except the bits corresponding to the cards whose
have the input rank.  This is a quick way to mask off all the cards of
a specific rank.
 */

StdDeck_CardMask StdDeck_maskRankTable[13] = { 
      { 0xfffefffefffefffe }  ,
      { 0xfffdfffdfffdfffd }  ,
      { 0xfffbfffbfffbfffb }  ,
      { 0xfff7fff7fff7fff7 }  ,
      { 0xffefffefffefffef }  ,
      { 0xffdfffdfffdfffdf }  ,
      { 0xffbfffbfffbfffbf }  ,
      { 0xff7fff7fff7fff7f }  ,
      { 0xfefffefffefffeff }  ,
      { 0xfdfffdfffdfffdff }  ,
      { 0xfbfffbfffbfffbff }  ,
      { 0xf7fff7fff7fff7ff }  ,
      { 0xefffefffefffefff }  
};
