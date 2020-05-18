#define _CRT_SECURE_NO_WARNINGS//����scanf����
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include "card_service.h"
#include "time.h"
time_t ToTime(char s[]) {
	time_t t;
	struct tm tm1;
	int year, mon, day, hour, min, sec;
	tm1.tm_year = (s[0] - '0') * 1000 + (s[1] - '0') * 100 + (s[2] - '0') * 10 + s[3] - '0' - 1900;
	tm1.tm_mon = (s[4] - '0') * 10 + s[5] - '0' - 1;
	tm1.tm_mday = (s[6] - '0') * 10 + s[7] - '0';
	tm1.tm_hour = (s[9] - '0') * 10 + s[10] - '0';
	tm1.tm_min = (s[12] - '0') * 10 + s[13] - '0';
	tm1.tm_sec = (s[15] - '0') * 10 + s[16] - '0';
	t = mktime(&tm1);
	return t;
}//ͨ���˷�������time_t���ͱ���

/*
���ļ�����ӿ���������
*/
void saveCard(Card *card) {
	FILE *file;
	time_t t = time(NULL);
	struct tm *p = localtime(&t);
	if ((file = fopen("D:\\card.txt", "a")) != NULL) {
		fprintf(file, "%s\t%s\t%d\t%4d%02d%02d-%02d:%02d:%02d\t%4d%02d%02d-%02d:%02d:%02d\t%.1f\t%4d%02d%02d-%02d:%02d:%02d\t%d\t%.1f\t%d\n",
			card->aName, card->aPwd, card->nStatus, p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec,
			p->tm_year + 1901, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, card->fTotalUse,
			p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, card->nUseCount, card->fBalance, card->nDel);
	}
	else printf("�ļ���ʧ��!\n");
	fclose(file);
}
/*
���ļ��ж�����Ϣ����
*/
Card* readCard() {
	Card *card = initial();
	//����������
	FILE *file;
	file = fopen("D:\\card.txt", "a");
	fclose(file);
	//���ļ�ʱ�����Ƚ���һ�ļ� ���򱨴�
	char time_start[50], time_end[50], time_last[50];
	if ((file = fopen("D:\\card.txt", "r")) != NULL) {
		while (!feof(file)) {
			Card *t;
			t = (Card*)malloc(sizeof(Card));
			if (fscanf(file, "%s %s %d %s %s %f %s %d %f %d",
				t->aName, t->aPwd, &t->nStatus, time_start, time_end,
				&t->fTotalUse, time_last, &t->nUseCount, &t->fBalance, &t->nDel) == EOF) break;
			t->tStart = ToTime(time_start);
			t->tEnd = ToTime(time_end);
			t->tLast = ToTime(time_last);
			t->next = card->next;
			card->next = t;
		}
	}
	else {
		printf("�ļ���ȡʧ�ܣ�");
	}
	fclose(file);
	return card;
}
/*
�����ļ��п���Ϣ�ı亯��
�ɴ���k�Ĳ�ͬ�жϸı����(1��������ϻ� 2��������»� 3.������г�ֵ����)
*/
int updateCard(Card *card, int k)
{
	FILE *f1, *f2;
	time_t t2 = time(NULL);
	struct tm *p = localtime(&t2);
	time_t t_start, t_end, t_last;
	char time_start[50], time_end[50], time_last[50];

	Card *t = (Card*)malloc(sizeof(Card));
	f2 = fopen("D:\\card2.txt", "a");  //����ļ��������򴴽�һ�� д�ķ�ʽ������Ϣд��card2.txt

	if ((f1 = fopen("D:\\card.txt", "r+")) != NULL)//ֻ���ķ�ʽ
	{
		while (fscanf(f1, "%s %s %d %s %s %f %s %d %f %d",
			t->aName, t->aPwd, &t->nStatus, time_start, time_end,
			&t->fTotalUse, time_last, &t->nUseCount, &t->fBalance, &t->nDel) != EOF)
		{
			if (strcmp(card->aName, t->aName) == 0) {
				if (k == 1)//�ϻ�
				{
					fprintf(f2, "%s\t%s\t%d\t%s\t%s\t%.1f\t%4d%02d%02d-%02d:%02d:%02d\t%d\t%.1f\t%d\n",
						card->aName, card->aPwd, 1, time_start, time_end, card->fTotalUse,
						p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, card->nUseCount + 1, card->fBalance, 0);
					continue;
				}
				else if (k == 2)//�»�
				{
					fprintf(f2, "%s\t%s\t%d\t%s\t%s\t%.1f\t%4d%02d%02d-%02d:%02d:%02d\t%d\t%.1f\t%d\n",
						card->aName, card->aPwd, 0, time_start, time_end, card->fTotalUse,
						p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, card->nUseCount, card->fBalance, 0);
					continue;
				}
				else if (k == 2)//�»�
				{
					fprintf(f2, "%s\t%s\t%d\t%s\t%s\t%.1f\t%4d%02d%02d-%02d:%02d:%02d\t%d\t%.1f\t%d\n",
						card->aName, card->aPwd, 0, time_start, time_end, card->fTotalUse,
						p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, card->nUseCount, card->fBalance, 0);
					continue;
				}
				else if (k == 3)//��ֵ
				{
					fprintf(f2, "%s\t%s\t%d\t%s\t%s\t%.1f\t%4d%02d%02d-%02d:%02d:%02d\t%d\t%.1f\t%d\n",
						card->aName, card->aPwd, card->nStatus, time_start, time_end, card->fTotalUse,
						p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, card->nUseCount, card->fBalance, 0);
					continue;
				}
				else if (k == 4)//�˷�
				{
					fprintf(f2, "%s\t%s\t%d\t%s\t%s\t%.1f\t%4d%02d%02d-%02d:%02d:%02d\t%d\t%.1f\t%d\n",
						card->aName, card->aPwd, 2, time_start, time_end, card->fTotalUse,
						p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, card->nUseCount, card->fBalance, 1);
					continue;
				}
				else if (k == 5)//ɾ����Ƭ
				{
					fprintf(f2, "%s\t%s\t%d\t%s\t%s\t%.1f\t%4d%02d%02d-%02d:%02d:%02d\t%d\t%.1f\t%d\n",
						card->aName, card->aPwd, 2, time_start, time_end, card->fTotalUse,
						p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, card->nUseCount, card->fBalance, 1);
					continue;
				}
				fprintf(f2, "%s\t%s\t%d\t%s\t%s\t%.1f\t%s\t%d\t%.1f\t%d\n",
					t->aName, t->aPwd, t->nStatus, time_start, time_end, t->fTotalUse,
					time_last, t->nUseCount, t->fBalance, t->nDel);
			}
			//f2���������ļ��о��Ǹ��º����Ϣ��
			else fprintf(f2, "%s\t%s\t%d\t%s\t%s\t%.1f\t%s\t%d\t%.1f\t%d\n",
				t->aName, t->aPwd, t->nStatus, time_start, time_end, t->fTotalUse,
				time_last, t->nUseCount, t->fBalance, t->nDel);
		}
	}
	else
	{
		printf("�ļ���ʧ��");
		return 0;
	}
	fclose(f1);
	fclose(f2);

	f1 = fopen("D:\\card2.txt", "r+");  //��card2.txt
	f2 = fopen("D:\\card.txt", "w");  //дcard.txt
	while (fscanf(f1, "%s %s %d %s %s %f %s %d %f %d",
		t->aName, t->aPwd, &t->nStatus, time_start, time_end,
		&t->fTotalUse, time_last, &t->nUseCount, &t->fBalance, &t->nDel) != EOF)
	{
		fprintf(f2, "%s\t%s\t%d\t%s\t%s\t%.1f\t%s\t%d\t%.1f\t%d\n",
			t->aName, t->aPwd, t->nStatus, time_start, time_end, t->fTotalUse,
			time_last, t->nUseCount, t->fBalance, t->nDel);
	}
	fclose(f1);
	fclose(f2);
	return 1;
}
