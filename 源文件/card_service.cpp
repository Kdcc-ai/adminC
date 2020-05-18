#define _CRT_SECURE_NO_WARNINGS//����scanf����
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include "card_service.h"
#include "card_file.h"
#include "time.h"
/*
���ӿ���Ϣ��������
*/
void addCard(Card *card, char no[], char pwd[], float balance) {
	Card *t;
	t = (Card*)malloc(sizeof(Card));
	strcpy(t->aName, no);
	strcpy(t->aPwd, pwd);
	t->nStatus = 0;
	t->fTotalUse = 0;
	t->fBalance = balance;
	t->nUseCount = 0;
	t->nDel = 0;
	t->next = card->next;
	card->next = t;
	saveCard(t);
}
/*
����Ϣ�����ʼ������
*/
Card* initial() {
	Card *card;
	card = (Card*)malloc(sizeof(Card));
	card->next = NULL;
	return card;
}
/*
��ѯ��Ƭ����
*/
Card* queryCard(Card *card, char no[]) {
	while (card != NULL)
	{
		if (strcmp(card->aName, no) == 0)
		{
			return card;
		}
		card = card->next;
	}
	return NULL;
}
Card *queryCardCon(Card *card, char *name, int *index)
{
	/*
	ģ����ѯ
	������Ϣ����������
	*/
	Card *s = initial();
	while (card != NULL)
	{
		/*
		ͨ��strstr����ģ����ѯ
		*/
		if (strstr(card->aName, name) != NULL)
		{
			Card *tmp;
			tmp = (Card*)malloc(sizeof(Card));
			strcpy(tmp->aName, card->aName);
			strcpy(tmp->aPwd, card->aPwd);
			tmp->tEnd = card->tEnd;
			tmp->fBalance = card->fBalance;
			tmp->tLast = card->tLast;
			tmp->nDel = card->nDel;
			tmp->nStatus = card->nStatus;
			tmp->nUseCount = card->nUseCount;
			tmp->tStart = card->tStart;
			tmp->fTotalUse = card->fTotalUse;
			tmp->next = s->next;
			s->next = tmp;
			(*index)++;
		}
		card = card->next;
	}
	return s;
}
/*
�ϻ�����
*/
void showchange(Card *card, int k)
{
	printf("----�ϻ��ɹ�----\n");
	printf("����\t���\t�ϻ�ʱ��\n");
	time_t t = time(NULL);
	struct tm *p = localtime(&t);
	printf("%s\t%.1f\t%4d-%02d-%02d %02d:%02d:%02d\n",
		card->aName, card->fBalance, p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
	//���û�չʾ��Ϣ��

	//���������еĿ���Ϣ
	Card *card2 = initial();
	card2 = readCard();
	while (card2 != NULL)
	{
		if (strcmp(card2->aName, card->aName) == 0)
		{
			card2->nStatus = 1;//��ʾ�����ϻ�
			card2->tLast = t;//������һ���ϻ�ʱ��
		}
		card2 = card2->next;
	}

	//�����ļ��еĿ���Ϣ
	int c = 1;
	//c=1������ļ��иı�Ϊ���ϻ�״̬
	//����card_file.cpp�ļ��и����ļ�����Ϣ����
	updateCard(card, c);

}
/*
ɾ����Ƭ
*/
int delCard(Card *card2)
{
	int k = 5;
	//k=5������ļ��иı�Ϊע��״̬
	//����card_file.cpp�ļ���ע������Ϣ����
	if (updateCard(card2, k) == 1)
		return 1;
	else
		return 0;
}

