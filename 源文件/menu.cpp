#define _CRT_SECURE_NO_WARNINGS//消除scanf警告
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include"menu.h"
#include"card_service.h"
#include"card_file.h"
#include"service.h"
#include"billing_service.h"
#include"time.h"
Card *card = initial();
//与用户交互界面
void outputmenu() {
	do {
		int choice;
		printf("欢迎进入计费管理系统\n");
		printf("1.添加卡\n");
		printf("2.查询卡\n");
		printf("3.上机\n");
		printf("4.下机\n");
		printf("5.充值\n");
		printf("6.退费\n");
		printf("7.查询统计\n");
		printf("8.注销卡\n");
		printf("0.退出\n");
		printf("请输入0-8数字来进行选择\n");
		scanf("%d", &choice);
		switch (choice) {
		case 0: {
			exit(0);
			break;
		}
		case 1: {
			system("cls");
			add();
			system("pause");
			break;
		}
		case 2: {
			system("cls");
			query();
			system("pause");
			break;
		}
		case 3: {
			system("cls");
			login();
			system("pause");
			break;
		}
		case 4: {
			system("cls");
			Settle();
			system("pause");
			break;
		}
		case 5: {
			system("cls");
			addMoney();
			system("pause");
			break;
		}
		case 6: {
			system("cls");
			reFundMoney();
			system("pause");
			break;
		}
		case 7: {
			system("cls");
			inQuirystat();
			system("pause");
			break;
		}
		case 8: {
			system("cls");
			del();
			system("pause");
			break;
		}
		default: {
			system("cls");
			printf("请输入正确的1-8数字\n");
			break;
		}
		}
	} while (1);
}

/*
增加卡片信息
*/
void add() {
	card = readCard();
	//先从文件中读取卡的链表
	printf("请输入您要添加的卡号<长度1-18>:\n");
	char name[50];
	scanf("%s", name);
	while (strlen(name) > 18 || queryCard(card, name) != NULL) {
		if (strlen(name) > 18) printf("卡号大于18位 重新输入：\n");
		else printf("卡号已存在 重新输入：\n");
		scanf("%s", name);
	}
	char pwd[50];
	printf("请输入密码<长度1-8>:\n");
	scanf("%s", pwd);
	while (strlen(pwd) > 8 || strlen(pwd) < 1) {
		printf("密码超过8位，请重新输入密码<长度1-8>:\n");
		scanf("%s", pwd);
	}
	float balance;
	printf("请输入开卡金额<人民币>:\n");
	scanf("%f", &balance);
	printf("添加卡的信息如下:\n");
	printf("卡号\t密码\t状态\t金额\n");
	printf("%s\t%s\t0\t%.1f\n", name, pwd, balance);

	//卡信息存入卡文件中
	addCard(card, name, pwd, balance);
}

/*
查询卡片信息
*/
void query()
{
	card = readCard();
	printf("请您输入要查询的卡号<长度为1~18位> ");
	char aName[50];
	scanf("%s", aName);
	while (strlen(aName) > 18)
	{
		printf("所输入查询卡号有误 重新输入\n");
		scanf("%s", aName);
	}
	Card *card2 = queryCard(card, aName);
	if (card == NULL)
	{
		printf("不存在该卡号卡片\n");
		return;
	}
	else {
		printf("卡号\t状态\t余额\t累计使用\t使用次数\t上次使用时间\n");
		int index = 0;
		Card *CardNo = queryCardCon(card, aName, &index);
		int k = 0;
		CardNo = CardNo->next;
		while (CardNo != NULL)
		{
			struct tm *p;
			p = gmtime(&(CardNo->tLast));//获取tm数组
			printf("%s\t%d\t%.1f\t%.1f\t\t\t%d\t%04d-%02d-%02d %02d:%02d\n",
				CardNo->aName, CardNo->nStatus, CardNo->fBalance, CardNo->fTotalUse, CardNo->nUseCount,
				1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min);
			CardNo = CardNo->next;
		}
	}
}

void login()
{
	int t = 0;
	//先从文件中读取卡的链表
	card = readCard();
	printf("请输入上机卡号<长度为1~18> \n");
	char nam[50];
	scanf("%s", nam);
	printf("请输入上机密码<c长度为1~8> \n");
	char pas[20];
	scanf("%s", pas);
	if (strlen(nam) > 18)
	{
		printf("卡号超过18位 重新输入\n");
	}
	Card *car;
	car = queryCard(card, nam);
	if (car != NULL && strcmp(car->aPwd, pas) == 0)
	{
		printf("您输入的卡号密码正确 正在为您验证卡信息\n");
	}
	if (car == NULL)
	{
		printf("不存在该卡\n");
		t = 1;
		return;
	}
	if (car->nStatus == 1)
	{
		printf("该卡正在上机\n");
		t = 1;
	}
	if (car->nStatus == 2)
	{
		printf("该卡已注销\n");
		t = 1;
	}
	if (car->nStatus == 3)
	{
		printf("该卡已失效\n");
		t = 1;
	}
	if (car->fBalance <= 0)
	{
		printf("该卡余额小于0\n");
		t = 1;
	}
	if (t != 1)
	{
		int k = 1;
		doLogon(car, k);
	}
	else
	{
		printf("上机失败\n");
		return;
	}
}
/*
下机操作
*/
void Settle()
{
	card = readCard();
	//从文件中读取文件中卡信息
	char name[50];
	printf("请输入您要下机的卡号\n");
	scanf("%s", name);
	if (strlen(name) > 18)
	{
		printf("卡号大于18位 查询失败\n");
		return;
	}
	Card *card2;
	//调用card_service.cpp文件 卡查询函数
	card2 = queryCard(card, name);
	if (card2 == NULL)
	{
		printf("不存在该卡 下机失败！\n");
		return;
	}
	printf("请输入您要下机卡号的密码\n");
	char pas[20];
	scanf("%s", pas);
	if (strcmp(card2->aPwd, pas) == 0)
	{
		printf("您输入的卡号密码正确 正在为您验证卡信息\n");
	}
	else
	{
		printf("您输入的卡号密码错误 下机失败！\n");
		return;
	}
	if (card2->nStatus != 1)
	{
		printf("您要下机的卡号目前不在上机 下机失败！\n");
		return;
	}
	//卡号密码正确 调用billing_service.cpp下机操作函数
	doSettle(card2);
	return;
}

/*
充值操作
*/
void addMoney()
{
	card = readCard();
	//从文件中读取文件中卡信息
	char name[50];
	printf("请输入您要充值的卡号\n");
	scanf("%s", name);
	if (strlen(name) > 18)
	{
		printf("您输入卡号大于18位 充值失败！\n");
		return;
	}
	if (queryCard(card, name) == NULL)
	{
		printf("不存在该卡号对应卡片 充值失败！\n");
		return;
	}
	printf("请输入您要充值卡号的密码\n");
	char pas[20];
	scanf("%s", pas);
	Card *card2;
	card2 = (Card*)malloc(sizeof(Card));
	//调用card_service.cpp文件 卡查询函数
	card2 = queryCard(card, name);
	if (strcmp(card2->aPwd, pas) != 0)
	{
		printf("您输入卡号密码错误 充值失败! \n");
		return;
	}
	printf("请输入充值金额\n");
	float money;
	scanf("%f", &money);
	//卡号密码正确 调用service.cpp充值操作函数
	int ye = doaddMoney(card2, money);
	if (ye == 1)
	{
		printf("------充值信息如下------\n");
		printf("卡号\t充值金额   余额\n");
		printf("%s\t%.1f\t  %.1f\n", card2->aName, money, card2->fBalance);
		return;
	}
	else
	{
		printf("充值失败\n");
		return;
	}
}

/*
退费操作
*/
void reFundMoney()
{
	card = readCard();
	//从文件中读取文件中卡信息
	char name[50];
	printf("请输入您要退款的卡号\n");
	scanf("%s", name);
	if (strlen(name) > 18)
	{
		printf("您输入卡号大于18位 退费失败！\n");
		return;
	}
	if (queryCard(card, name) == NULL)
	{
		printf("不存在该卡号对应卡片 退费失败！\n");
		return;
	}
	printf("请输入您要退费卡号的密码\n");
	char pas[20];
	scanf("%s", pas);
	Card *card2;
	card2 = (Card*)malloc(sizeof(Card));
	card2 = queryCard(card, name);
	if (strcmp(card2->aPwd, pas) != 0)
	{
		printf("您输入卡号密码错误 退费失败! \n");
		return;
	}
	//卡号密码正确 调用service.cpp下机操作函数
	int ye = dofeFundMoney(card2);
	if (ye == 1)
	{
		printf("------退费信息如下------\n");
		printf("卡号\t退费金额    余额\n");
		printf("%s\t%.1f\t  %.1f", card2->aName, card2->fBalance, 0);
	}
	else
	{
		printf("该卡已失效或正在上机 退费失败！\n");
		return;
	}
}

/*
查询统计操作
*/
void inQuirystat()
{
	Card *card = readCard();
	//从文件中读取文件中卡信息
	printf("1.查询某卡消费记录\n");
	printf("2.统计总营业额\n");
	printf("输入您的选择:\n");
	int choice;
	scanf("%d", &choice);
	switch (choice) {
	case 1:
	{
		printf("请您输入要查询的卡号\n");
		char name[50];
		scanf("%s", name);
		if (strlen(name) > 18)
		{
			printf("您输入卡号大于18位 退费失败！\n");
			return;
		}
		if (queryCard(card, name) == NULL)
		{
			printf("不存在该卡号对应卡片 退费失败！\n");
			return;
		}
		printf("请输入您要退费卡号的密码\n");
		char pas[20];
		scanf("%s", pas);
		Card *card2;
		card2 = (Card*)malloc(sizeof(Card));
		card2 = queryCard(card, name);
		if (strcmp(card2->aPwd, pas) != 0)
		{
			printf("您输入卡号密码错误 查询失败! \n");
			return;
		}
		//卡号密码正确 调用billing_service.cpp查询该卡消费总额操作函数
		float Total = inQuiryMon(card2);
		printf("该卡消费总金额为\n");
		printf("%.1f\n", Total);
		break;
	}
	case 2:
	{
		//卡号密码正确 调用billing_service.cpp查询总营业额操作函数
		float TotalMon = inQuiryTo();
		printf("总营业额为\n");
		printf("%.1f\n", TotalMon);
		break;
	}
	default:
	{
		printf("输入选择错误 查询统计失败！\n");
		return;
	}
	}
}

/*
删除操作
*/
void del()
{
	card = readCard();
	int t = 0;
	char name[50];
	printf("请输入您要注销的卡号\n");
	scanf("%s", name);
	char pas[20];
	printf("请输入您要主要的卡号的密码\n");
	scanf("%s", pas);
	Card *card2;
	card2 = (Card*)malloc(sizeof(Card));
	while (card != NULL)
	{
		if (queryCard(card, name) != NULL)
		{
			card2 = queryCard(card, name);
			printf("找到对应卡号密码 正在查询卡信息\n");
		}
		else
		{
			card = card->next;
			continue;
		}
		if (card->nStatus == 1)
		{
			printf("该卡正在使用 不可注销");
			break;
		}
		if (card->nStatus == 2)
		{
			printf("该卡已注销");
			break;
		}
		t = 1;
		break;
	}
	if (t == 1)
	{   //调用card_service.cpp注销卡片操作函数
		int ye = delCard(card2);
		if (ye == 1)
		{
			printf("卡号\t退还金额\n");
			printf("%s\t%.1f", card2->aName, card2->fBalance);
			printf("\n");
			printf("注销成功\n");
		}
		else
		{
			printf("注销失败\n");
		}
	}
}

