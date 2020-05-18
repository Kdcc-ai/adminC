#ifndef CARD_FILE_H_INCLUDED
#define CARD_FILE_H_INCLUDED
#include "card_service.h"
void saveCard(Card *card);
Card* readCard();
int updateCard(Card *card, int k);
#endif // CARD_FILE_H_INCLUDED
