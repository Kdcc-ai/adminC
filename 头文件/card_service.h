#ifndef CARD_SERVICE_H_INCLUDED
#define CARD_SERVICE_H_INCLUDED
typedef struct Card {
	char      aName[18];  //����
	char      aPwd[8];       //����
	int         nStatus;         //��״̬��0-δ�ϻ���1-�����ϻ���2-��ע����3-ʧЧ��
	time_t    tStart;            //����ʱ��
	time_t    tEnd;             //���Ľ�ֹʱ��
	float       fTotalUse;     //�ۼƽ��
	time_t    tLast;            //���ʹ��ʱ��
	int          nUseCount;  //ʹ�ô���
	float       fBalance;      //���
	int          nDel;            //ɾ����ʶ 0-δɾ����1-ɾ��
	struct Card* next;            //ָ����һ������ָ��
}Card;
void addCard(Card *card, char no[], char pwd[], float balance);
Card* initial();
Card* queryCard(Card *card, char no[]);
Card *queryCardCon(Card *card, char *name, int *index);
void showchange(Card *card, int k);
int delCard(Card *card);
#endif // CARD_SERVICE_H_INCLUDED
