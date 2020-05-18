#define _CRT_SECURE_NO_WARNINGS//消除scanf警告
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include "card_service.h"
#include "card_file.h"
#include "time.h"
/*
增加卡信息操作函数
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
卡信息链表初始化操作
*/
Card* initial() {
	Card *card;
	card = (Card*)malloc(sizeof(Card));
	card->next = NULL;
	return card;
}
/*
查询卡片操作
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
	模糊查询
	所有信息到卡链表中
	*/
	Card *s = initial();
	while (card != NULL)
	{
		/*
		通过strstr函数模糊查询
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
上机操作
*/
void showchange(Card *card, int k)
{
	printf("----上机成功----\n");
	printf("卡号\t余额\t上机时间\n");
	time_t t = time(NULL);
	struct tm *p = localtime(&t);
	printf("%s\t%.1f\t%4d-%02d-%02d %02d:%02d:%02d\n",
		card->aName, card->fBalance, p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
	//向用户展示信息。

	//更新链表中的卡信息
	Card *card2 = initial();
	card2 = readCard();
	while (card2 != NULL)
	{
		if (strcmp(card2->aName, card->aName) == 0)
		{
			card2->nStatus = 1;//表示正在上机
			card2->tLast = t;//更新上一次上机时间
		}
		card2 = card2->next;
	}

	//更新文件中的卡信息
	int c = 1;
	//c=1代表从文件中改变为成上机状态
	//调用card_file.cpp文件中更新文件卡信息操作
	updateCard(card, c);

}
/*
删除卡片
*/
int delCard(Card *card2)
{
	int k = 5;
	//k=5代表从文件中改变为注销状态
	//调用card_file.cpp文件中注销卡信息操作
	if (updateCard(card2, k) == 1)
		return 1;
	else
		return 0;
}

