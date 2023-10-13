#include "card_set.h"

Card_Set* create_Card_Set(int n){
    Card_Set *cs = (Card_Set*) malloc(sizeof(Card_Set));
    cs->cards = (Card*)malloc(sizeof(Card)*n);
    cs->n = n;

    for(int i = 0; i<n; i++){
        cs->cards[i] = create_random_card();
    }

    return cs;
}


//play a card
void play_card(Card_Set *c, int value, char color){
    
    for(int i = 0; i< c->n;i++){
        if(c->cards[i].value == value && c->cards[i].color == color){
            removeCardFromSet();
        }
    }


}


int main(){
    srand((unsigned int)time(NULL));

    Card_Set *cs = create_Card_Set(30);

    for(int i=0; i< cs->n;i++){
        print_card(&cs->cards[i]);
    }

    return 0; 
}