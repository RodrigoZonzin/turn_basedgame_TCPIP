#define _CARD_SET_H

#include "card.h"

struct card_set{
    Card *cards;
    int n;
};
typedef struct card_set Card_Set;


Card_Set* create_Card_Set();