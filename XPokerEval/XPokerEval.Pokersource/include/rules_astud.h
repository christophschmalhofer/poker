/*
 * Copyright (C) 1999-2006 
 *           Brian Goetz <brian@quiotix.com>
 *           Loic Dachary <loic@dachary.org>
 *
 *  This package is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 dated June, 1991.
 *
 *  This package is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this package; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 *  MA 02110-1301, USA.
 */
/*
   Note that this file has two #if .. #endif sections -- one for 
   StdDeck macros to prevent double-inclusion, and one to define the 
   generic Rules_ macros if RULES_ASTUD is defined 
*/

#ifndef __RULES_ASTUD_H__
#define __RULES_ASTUD_H__

#include "pokereval_export.h"

#define AStudRules_HandType_NOPAIR    0
#define AStudRules_HandType_ONEPAIR   1
#define AStudRules_HandType_TWOPAIR   2
#define AStudRules_HandType_TRIPS     3
#define AStudRules_HandType_STRAIGHT  4
#define AStudRules_HandType_FULLHOUSE 5
#define AStudRules_HandType_FLUSH     6
#define AStudRules_HandType_QUADS     7
#define AStudRules_HandType_STFLUSH   8
#define AStudRules_HandType_FIRST     AStudRules_HandType_NOPAIR
#define AStudRules_HandType_LAST      AStudRules_HandType_STFLUSH
#define AStudRules_HandType_COUNT     9

#define AStudRules_TEN_STRAIGHT \
 ((1 << AStudDeck_Rank_ACE ) \
  | (1 << AStudDeck_Rank_7 ) \
  | (1 << AStudDeck_Rank_8 ) \
  | (1 << AStudDeck_Rank_9 ) \
  | (1 << AStudDeck_Rank_TEN ))

extern const POKEREVAL_EXPORT char *AStudRules_handTypeNames[AStudRules_HandType_LAST+1];
extern const POKEREVAL_EXPORT char *AStudRules_handTypeNamesPadded[AStudRules_HandType_LAST+1];

extern POKEREVAL_EXPORT int AStudRules_nSigCards[AStudRules_HandType_LAST+1];
extern POKEREVAL_EXPORT int AStudRules_HandVal_toString(HandVal handval, char *outString); 
extern POKEREVAL_EXPORT int AStudRules_HandVal_print(HandVal handval);

#endif

#ifdef RULES_ASTUD

#if defined(HandType_COUNT)
#include "rules_undef.h"
#endif

#define HandType_NOPAIR    AStudRules_HandType_NOPAIR    
#define HandType_ONEPAIR   AStudRules_HandType_ONEPAIR   
#define HandType_TWOPAIR   AStudRules_HandType_TWOPAIR   
#define HandType_TRIPS     AStudRules_HandType_TRIPS     
#define HandType_STRAIGHT  AStudRules_HandType_STRAIGHT  
#define HandType_FLUSH     AStudRules_HandType_FLUSH     
#define HandType_FULLHOUSE AStudRules_HandType_FULLHOUSE 
#define HandType_QUADS     AStudRules_HandType_QUADS     
#define HandType_STFLUSH   AStudRules_HandType_STFLUSH    
#define HandType_FIRST     AStudRules_HandType_FIRST  
#define HandType_COUNT     AStudRules_HandType_COUNT     
#define HandType_LAST      AStudRules_HandType_LAST

#define handTypeNames        AStudRules_handTypeNames
#define handTypeNamesPadded  AStudRules_handTypeNamesPadded
#define nSigCards            AStudRules_nSigCards
#define HandVal_print        AStudRules_HandVal_print
#define HandVal_toString     AStudRules_HandVal_toString

#endif
