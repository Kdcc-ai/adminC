#define _CRT_SECURE_NO_WARNINGS//消除scanf警告
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
}//通过此方法返回time_t类型变量

/*
向文件中添加卡操作函数
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
	else printf("文件打开失败!\n");
	fclose(file);
}
/*
由文件中读卡信息函数
*/
Card* readCard() {
	Card *card = initial();
	//获得链表起点
	FILE *file;
	file = fopen("D:\\card.txt", "a");
	fclose(file);
	//读文件时必须先建立一文件 否则报错
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
		printf("文件读取失败！");
	}
	fclose(file);
	return card;
}
/*
进行文件中卡信息改变函数
由传入k的不同判断改变情况(1代表进行上机 2代表进行下机 3.代表进行充值。。)
*/
int updateCard(Card *card, int k)
{
	FILE *f1, *f2;
	time_t t2 = time(NULL);
	struct tm *p = localtime(&t2);
	time_t t_start, t_end, t_last;
	char time_start[50], time_end[50], time_last[50];

	Card *t = (Card*)malloc(sizeof(Card));
	f2 = fopen("D:\\card2.txt", "a");  //如果文件不存在则创建一个 写的方式将新信息写入card2.txt

	if ((f1 = fopen("D:\\card.txt", "r+")) != NULL)//只读的方式
	{
		while (fscanf(f1, "%s %s %d %s %s %f %s %d %f %d",
			t->aName, t->aPwd, &t->nStatus, time_start, time_end,
			&t->fTotalUse, time_last, &t->nUseCount, &t->fBalance, &t->nDel) != EOF)
		{
			if (strcmp(card->aName, t->aName) == 0) {
				if (k == 1)//上机
				{
					fprintf(f2, "%s\t%s\t%d\t%s\t%s\t%.1f\t%4d%02d%02d-%02d:%02d:%02d\t%d\t%.1f\t%d\n",
						card->aName, card->aPwd, 1, time_start, time_end, card->fTotalUse,
						p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, card->nUseCount + 1, card->fBalance, 0);
					continue;
				}
				else if (k == 2)//下机
				{
					fprintf(f2, "%s\t%s\t%d\t%s\t%s\t%.1f\t%4d%02d%02d-%02d:%02d:%02d\t%d\t%.1f\t%d\n",
						card->aName, card->aPwd, 0, time_start, time_end, card->fTotalUse,
						p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, card->nUseCount, card->fBalance, 0);
					continue;
				}
				else if (k == 2)//下机
				{
					fprintf(f2, "%s\t%s\t%d\t%s\t%s\t%.1f\t%4d%02d%02d-%02d:%02d:%02d\t%d\t%.1f\t%d\n",
						card->aName, card->aPwd, 0, time_start, time_end, card->fTotalUse,
						p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, card->nUseCount, card->fBalance, 0);
					continue;
				}
				else if (k == 3)//充值
				{
					fprintf(f2, "%s\t%s\t%d\t%s\t%s\t%.1f\t%4d%02d%02d-%02d:%02d:%02d\t%d\t%.1f\t%d\n",
						card->aName, card->aPwd, card->nStatus, time_start, time_end, card->fTotalUse,
						p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, card->nUseCount, card->fBalance, 0);
					continue;
				}
				else if (k == 4)//退费
				{
					fprintf(f2, "%s\t%s\t%d\t%s\t%s\t%.1f\t%4d%02d%02d-%02d:%02d:%02d\t%d\t%.1f\t%d\n",
						card->aName, card->aPwd, 2, time_start, time_end, card->fTotalUse,
						p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, card->nUseCount, card->fBalance, 1);
					continue;
				}
				else if (k == 5)//删除卡片
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
			//f2所关联的文件中就是更新后的信息。
			else fprintf(f2, "%s\t%s\t%d\t%s\t%s\t%.1f\t%s\t%d\t%.1f\t%d\n",
				t->aName, t->aPwd, t->nStatus, time_start, time_end, t->fTotalUse,
				time_last, t->nUseCount, t->fBalance, t->nDel);
		}
	}
	else
	{
		printf("文件打开失败");
		return 0;
	}
	fclose(f1);
	fclose(f2);

	f1 = fopen("D:\\card2.txt", "r+");  //读card2.txt
	f2 = fopen("D:\\card.txt", "w");  //写card.txt
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
