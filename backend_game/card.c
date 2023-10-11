#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "card.h"

Card create_random_card(){
    //Card *c = (Card*)malloc(sizeof(Card));
    Card c;

    c.value = rand() % 10; 

    //srand((time_t)time(NULL));
    int color = rand() % 4;

    if(color == 0) c.color = 'G'; else
    if(color == 1) c.color = 'B'; else
    if(color == 2) c.color = 'R'; else
    if(color == 3) c.color = 'Y';

    return c;
}

void print_card(Card *c){
    printf("%d, %c\n", c->value, c->color);
}

Card_Set* create_Card_Set(int n){
    Card_Set *cs = (Card_Set*) malloc(sizeof(Card_Set));
    cs->cards = (Card*)malloc(sizeof(Card)*n);
    cs->n = n;

    for(int i = 0; i<n; i++){
        cs->cards[i] = create_random_card();
    }

    return cs;
}

int main(){
    srand((unsigned int)time(NULL));

    Card_Set *cs = create_Card_Set(30);

    for(int i=0; i< cs->n;i++){
        print_card(&cs->cards[i]);
    }


    return 0;
}