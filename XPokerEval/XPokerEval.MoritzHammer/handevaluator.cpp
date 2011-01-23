#include "handevaluator.h"
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

handeval_dag_node *dag;
handeval_eq_class *classes[7462];

void error(char* msg) {
	fprintf(stderr, "ERROR: %s\n\n", msg);
	exit(-1);
}

void errorline(char* msg, unsigned int line) {
	fprintf(stderr, "ERROR in line %u: %s\n\n", line, msg);
	exit(-1);
}


///////////////////////////////////////////////////////////////////////////////
// Load equivalence classes from the equivalence class data file, based on
// the equivalence class list provided by Kevin Suffecool (Cactus Kev) at:
///////////////////////////////////////////////////////////////////////////////
void load_equivalenceclasses(char* fn)
{
	char buf[1024];
	static const char *DELIM = "\t";
	unsigned int i=1;
	FILE* f = fopen(fn, "r");
	while (fgets(buf, 1024, f)) {	// read a line
		// 1	AKQJT	8	Royal Flush	100.00	0.0032
		classes[i] = (handeval_eq_class*) malloc(sizeof(handeval_eq_class));
		classes[i]->id = i;
		strtok(buf, DELIM);
		classes[i]->cards = _strdup(strtok(0, DELIM));
		classes[i]->type = atoi(strtok(0, DELIM));
		classes[i]->desc = _strdup(strtok(0,DELIM));
		classes[i]->domination = atof(strtok(0,DELIM));
		classes[i]->likelihood = atof(strtok(0,DELIM));
		i++;
	}
	fclose(f);
}



///////////////////////////////////////////////////////////////////////////////
// Load the directed acyclic graph (DAG).
///////////////////////////////////////////////////////////////////////////////
void load_dag(char* fn)
{
	static char DELIM[2] = {'\t', '\n'};
	char buf[1024];
	unsigned int i, id, v, j;
	int ecf, ecn;
	handeval_dag_node *n;
	dag = (handeval_dag_node*) calloc(76154, sizeof(handeval_dag_node));
	
	FILE* f = fopen(fn, "r");
	i=0;
	while (fgets(buf, 1024, f))
	{	// read a line
		if ((id = atoi(strtok(buf, DELIM))) != i) 
		{
			errorline("Nonmatching node ID!", i+1);
		}
		n = &(dag[i]);	// get the target node
		n->id = id;		// set the id
		for (j=0; j < 13; j++) 
		{	// iterate the target cards
			v = atoi(strtok(0, DELIM));
			if (v > 0) {	// if valid link - invalid links remain set to 0 (by calloc)
				n->succs[j] = &(dag[v]);	// set card target
			}
		}
		
		ecf = atoi(strtok(0, DELIM));	// get and set the equivalence classes
		ecn = atoi(strtok(0, DELIM));

		if (ecf == 0 || ecn == 0 || ecf > 7500 || ecn > 7500) 
		{
			errorline("Invalid equivalence class ID", i+1);			
		}
		n->equiv_class_flush = ecf;
		n->equiv_class_normal = ecn;
		strtok(0, DELIM);					// skip the human-readable part
		n->maxn = atoi(strtok(0, DELIM));
		n->maxf = atoi(strtok(0, DELIM));
		n->minn = atoi(strtok(0, DELIM));
		n->minf = atoi(strtok(0, DELIM));
		i++;
	}
	fclose(f);
}	



///////////////////////////////////////////////////////////////////////////////
// Here's the workhorse function. Take a given poker hand and determine it's
// underlying equivalence class. You can take this number and compare it to
// the equivalence class of any other poker hand in order to determine the
// winner.
///////////////////////////////////////////////////////////////////////////////
handeval_eq_class* calculate_equivalence_class(char* hand)
{
	// first, walk the dag for non-flush-hands, and count the colors
	unsigned int i, colarr = 0, r, c, flush;			// color array stored in a 32-bit value. I hope that there is some way to do bitfiddeling for flush detection
	char* carr = (char*) &colarr;
	handeval_dag_node* n = 0;
	for (i=0; i < 7; i++) {
		r = hand[i] >> 2;	
		c = hand[i] & 3;
		if (!i) {
			n = &(dag[r]);  // the first layer - get the r-th node of the dag
		} else {			
			n = n->succs[r]; // later layers - follow the links (we assume the hand is valid and crash upon errors here)
		}
		carr[c]++;		// count the color
	}
	// next, check for a flush
	flush = 4;
	for (i=0; i < 4; i++) {
		if (carr[i] >= 5) {
			flush = i;
		}
	}
	if (flush == 4) {	// no flush
		return classes[n->equiv_class_normal];
	} else {
		// finally, if there is a flush, walk the dag again, and use only the cards of the flush-color
		n = 0;
		for (i=0; i < 7; i++) {	
			r = hand[i] >> 2;	
			c = hand[i] & 3;
			if (c == flush) {	// flush-coloured card?
				if (!n) {
					n = &(dag[r]);  // the first layer - get the r-th node of the dag
				} else {			
					n = n->succs[r]; // later layers - follow the links (we assume the hand is valid and crash upon errors here)
				}
			}
		}
		return classes[n->equiv_class_flush];
	}
}

char* hand_to_str(char* hand, unsigned int cards) {
	unsigned int i;
	char* res = (char*)malloc(cards*3);
	for (i=0; i < cards; i++) {
		res[i*3+0] = CARD[hand[i] >> 2];
		res[i*3+1] = COLOR[hand[i] & 3];
		res[i*3+2] = ' ';
	}
	res[cards*3-1] = 0;
	return res;
}

/** Convert a string to a hand, for debugging purposes. VERY UNSAFE! */
char* str_to_cards(char* str, unsigned int number) {
	unsigned int i, pos = 0, j;
	char val, r, c;
	char* res = (char*) malloc(number);
	for (i=0; i < number; i++) {
		r = str[pos++];
		c = str[pos++];
		pos++;
		val = 0;
		for (j=0; j < 13; j++) {
			if (r == CARD[j]) {
				val = j << 2;
			}
		}
		for (j=0; j < 4; j++) {
			if (c == COLOR[j]) {
				val += j;
			}
		}	
		res[i] = val;
	}
	return res;
}

void handeval_init(char* equivClassFile, char* cardDagFile) {
	load_equivalenceclasses(equivClassFile);
	load_dag(cardDagFile);
}

void partial_evaluate(unsigned int cardcnt, char* cards, partial_evaluation* pe) {
	unsigned int i, r, c;
	assert(cardcnt);				// there must be more than one card
	pe->evaluated = cardcnt;
	for (i=0; i < 4; i++) {
		pe->flushnodes[i] = 0;
		pe->colors[i] = 0;
	}
	for (i=0; i < cardcnt; i++) {
		r = cards[i] >> 2;	
		c = cards[i] & 3;
		// follow the dag for non-flush nodes
		if (!i) {		
			pe->noflushnode = &(dag[r]);  
		} else {			
			pe->noflushnode = pe->noflushnode->succs[r];  
		}
		// follow the dag for flush color candidates nodes
		pe->colors[c]++;
		if (pe->flushnodes[c] == 0) {
			pe->flushnodes[c] = &(dag[r]);  
		} else {			
			pe->flushnodes[c] = pe->flushnodes[c]->succs[r];  
		}
	}
}


handeval_eq_class* resume_evaluation(char* cards, const partial_evaluation* pe) {
	unsigned int i, r, c;
	handeval_dag_node *n = pe->noflushnode;
	unsigned int remain = 7 - pe->evaluated;
	int cl;
	char col[4] = {pe->colors[0], pe->colors[1], pe->colors[2], pe->colors[3]};
	
	for (i=0; i < remain; i++) {
		r = cards[i] >> 2;	
		c = cards[i] & 3;
		n = n->succs[r];				// follow the non-flush dag
		col[c]++;
	}
	// check for flushes
	int fc = -1;
	for (i=0; i < 4; i++) {
		if (col[i] >= 5) {
			fc = i;
			break;
		}
	}
	assert(n->equiv_class_normal > 0);
	if (fc >= 0) {
		n = pe->flushnodes[fc];
		for (i=0; i < remain; i++) {
			c = cards[i] & 3;
			if ((int) c == fc) {
				r = cards[i] >> 2;
				if (n) {
					n = n->succs[r];	
				} else {
					n = &(dag[r]);
				}
			}
		}
		assert(n->equiv_class_flush > 0);
		cl = n->equiv_class_flush;
	} else {
		cl = n->equiv_class_normal;
	}	
	
	return classes[cl];
}

handeval_eq_class* calculate_minimal_class(unsigned int cardcnt, char* cards) {
	unsigned int i, r, c;
	partial_evaluation pe;
	assert(cardcnt);				// there must be more than one card
	pe.noflushnode = 0;
	
	for (i=0; i < 4; i++) {
		pe.flushnodes[i] = 0;
		pe.colors[i] = 0;
	}
	for (i=0; i < cardcnt; i++) {
		r = cards[i] >> 2;	
		c = cards[i] & 3;
		// follow the dag for non-flush nodes
		if (!i) {		
			pe.noflushnode = &(dag[r]);  
		} else {			
			pe.noflushnode = pe.noflushnode->succs[r];  
		}
		// follow the dag for flush color candidates nodes
		pe.colors[c]++;
		if (pe.flushnodes[c] == 0) {
			pe.flushnodes[c] = &(dag[r]);  
		} else {			
			pe.flushnodes[c] = pe.flushnodes[c]->succs[r];  
		}
	}
	int minc = pe.noflushnode->maxn;
	if (minc == 0) {
		printf("MORE SEVERE BUG!\n\n\n");
		fflush(stdout);
	}
	for (i=0; i < 4; i++) {
		if (pe.colors[i] >= 5) {	// flush found
			if (pe.flushnodes[i]->equiv_class_flush < minc) {
				minc = pe.flushnodes[i]->equiv_class_flush;
			}
		}
	}
	if (minc <= 0) {
		printf("SEVERE BUG!\n\n\n");
		fflush(stdout);
	}
	return classes[minc];
}


