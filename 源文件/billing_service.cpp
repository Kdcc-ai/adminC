#define _CRT_SECURE_NO_WARNINGS//����scanf����
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include "card_service.h"
#include "card_file.h"
#include "time.h"
#include "billing_service.h"
#define UNIT 1 //��С�շѵ�Ԫ15����
#define CHARGE 0.5 //ÿ���շѵ�Ԫ�շ�0.5Ԫ
/*
������Ϣ����������ļ�*
*/
Billing* saveBilling(Card *card, time_t t, int hour, int min)
{
	time_t Start1 = card->tLast;
	struct tm *Start = localtime(&Start1);
	/*
	ͨ���ϻ�ʱ�����»�ʱ��������ѽ��
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
	������Ϣ�������ڴ�ſ�������Ϣ�ļ���
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
		printf("�»���Ϣд��ʧ�ܣ�");
		return NULL;
	}
	fclose(file);
	return bill;
}
/*
��ȡ������Ϣ������
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
		printf("�ļ���ȡʧ��");
	}
	fclose(file);
	return Info;
}

/*
�»�����
*/
void doSettle(Card *card)
{
	time_t t = time(NULL);
	struct tm *p = localtime(&t);
	int hour = p->tm_hour;
	int min = p->tm_min;
	/*
	���»���Ϣ���浽�������ļ���
	���ڹ���7:��ѯͳ��
	��ӡ�»���Ϣ �ı俨�ļ��п���Ϣ
	*/

	Billing *bill = (Billing*)malloc(sizeof(Billing));
	bill = saveBilling(card, t, hour, min);
	printf("-----�»���Ϣ����-----\n");
	printf("����\t����\t���\t�»�ʱ��\n");
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
��ѯ��Ӫҵ��
*/
float inQuiryTo()
{
	float Sum = 0;
	/*
	��ȡ������Ϣ �����Ӫҵ��
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
��ѯĳ�����ѽ��
*/
float inQuiryMon(Card *card)
{
	float Sum = 0;
	/*
	��ȡ������Ϣ ���űȽ� ��øÿ����ѽ��
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

