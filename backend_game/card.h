#include <stdio.h>

#define _CARD_H

struct card{
    int value; 
    char color; 
};
typedef struct card Card;

Card create_random_card(); 
void print_card(Card*);