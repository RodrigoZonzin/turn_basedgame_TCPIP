#include <stdio.h>
#include <stdlib.h>


#include "odd_even.h"


Player* create_player(int value, int id){
    Player *p  = (Player*)malloc(sizeof(Player));

    p->choice = value;
    p->id = id;
    
    return p;
}


//Players must chose between 0 (odd) and 1 (even)
//If a player is betting for odd and the sum of 
// the two throws is odd, then he wins 
int decide(Player *A, int a, Player *B, int b){

    int odd_or_even = (a + b) % 2;

    if(odd_or_even == 0 && A->choice == 0) return A->id;
    if(odd_or_even == 0 && B->choice == 1) return B->id;

    return -1;     
}

/* 
int main(){
    Player *Rodrigo = create_player(0, 28); 
    Player *Renan = create_player(1, 10); 

    int quem_ganhou = decide(Rodrigo, 5, Renan, 1); 

    printf("Quem ganhou? %d\n", quem_ganhou); 
    
    
    return 0; 
}

*/