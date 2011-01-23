#ifndef __HANDEVALUATOR_H__
#define __HANDEVALUATOR_H__

#include "common.h"


typedef struct handeval_dag_node {
	struct handeval_dag_node* succs[13];	// the successor array, pointing to the successor node for each card rank
	unsigned int id;						// the id of the node
	int equiv_class_flush;					// the class if this hand resembles the flush-colored cards (-1 for invalid)	
	int equiv_class_normal;					// the class for this card if no flush is present (-1 for invalid)
	unsigned int maxn, maxf, minn, minf;    // the maximum (worst) and minimum (best) class reachable with this card combination, normal or with a flush
} handeval_dag_node;

typedef struct handeval_eq_class {
	unsigned int id;    // the id, lower for better classes
	char* cards;		// the 5 cards that constitute this equivalence class
	char* desc;			// a human readable class description
	char* shortdesc;	// a one or two character description, see HANDTYPE below for the distinct nine values
	char isflush;       // true if this equivalence class represents a flush or a straight flush
	unsigned int type;	// the type (numerical value of shortdesc), from "High Card"=0 to "Straight Flush"=8
	float domination, likelihood; 	// domination and likelihood value (domination is the percentage of hands that will loose against this one, likelihood the chance in percent that a random hand belongs to this class - high for straights, low for flushes)
} handeval_eq_class;

typedef struct simulation_result {
	unsigned int rounds;
	unsigned int player; /* Number of opponents (1 for heads-up) */
	unsigned int won;
	unsigned int split;
	char card1, card2;
	unsigned int winclasses[10], looseclasses[10];
} simulation_result;

typedef struct partial_evaluation {
	unsigned int evaluated;				// number of cards already evaluated
	handeval_dag_node *noflushnode;		// pointer to the node for non-flushes
	handeval_dag_node *flushnodes[4];	// pointer for the nodes for flushes
	char colors[4];						// counter for the colors
} partial_evaluation;

void handeval_init(char*, char*);
char* hand_to_str(char* hand, unsigned int cards);
char* str_to_cards(char* str, unsigned int number);
handeval_eq_class* calculate_equivalence_class(char* hand);

void partial_evaluate(unsigned int cardcnt, char* cards, partial_evaluation* pe);
handeval_eq_class* resume_evaluation(char* cards, const partial_evaluation* pev);
handeval_eq_class* calculate_minimal_class(unsigned int cardcnt, char* hand);
#endif



