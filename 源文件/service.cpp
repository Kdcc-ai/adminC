#define _CRT_SECURE_NO_WARNINGS//����scanf����
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include "card_file.h"
#include "card_service.h"
#include "time.h"
/*
 �����ϻ� ��ֵ �˷�
*/
void doLogon(Card *card, int k)//�ϻ�
{
	int  m = 1;
	showchange(card, m);
}
int doaddMoney(Card *card, float money)//��ֵ
{
	int m = 3;
	card->fBalance = card->fBalance + money;//�ı�ÿ����
	//����card_file.cpp�ļ��и��¿���Ϣ����
	int ye = updateCard(card, 3);
	if (ye == 1)
		return 1;
	else
		return 0;
}
int dofeFundMoney(Card *card)//�˷�
{
	int m = 4;
	if (card->nStatus == 2 || card->nStatus == 1)
		return 0;
	//����card_file.cpp�ļ��и��¿���Ϣ����
	int ye = updateCard(card, 4);
	if (ye == 1)
		return 1;
	else
		return 0;
}

