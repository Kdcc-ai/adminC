#define _CRT_SECURE_NO_WARNINGS//消除scanf警告
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include "card_file.h"
#include "card_service.h"
#include "time.h"
/*
 用于上机 充值 退费
*/
void doLogon(Card *card, int k)//上机
{
	int  m = 1;
	showchange(card, m);
}
int doaddMoney(Card *card, float money)//充值
{
	int m = 3;
	card->fBalance = card->fBalance + money;//改变该卡余额
	//调用card_file.cpp文件中更新卡信息函数
	int ye = updateCard(card, 3);
	if (ye == 1)
		return 1;
	else
		return 0;
}
int dofeFundMoney(Card *card)//退费
{
	int m = 4;
	if (card->nStatus == 2 || card->nStatus == 1)
		return 0;
	//调用card_file.cpp文件中更新卡信息函数
	int ye = updateCard(card, 4);
	if (ye == 1)
		return 1;
	else
		return 0;
}

