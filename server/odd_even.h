#define _ODD_EVEN_H

struct player{
    int choice;
    int id;
}; 
typedef struct player Player;

Player* create_player(int, int); 
int decide(Player*, int, Player*, int); 