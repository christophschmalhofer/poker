/* Showdown.c */

#include "Showdown.h"

Hand_T      board[5], holeHand[kMaxPlayers], deck[52],
            /* The following are each one card per hand so the cards can be
               output to results in the same order that user input them: */
            holeCard[2*kMaxPlayers], boardCard[4+1],
            deadCard[52-2-5+1]; /* +1s for excess input */
long        wins[kMaxPlayers], splits[kMaxPlayers];
int         nPlayers, nUnknown, nBoardCards, nCardsInDeck, nDeadCards, firstUnknown;
double      partialPots[kMaxPlayers], nPots;
FILE        *out, *output[2];
int         limitIx0, limitIx1, limitIx2, limitIx3, limitIx4;
int         dealt[52];

void InitDeck(void)
{
    Hand_T	*h;
    int		rank, suit, i = 0;

    h = deck;
    for (suit = kClubs; suit <= kSpades; ++suit)
        for (rank = 2; rank <= kA; ++rank) {
            *h++ = Encode(rank, suit);
            dealt[i++] = false; }
}

void Enum2GuysNoFlop(void)  /* special case for speed of EnumBoardsNoUnknown */
{
    int     deckIx0, deckIx1, deckIx2, deckIx3, deckIx4;
    Eval_T	handValue0, handValue1;
    int     wins0 = 0, splits0 = 0, pots = 0;

    for (deckIx0 = 0; deckIx0 <= limitIx0; ++deckIx0) {
        board[0] = deck[deckIx0];
        for (deckIx1 = deckIx0 + 1; deckIx1 <= limitIx1; ++deckIx1) {
            board[1] = board[0] | deck[deckIx1];
            for (deckIx2 = deckIx1 + 1; deckIx2 <= limitIx2; ++deckIx2) {
                board[2] = board[1] | deck[deckIx2];
                for (deckIx3 = deckIx2 + 1; deckIx3 <= limitIx3; ++deckIx3) {
                    board[3] = board[2] | deck[deckIx3];
                    for (deckIx4 = deckIx3 + 1; deckIx4 <= limitIx4; ++deckIx4) {
                        board[4] = board[3] | deck[deckIx4];
                        handValue0 = Hand_7_Eval(board[4] | holeHand[0]);
                        handValue1 = Hand_7_Eval(board[4] | holeHand[1]);
                        /* wins[1], splits[1], and partialPots are inferred below */
                        ++pots;
                        if (handValue0 > handValue1)
                            ++wins0;
                        else
                            if (handValue0 == handValue1)
                                ++splits0; } } } } }
		wins[0] = wins0;
		wins[1] = pots - wins0 - splits0;
		splits[0] = splits[1] = splits0;
		partialPots[0] = partialPots[1] = splits0 / 2.0;
}

#define mPotResults                                     \
    {                                                   \
    Eval_T			eval, bestEval, handValue[kMaxPlayers]; \
    int				i, winningPlayer, waysSplit;        \
    Hand_T			player;                             \
    double			partialPot;                         \
                                                        \
    bestEval = 0;                                       \
    for (i = 0; i < nPlayers; ++i) {                    \
        player = board[4] | holeHand[i];                \
        handValue[i] = eval = Hand_7_Eval(player);      \
        if (eval > bestEval) {                          \
            bestEval = eval;                            \
            waysSplit = 0;                              \
            winningPlayer = i; }                        \
        else                                            \
            if (eval == bestEval)                       \
                ++waysSplit; }                          \
    if (!waysSplit)                                     \
        ++wins[winningPlayer];                          \
    else {                                              \
        partialPot = 1.0/++waysSplit;                   \
        for (i = 0; waysSplit; ++i)                     \
            if (handValue[i] == bestEval) {             \
                partialPots[i] += partialPot;           \
                ++splits[i];                            \
                --waysSplit; } } }

void EnumBoardsNoUnknown(void)
/*  This is the same as EnumBoards (below) except mPotResults is executed directly.
    This one is called when there are no players with unspecified hole cards. */
{
    int     deckIx0, deckIx1, deckIx2, deckIx3, deckIx4;

    switch (nBoardCards) {
        case 0:
            if (nPlayers == 2) {
                Enum2GuysNoFlop();  /* special case */
                break; }
            for (deckIx0 = 0; deckIx0 <= limitIx0; ++deckIx0) {
                board[0] = deck[deckIx0];
                for (deckIx1 = deckIx0 + 1; deckIx1 <= limitIx1; ++deckIx1) {
                    board[1] = board[0] | deck[deckIx1];
                    for (deckIx2 = deckIx1 + 1; deckIx2 <= limitIx2; ++deckIx2) {
                        board[2] = board[1] | deck[deckIx2];
                        for (deckIx3 = deckIx2 + 1; deckIx3 <= limitIx3; ++deckIx3) {
                            board[3] = board[2] | deck[deckIx3];
                            for (deckIx4 = deckIx3 + 1; deckIx4 <= limitIx4; ++deckIx4) {
                                board[4] = board[3] | deck[deckIx4];
                                mPotResults } } } } }
            break;
        case 1:
            for (deckIx1 = 0; deckIx1 <= limitIx1; ++deckIx1) {
                board[1] = board[0] | deck[deckIx1];
                for (deckIx2 = deckIx1 + 1; deckIx2 <= limitIx2; ++deckIx2) {
                    board[2] = board[1] | deck[deckIx2];
                    for (deckIx3 = deckIx2 + 1; deckIx3 <= limitIx3; ++deckIx3) {
                        board[3] = board[2] | deck[deckIx3];
                        for (deckIx4 = deckIx3 + 1; deckIx4 <= limitIx4; ++deckIx4) {
                            board[4] = board[3] | deck[deckIx4];
                            mPotResults } } } }
            break;
        case 2:
            for (deckIx2 = 0; deckIx2 <= limitIx2; ++deckIx2) {
                board[2] = board[0] | deck[deckIx2];
                for (deckIx3 = deckIx2 + 1; deckIx3 <= limitIx3; ++deckIx3) {
                    board[3] = board[2] | deck[deckIx3];
                    for (deckIx4 = deckIx3 + 1; deckIx4 <= limitIx4; ++deckIx4) {
                        board[4] = board[3] | deck[deckIx4];
                        mPotResults } } }
            break;
        case 3:
            for (deckIx3 = 0; deckIx3 <= limitIx3; ++deckIx3) {
                board[3] = board[0] | deck[deckIx3];
                for (deckIx4 = deckIx3 + 1; deckIx4 <= limitIx4; ++deckIx4) {
                    board[4] = board[3] | deck[deckIx4];
                    mPotResults } }
            break;
        case 4:
            for (deckIx4 = 0; deckIx4 <= limitIx4; ++deckIx4) {
                board[4] = board[0] | deck[deckIx4];
                mPotResults }
    } /* end switch */
}

void EnumUnknowns(void)
{
    int     i;
    int     deckIx0, deckIx1, deckIx2, deckIx3;

    if (nUnknown == 1) {
        for (deckIx0 = 0; deckIx0 <= limitIx3; ++deckIx0) {
            if (dealt[deckIx0])
                continue;
            for (deckIx1 = deckIx0 + 1; deckIx1 <= limitIx4; ++deckIx1) {
                if (dealt[deckIx1])
                    continue;
                holeHand[firstUnknown] = deck[deckIx0] | deck[deckIx1];
                mPotResults } }
    } else {
        for (deckIx0 = 0; deckIx0 <= limitIx1; ++deckIx0) {
            if (dealt[deckIx0])
                continue;
            for (deckIx1 = deckIx0 + 1; deckIx1 <= limitIx2; ++deckIx1) {
                if (dealt[deckIx1])
                    continue;
                for (deckIx2 = deckIx1 + 1; deckIx2 <= limitIx3; ++deckIx2) {
                    if (dealt[deckIx2])
                        continue;
                    for (deckIx3 = deckIx2 + 1; deckIx3 <= limitIx4; ++deckIx3) {
                        if (dealt[deckIx3])
                            continue;
                        /* The first "unknown" holding wx and the second holding yz is equivalent,
                            w/r the results of interest, to the first holding yz and the second wx.
                            So we just one of the two cases and double the total of wins, splits,
                            etc. later. */
                        holeHand[firstUnknown] = deck[deckIx0] | deck[deckIx1];
                        holeHand[firstUnknown+1] = deck[deckIx2] | deck[deckIx3];
                        mPotResults
                        holeHand[firstUnknown] = deck[deckIx0] | deck[deckIx2];
                        holeHand[firstUnknown+1] = deck[deckIx1] | deck[deckIx3];
                        mPotResults
                        holeHand[firstUnknown] = deck[deckIx0] | deck[deckIx3];
                        holeHand[firstUnknown+1] = deck[deckIx1] | deck[deckIx2];
                        mPotResults } } } }
		//double totals as indicated above
		for (i = 0; i < nPlayers; i++) {
			wins[i] *= 2;
			splits[i] *= 2;
            partialPots[i] *= 2.0; }
		}
}

void EnumBoards(void)
{
    int     deckIx0, deckIx1, deckIx2, deckIx3, deckIx4;

    switch (nBoardCards) {
        case 0:
            for (deckIx0 = 0; deckIx0 <= limitIx0; ++deckIx0) {
                board[0] = deck[deckIx0];
                dealt[deckIx0] = true;
                for (deckIx1 = deckIx0 + 1; deckIx1 <= limitIx1; ++deckIx1) {
                    board[1] = board[0] | deck[deckIx1];
                    dealt[deckIx1] = true;
                    for (deckIx2 = deckIx1 + 1; deckIx2 <= limitIx2; ++deckIx2) {
                        board[2] = board[1] | deck[deckIx2];
                        dealt[deckIx2] = true;
                        for (deckIx3 = deckIx2 + 1; deckIx3 <= limitIx3; ++deckIx3) {
                            board[3] = board[2] | deck[deckIx3];
                            dealt[deckIx3] = true;
                            for (deckIx4 = deckIx3 + 1; deckIx4 <= limitIx4; ++deckIx4) {
                                board[4] = board[3] | deck[deckIx4];
                                dealt[deckIx4] = true;
                                EnumUnknowns();
                                dealt[deckIx4] = false; }
                            dealt[deckIx3] = false; }
                        dealt[deckIx2] = false; }
                    dealt[deckIx1] = false; }
                dealt[deckIx0] = false; }
            break;
        case 1:
            for (deckIx1 = 0; deckIx1 <= limitIx1; ++deckIx1) {
                board[1] = board[0] | deck[deckIx1];
                dealt[deckIx1] = true;
                for (deckIx2 = deckIx1 + 1; deckIx2 <= limitIx2; ++deckIx2) {
                    board[2] = board[1] | deck[deckIx2];
                    dealt[deckIx2] = true;
                    for (deckIx3 = deckIx2 + 1; deckIx3 <= limitIx3; ++deckIx3) {
                        board[3] = board[2] | deck[deckIx3];
                        dealt[deckIx3] = true;
                        for (deckIx4 = deckIx3 + 1; deckIx4 <= limitIx4; ++deckIx4) {
                            board[4] = board[3] | deck[deckIx4];
                            dealt[deckIx4] = true;
                            EnumUnknowns();
                            dealt[deckIx4] = false; }
                        dealt[deckIx3] = false; }
                    dealt[deckIx2] = false; }
                dealt[deckIx1] = false; }
            break;
        case 2:
            for (deckIx2 = 0; deckIx2 <= limitIx2; ++deckIx2) {
               board[2] = board[0] | deck[deckIx2];
               dealt[deckIx2] = true;
               for (deckIx3 = deckIx2 + 1; deckIx3 <= limitIx3; ++deckIx3) {
                    board[3] = board[2] | deck[deckIx3];
                    dealt[deckIx3] = true;
                    for (deckIx4 = deckIx3 + 1; deckIx4 <= limitIx4; ++deckIx4) {
                        board[4] = board[3] | deck[deckIx4];
                        dealt[deckIx4] = true;
                        EnumUnknowns();
                        dealt[deckIx4] = false; }
                    dealt[deckIx3] = false; }
                dealt[deckIx2] = false; }
            break;
        case 3:
            for (deckIx3 = 0; deckIx3 <= limitIx3; ++deckIx3) {
                board[3] = board[0] | deck[deckIx3];
                dealt[deckIx3] = true;
                for (deckIx4 = deckIx3 + 1; deckIx4 <= limitIx4; ++deckIx4) {
                    board[4] = board[3] | deck[deckIx4];
                    dealt[deckIx4] = true;
                    EnumUnknowns();
                    dealt[deckIx4] = false; }
                dealt[deckIx3] = false; }
            break;
        case 4:
            for (deckIx4 = 0; deckIx4 <= limitIx4; ++deckIx4) {
                board[4] = board[0] | deck[deckIx4];
                dealt[deckIx4] = true;
                EnumUnknowns();
                dealt[deckIx4] = false; }
    } /* end switch */
}

void Enumerate(void) {

    int i;
    int deckIx0 = 0, deckIx1;

    for (i = 0; i < nPlayers; ++i) {
        wins[i] = splits[i] = 0;
        partialPots[i] = 0.0; }

    /* Compact deck array to remove elements for user-specified hole/board/dead cards */
    for (deckIx1 = 0; deckIx1 < 52; ++deckIx1)
        if (!dealt[deckIx1]) {
            deck[deckIx0] = deck[deckIx1];
            dealt[deckIx0++] = false; }

    limitIx4 = nCardsInDeck - 1;
    limitIx3 = limitIx4 - 1;
    limitIx2 = limitIx3 - 1;
    limitIx1 = limitIx2 - 1;
    limitIx0 = limitIx1 - 1;

    if (nUnknown > 0) {
        firstUnknown = nPlayers - nUnknown;
        EnumBoards(); }
	else
		EnumBoardsNoUnknown();
}

void PrintCard(FILE *f, Hand_T *h)
{
    int     r, s;

    Decode(*h, &r, &s);
    switch (s) {
        case 0: s = 'c'; break;
        case 1: s = 'd'; break;
        case 2: s = 'h'; break;
        case 3: s = 's'; break; }
    switch (r) {
        case kA: r = 'A'; break;
        case kK: r = 'K'; break;
        case kQ: r = 'Q'; break;
        case kJ: r = 'J'; break;
        case 10: r = 'T'; break;
        default: r += '0'; }
    fprintf(f, "%c%c", r, s);
}

void General_Help(void)
{
	printf("This program displays a series of prompts for information.  After each prompt\n");
	printf("it waits for a response from you.  End each of your responses with the Return\n");
	printf("or Enter key.\n");
	printf("\n");
	printf("Some prompts indicate valid responses in (parentheses).  And some indicate the\n");
	printf("most typical situation in [square brackets]; to specify that condition, just\n");
	printf("press Return or Enter by itself.\n");
	printf("\n");
	printf("If you need to continue your response to a single prompt on another line (such\n");
	printf("as when you need to enter many hole cards) end the line to be continued with\n");
	printf("\"--\" (two hyphens).\n"); 
	printf("\n");
	printf("For help with a specific response, type \"?\" and then press Return or Enter.\n");
#ifndef __Mac__
	printf("To quit the program, close its window.\n");
#endif
}

int GetNextCard(char *string, int *offset, Hand_T *h)
/*  Increments *offset to skip over any initial spaces, tabs, or commas;
    returns 1 for valid card, 0 if string empty, -1 for bad input;
    if valid card, updates *offset to be just beyond last char of card */
{
    char    c = 1, s, *p;       /* initialization works around CW Pro 2 sscanf bug */
    int     rank, suit;
    int     i, consumed, digit_one = 0;

    p = string + *offset;
    while (*p == ' ' || *p == '\t' || *p == ',')
        ++p;
    *offset += (int)(p - (string + *offset));
    if (*p == '1') {
    	/* 10 rather than T for a ten is legal */
    	digit_one = 1;
    	++p;
    	++*offset; }
    if ((i = sscanf(p, " %c%c%n", &c, &s, &consumed)) != 2) {
        if (!digit_one && (i <= 0 || (i == 1 && !c)))   /* rhs of || works around CW Pro 2 sscanf bug */
            return 0;
        printf("Please specify each card as a rank (2..9/10/T/J/Q/K/A) and a suit (c/d/h/s).\n");
        return -1; }
    *offset += consumed;
    if (digit_one) {
    	if (c == '0')
    		rank = kT;
    	else {
    		printf("(Found \"1\" not followed by \"0\") Enter a ten as \"T\" or \"10\".\n");
    		return -1;
    		} }
    else
	    if (c >= '2' && c <= '9')
	        rank = c - '0';
	    else
	        switch (toupper(c)) {
	            case 'T':   rank = kT; break;
	            case 'J':   rank = kJ; break;
	            case 'Q':   rank = kQ; break;
	            case 'K':   rank = kK; break;
	            case 'A':   rank = kA; break;
	            default:    printf("Bad card rank \"%c\" (expecting 2..10 or one of T/J/Q/K/A).\n",
	                                (int)c);
	                        return -1; }
    switch (tolower(s)) {
        case 'c':   suit = kClubs;      break;
        case 'd':   suit = kDiamonds;   break;
        case 'h':   suit = kHearts;     break;
        case 's':   suit = kSpades;     break;
        default:    printf("Bad suit \"%c\" (expecting one of c/d/h/s).\n", (int)s); return -1; }
    *h = Encode(rank, suit);
    if (dealt[(suit-kClubs)*13 + rank - 2]) {
        printf("There's only one ");
        PrintCard(stdout, h);
        printf(" in the deck!\n");
        return -1; }
    dealt[(suit-kClubs)*13 + rank - 2] = true;
    return 1;
}

void ReadLine(char *s, int max)
{
    int     count;

	for (count = 0; count < max && !feof(stdin); ++count)
        if ((*s++ = getchar()) == '\n') {
            *--s = 0;
            if (strlen(s) < 2)
            	return;
            if (*(s-1) == '-' && *(s-2) == '-') {
            	s -= 2;		/* line ended with "--" indicating line continuation */
            	count -= 2; }
            else
				return; }
    *s = 0;
}

Boolean GetResponse(char *prompt, char *s, int max, char *help_text)
{
	do {
		printf(prompt);
		ReadLine(s, max);
		if (*s == '?')
			printf(help_text);
	} while (*s == '?');
	if (tolower(*s) == 'h') {
		General_Help();
		return false; }
	return !feof(stdin);
}

Boolean GetHoleCards(int *nPlayers)
{
    char        s[81];
    int         offset, card = 0, i;

    if (!GetResponse("\nKnown hole cards; two per player: ", s, 80,
    "For each card enter the rank -- 2,3,4,5,6,7,8,9,(10 or T),J,Q,K,A --\n"
    "and suit -- c,d,h,s.  Enter two cards for each player.  Letters\n"
    "may be either upper- or lower-case.\n"))
    	return false;
    offset = 0;
    while ((i = GetNextCard(s, &offset, &holeCard[card])) > 0)
        if (++card == kMaxPlayers*2)
            break;
    if (i < 0)
        return false;
    if (card & 1 || card < 2) {
        printf("Number of hole cards must be at least two, and even.\n");
        return false; }
    *nPlayers = card / 2;
    for (i = 0; i < *nPlayers; ++i) {
        holeHand[i] = holeCard[2*i] | holeCard[2*i + 1]; }
    return true;
}

Boolean GetBoard(int *nBoardCards, Hand_T *board)
{
    char        s[81];
    int         offset, i;

    *board = 0;
    *nBoardCards = 0;
    if (!GetResponse("Known board cards [none]: ", s, 80,
    "If no board cards are known (there's no flop yet) just press Return or Enter.\n"
    "If only the flop has been dealt, enter three cards; if the turn card has been\n"
    "dealt, enter four cards.\n"
    "For each card enter the rank (2,3,4,5,6,7,8,9,10 or T,J,Q,K,A) and suit (c,d,h,s).\n"))
    	return false;
    offset = 0;
    while ((i = GetNextCard(s, &offset, &boardCard[*nBoardCards])) > 0) {
        *board |= boardCard[*nBoardCards];
        if (++*nBoardCards > 4)
            break; }
    if (i < 0)
        return false;
    if (*nBoardCards > 4) {
        printf("Number of board cards cannot exceed 4 (flop and turn).\n");
        return false; }
    return true;
}

Boolean GetDeadCards(int *nDeadCards)
{
    char    s[81];
    int     offset, i, max = nCardsInDeck - (5 - nBoardCards);

    if (!GetResponse("Dead/exposed cards [none]: ", s, 80,
    "Enter any known cards which are unavailable to be dealt to the board, such as\n"
    "those which have been exposed, perhaps accidentally.  If there are no such cards\n"
    "then just press Return or Enter."))
    	return false;
    *nDeadCards = 0;
    offset = 0;
    while ((i = GetNextCard(s, &offset, &deadCard[*nDeadCards])) > 0)
        if (++*nDeadCards == max + 1)
            break;
    if (i < 0)
        return false;
    if (*nDeadCards > max) {
        printf("Not enough cards left for the board!\n");
        return false; }
    return true;
}

Boolean GetNbrUnknown(int *nUnknown)
{
    char    s[81], prompt[81];
    int     max, i;

    *nUnknown = 0;
    max = kMaxPlayers - nPlayers;
    if (!max)
        return true;
    if (max > 2)
        max = 2;
    sprintf(prompt, "Number of players with unknown hole cards (0 to %d) [0]: ", max);
    do {
        if (!GetResponse(prompt, s, 80,
        "Enter the number of players holding unknown (\"random\") cards.  Only\n"
        "a small number of such players can be accommodated by this program\n"
        "because otherwise the number of possible outcomes would be too large.\n"
        "If there are no such players you can just press Return or Enter.\n"))
        	return false;
        *nUnknown = 0;
        if (!s[0])
            return true;
        i = sscanf(s, "%d", nUnknown);
    } while (i == -1 || *nUnknown < 0 || *nUnknown > max);
    return true;
}

char *WithCommas(double n, char *s)
/* returns s; caller responsible for s being large enough */
{
    char    *in, *out;
    int     commas, len, leadDigits, i;

    sprintf(s, "%.0f", n);
    commas = (len = (int)strlen(s)) / 3;
    if (!(leadDigits = len % 3)) {
        --commas;
        leadDigits = 3; }
    if (commas < 1)
        return s;
    in = s + len;
    out = in + commas;
    *out-- = *in--; /* copy the terminating null */
    for (i = 0; i < commas; ++i) {
        *out-- = *in--;
        *out-- = *in--;
        *out-- = *in--;
        *out-- = ','; }
    for (i = 0; i < leadDigits; ++i)
        *out-- = *in--;
    return s;
}

Boolean UserConfirm(double nDeals)
{
    char    s[81], prompt[81], c;

	sprintf(prompt, "%s deals required.  Start dealing? (y/n) [y]: ", WithCommas(nDeals, s));
    while (true) {
        if (!GetResponse(prompt, s, 80,
        "The program is ready to tabulate each possible outcome (\"deal\").  Nothing\n"
        "will happen while it is working.  When it is finished, you will see the results.\n"
        "The time required depends on your computer's speed and how many deals are\n"
        "required.  As a very rough rule of thumb, figure 1 million deals per second per\n"
        "400 MHz of computer processor speed.\n"
        "To start the calculations, just press Return or Enter.  To start over, press\n"
        "\"n\" and then press Return or Enter.\n"))
        	return false;
        if (s[0]) {
            if ((sscanf(s, " %c", &c)) != 1)
                continue;
            if (tolower(c) == 'y')
                return true;
            if (tolower(c) == 'n')
                return false; }
        else
            return true; }
}

void Initialize(void)
{
    Boolean     resultsCreated;
#ifdef __Mac__
    HFileParam  PB;
#endif

    if (!(resultsCreated = ((out = fopen(kOutFileName, "r")) == 0)))
        fclose(out);
    if (!(out = fopen(kOutFileName, "a")))
        exit(errno);
    if (resultsCreated) {
        fprintf(out, "View this file with a fixed-width font such as Courier");
#ifdef __Mac__
		fprintf(out, " or Monaco");
#endif
		fprintf(out, "\n"); }
#ifdef __Mac__
    PB.ioNamePtr = "\p"kOutFileName;
    PB.ioVRefNum = -LMGetSFSaveDisk();
    PB.ioDirID = LMGetCurDirStore();
    PB.ioFDirIndex = 0;
    if (PBHGetFInfoSync((HParmBlkPtr)&PB) == noErr) {
            PB.ioFlFndrInfo.fdCreator = kFileCreator;
            PB.ioDirID = LMGetCurDirStore();
            PBHSetFInfoSync((HParmBlkPtr)&PB); }
#endif
#ifdef THINK_C
    console_options.title = "\pHold¼Em Showdown";
#endif
#ifdef __MWERKS__
	SIOUXSettings.asktosaveonclose = false;
    SIOUXSettings.rows = 25;
    SIOUXSettings.columns = 80;
#ifdef __Mac__
    printf("\n");
    SIOUXSetTitle("\pHold'Em Showdown");
#else
	SIOUXSetTitle("Hold'Em Showdown");
#endif
#endif
    if (!Init_Hand_Eval()) {
        printf("RAM allocation failure.\n");
        exit(0); }
#ifdef __Mac__
    printf("        Hold'Em Showdown version ");
    printf(kVersion);
    printf(" written by Steve Brecher\n");
    printf("Deals all possible boards to get exact win probability for each hand specified.\n");
    printf("Results written/appended to ""kOutFileName"" in Hold'Em Showdown's folder.\n");
#else
    printf("        HoldEm Showdown version ");
    printf(kVersion);
    printf(" written by Steve Brecher\n");
    printf("Deals all possible boards to get exact win probability for each hand specified.\n");
    printf("Results written/appended to \""kOutFileName"\".\n");
#endif
    printf("\nFor general help, type \"help\" or just \"h\" followed by Return or Enter.\n");
    printf("For help with a specific response, type \"?\" followed by Return or Enter.\n");
    output[0] = out;
    output[1] = stdout;
}

#define kFormat "%13.6f"

void ResultsOut(void)
{
    int     i, j, n, f, nbrToPrint;
    char    s[81];

    nbrToPrint = nPlayers;
    if (nUnknown == 2) {    /* show the total of the two as one entry */
        --nbrToPrint;
        wins[nPlayers-2] += wins[nPlayers-1];
        splits[nPlayers-2] += splits[nPlayers-1];
        partialPots[nPlayers-2] += partialPots[nPlayers-1]; }

    fprintf(out, "\n%s pots with board cards:", WithCommas(nPots, s));
    if (!nBoardCards)
        fprintf(out, " (unspecified)");
    for (i = 0; i < nBoardCards; ++i) {
        fprintf(out, " ");
        PrintCard(out, &boardCard[i]); }
    if (nDeadCards) {
        fprintf(out, "\nDead/exposed cards:");
        for (i = 0; i < nDeadCards; ++i) {
            fprintf(out, " ");
            PrintCard(out, &deadCard[i]); } }
    fprintf(out, "\n");

    for (f = 0; f < 2; ++f) {
        n = nbrToPrint;
        j = 0;
        while (n > 0) {
            fprintf(output[f], "\n                     ");
            for (i = j; i < j+k_hands_per_line && i < nbrToPrint; ++i)
                if (i >= nPlayers - nUnknown)
                    if (nUnknown == 1)
                        fprintf(output[f], "         Unknown");
                    else
                        fprintf(output[f], "         Unknowns");
                else {
                    fprintf(output[f], "         ");
                    PrintCard(output[f], &holeCard[i*2]);
                    PrintCard(output[f], &holeCard[i*2+1]); }
            fprintf(output[f], "\n%% chance of outright win ");
            for (i = j; i < j+k_hands_per_line && i < nbrToPrint; ++i)
                fprintf(output[f], kFormat, wins[i]*100.0/nPots);
            fprintf(output[f], "\n%% chance of win or split ");   
            for (i = j; i < j+k_hands_per_line && i < nbrToPrint; ++i)
                fprintf(output[f], kFormat, (wins[i]+splits[i])*100.0/nPots);
            fprintf(output[f], "\nexpected return, %% of pot");
            for (i = j; i < j+k_hands_per_line && i < nbrToPrint; ++i)
                fprintf(output[f], kFormat, (wins[i]+partialPots[i])*100.0/nPots);
            fprintf(output[f], "\nfair pot odds:1          ");
            for (i = j; i < j+k_hands_per_line && i < nbrToPrint; ++i) {
                if (wins[i] || partialPots[i] > 0.1E-9)
                    fprintf(output[f], kFormat,
                        (nPots-(wins[i]+partialPots[i]))/(wins[i]+partialPots[i]));
                else
                    fprintf(output[f], " infinite"); }
            fprintf(output[f], "\npots won:                ");
            for (i = j; i < j+k_hands_per_line && i < nbrToPrint; ++i) {
                fprintf(output[f], "%13.2f", wins[i] + partialPots[i]);
                --n; }
            fprintf(output[f], "\n");
            j += k_hands_per_line; }
        fflush(output[f]); }
}

double Factorial(double n)
{
    if (n > 1.0)
        return n * Factorial(n - 1.0);
    return 1.0;
}

double Combos(int chooseFrom, int choose)
{
    return Factorial((double)chooseFrom) /
            (Factorial((double)choose) * Factorial((double)(chooseFrom - choose)));
}

double NbrEnumerations(void)
{
    double  enums;

    enums = Combos(nCardsInDeck - 2*nUnknown, 5 - nBoardCards); /* number of boards */
    if (nUnknown)
        /*    *= the number of ORDERED sets of hole cards for the "unknown" players */
        enums *= Combos(nCardsInDeck, 2*nUnknown)
                    * Factorial(2*nUnknown)/((1 << nUnknown)/*2^^nUnknown*/);
    return enums;
}

int main(void)
{
    Boolean restart;
    char    s[81];
#if kTimer
    clock_t timer;
#endif

    Initialize();

    while (!feof(stdin)) {
        InitDeck();
        restart = true;
        if (GetHoleCards(&nPlayers)) {
            if (GetNbrUnknown(&nUnknown)) {
            	nPlayers += nUnknown;
	            if (nPlayers < 2)
	                printf("At least two players are required.\n");
	            else
	                if (GetBoard(&nBoardCards, &board[0])) {
	                    nCardsInDeck = 52 - (nPlayers - nUnknown)*2 - nBoardCards;
	                    if (GetDeadCards(&nDeadCards)) {
	                        nCardsInDeck -= nDeadCards;
	                        nPots = NbrEnumerations();
	                        if (nPots > LONG_MAX)
	                            printf("%s pots required, which is higher than this program can count.\n",
	                                    WithCommas(nPots, s));
	                        else
	                            /* Due to an optimization, deals == pots/2 when there are two
	                               players with unknown hole cards... */
	                            if (UserConfirm((nUnknown == 2) ? nPots/2.0 : nPots))
	                                restart = false; } } } }
		else
			if (feof(stdin))
				break;
        if (restart) {
            printf("\nRestarting...\n");
            continue; }
        IndicateWait();
#if kTimer
        timer = clock();
#endif
        Enumerate();
#if kTimer
        timer = clock() - timer;
#endif
        ResultsOut();
#if kTimer
        printf("seconds = %.2f\n", (float)timer/CLOCKS_PER_SEC);
		fflush(stdout);
		fprintf(out, "seconds = %.2f\n", (float)timer/CLOCKS_PER_SEC);
		fflush(out);
#endif
	}
}
