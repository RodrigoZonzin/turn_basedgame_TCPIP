#include <stdio.h>

#define _CARD_H

struct card{
    int value; 
    char color; 
};
typedef struct card Card;

struct card_set{
    Card *cards;
    int n;
};
typedef struct card_set Card_Set;


Card create_random_card(); 
Card_Set* create_Card_Set();
void print_card(Card*);