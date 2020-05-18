#define _CRT_SECURE_NO_WARNINGS//消除scanf警告
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include "card_service.h"
#include "card_file.h"
#include "time.h"
#include "billing_service.h"
#define UNIT 1 //最小收费单元15分钟
#define CHARGE 0.5 //每个收费单元收费0.5元
/*
消费信息存入二进制文件*
*/
Billing* saveBilling(Card *card, time_t t, int hour, int min)
{
	time_t Start1 = card->tLast;
	struct tm *Start = localtime(&Start1);
	/*
	通过上机时间与下机时间计算消费金额
	*/
	int hour1 = Start->tm_hour;
	int minute1 = Start->tm_min;
	int Beforetime = hour1 * 60 + minute1;
	int Endtime = hour * 60 + min;
	float fAmount = ((Endtime - Beforetime)*1.0 / UNIT)*CHARGE*1.0;
	float fBalance = card->fBalance;
	if (fBalance < fAmount)
	{
		return NULL;
	}
	/*
	消费信息存入用于存放卡消费信息文件中
	*/
	FILE *file;
	Billing *bill;
	bill = (Billing*)malloc(sizeof(Billing));
	strcpy(bill->aCardName, card->aName);
	bill->tStart = Start1;
	bill->tEnd = t;
	bill->Amount = fAmount;
	bill->fBalance = (card->fBalance) - fAmount;
	if ((file = fopen("D:\\billing.ams", "ab")) != NULL)
	{
		if (fwrite(bill, sizeof(struct Billing), 1, file) != 0);
	}
	else
	{
		printf("下机信息写入失败！");
		return NULL;
	}
	fclose(file);
	return bill;
}
/*
读取消费信息链表函数
*/
Billing *initial2()
{
	Billing *Info;
	Info = (Billing*)malloc(sizeof(Billing));
	Info->next = NULL;
	return Info;
}
Billing *readInfo()
{
	Billing *Info = (Billing*)malloc(sizeof(Billing));
	Info->next = NULL;
	FILE *file;
	file = fopen("D:\\billing.ams", "ab");
	fclose(file);
	int num = 0;
	if ((file = fopen("D:\\billing.ams", "rb")) != NULL)
	{
		while (!feof(file)) {
			Billing *bill;
			bill = (Billing*)malloc(sizeof(Billing));
			if (fread(bill, sizeof(struct Billing), 1, file) != 0);
			else
			{
				break;
			}
			bill->next = Info->next;
			Info->next = bill;
		}
	}
	else
	{
		printf("文件读取失败");
	}
	fclose(file);
	return Info;
}

/*
下机操作
*/
void doSettle(Card *card)
{
	time_t t = time(NULL);
	struct tm *p = localtime(&t);
	int hour = p->tm_hour;
	int min = p->tm_min;
	/*
	将下机信息保存到二进制文件中
	用于功能7:查询统计
	打印下机信息 改变卡文件中卡信息
	*/

	Billing *bill = (Billing*)malloc(sizeof(Billing));
	bill = saveBilling(card, t, hour, min);
	printf("-----下机信息如下-----\n");
	printf("卡号\t消费\t余额\t下机时间\n");
	printf("%s\t%.1f\t%.1f\t%4d%02d%02d-%02d:%02d\n",
		bill->aCardName, bill->Amount, bill->fBalance,
		p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, hour, min);


	card->fBalance = card->fBalance - bill->Amount;
	card->fTotalUse = card->fTotalUse + bill->Amount;
	card->tLast = t;
	int  k = updateCard(card, 2);
	return;
}
/*
查询总营业额
*/
float inQuiryTo()
{
	float Sum = 0;
	/*
	读取消费信息 获得总营业额
	*/
	Billing *Info = (Billing*)malloc(sizeof(Billing));
	Info = readInfo();
	Info = Info->next;
	while (Info != NULL)
	{
		Sum += Info->Amount;
		Info = Info->next;
	}
	return Sum;
}
/*
查询某卡消费金额
*/
float inQuiryMon(Card *card)
{
	float Sum = 0;
	/*
	读取消费信息 卡号比较 获得该卡消费金额
	*/
	Billing *Info = (Billing*)malloc(sizeof(Billing));
	Info = readInfo();
	while (Info != NULL)
	{
		if (strcmp(Info->aCardName, card->aName) == 0)
		{
			Sum += Info->Amount;
		}
		Info = Info->next;
	}
	return Sum;
}

