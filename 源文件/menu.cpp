#define _CRT_SECURE_NO_WARNINGS//����scanf����
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
//���û���������
void outputmenu() {
	do {
		int choice;
		printf("��ӭ����Ʒѹ���ϵͳ\n");
		printf("1.��ӿ�\n");
		printf("2.��ѯ��\n");
		printf("3.�ϻ�\n");
		printf("4.�»�\n");
		printf("5.��ֵ\n");
		printf("6.�˷�\n");
		printf("7.��ѯͳ��\n");
		printf("8.ע����\n");
		printf("0.�˳�\n");
		printf("������0-8����������ѡ��\n");
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
			printf("��������ȷ��1-8����\n");
			break;
		}
		}
	} while (1);
}

/*
���ӿ�Ƭ��Ϣ
*/
void add() {
	card = readCard();
	//�ȴ��ļ��ж�ȡ��������
	printf("��������Ҫ��ӵĿ���<����1-18>:\n");
	char name[50];
	scanf("%s", name);
	while (strlen(name) > 18 || queryCard(card, name) != NULL) {
		if (strlen(name) > 18) printf("���Ŵ���18λ �������룺\n");
		else printf("�����Ѵ��� �������룺\n");
		scanf("%s", name);
	}
	char pwd[50];
	printf("����������<����1-8>:\n");
	scanf("%s", pwd);
	while (strlen(pwd) > 8 || strlen(pwd) < 1) {
		printf("���볬��8λ����������������<����1-8>:\n");
		scanf("%s", pwd);
	}
	float balance;
	printf("�����뿪�����<�����>:\n");
	scanf("%f", &balance);
	printf("��ӿ�����Ϣ����:\n");
	printf("����\t����\t״̬\t���\n");
	printf("%s\t%s\t0\t%.1f\n", name, pwd, balance);

	//����Ϣ���뿨�ļ���
	addCard(card, name, pwd, balance);
}

/*
��ѯ��Ƭ��Ϣ
*/
void query()
{
	card = readCard();
	printf("��������Ҫ��ѯ�Ŀ���<����Ϊ1~18λ> ");
	char aName[50];
	scanf("%s", aName);
	while (strlen(aName) > 18)
	{
		printf("�������ѯ�������� ��������\n");
		scanf("%s", aName);
	}
	Card *card2 = queryCard(card, aName);
	if (card == NULL)
	{
		printf("�����ڸÿ��ſ�Ƭ\n");
		return;
	}
	else {
		printf("����\t״̬\t���\t�ۼ�ʹ��\tʹ�ô���\t�ϴ�ʹ��ʱ��\n");
		int index = 0;
		Card *CardNo = queryCardCon(card, aName, &index);
		int k = 0;
		CardNo = CardNo->next;
		while (CardNo != NULL)
		{
			struct tm *p;
			p = gmtime(&(CardNo->tLast));//��ȡtm����
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
	//�ȴ��ļ��ж�ȡ��������
	card = readCard();
	printf("�������ϻ�����<����Ϊ1~18> \n");
	char nam[50];
	scanf("%s", nam);
	printf("�������ϻ�����<c����Ϊ1~8> \n");
	char pas[20];
	scanf("%s", pas);
	if (strlen(nam) > 18)
	{
		printf("���ų���18λ ��������\n");
	}
	Card *car;
	car = queryCard(card, nam);
	if (car != NULL && strcmp(car->aPwd, pas) == 0)
	{
		printf("������Ŀ���������ȷ ����Ϊ����֤����Ϣ\n");
	}
	if (car == NULL)
	{
		printf("�����ڸÿ�\n");
		t = 1;
		return;
	}
	if (car->nStatus == 1)
	{
		printf("�ÿ������ϻ�\n");
		t = 1;
	}
	if (car->nStatus == 2)
	{
		printf("�ÿ���ע��\n");
		t = 1;
	}
	if (car->nStatus == 3)
	{
		printf("�ÿ���ʧЧ\n");
		t = 1;
	}
	if (car->fBalance <= 0)
	{
		printf("�ÿ����С��0\n");
		t = 1;
	}
	if (t != 1)
	{
		int k = 1;
		doLogon(car, k);
	}
	else
	{
		printf("�ϻ�ʧ��\n");
		return;
	}
}
/*
�»�����
*/
void Settle()
{
	card = readCard();
	//���ļ��ж�ȡ�ļ��п���Ϣ
	char name[50];
	printf("��������Ҫ�»��Ŀ���\n");
	scanf("%s", name);
	if (strlen(name) > 18)
	{
		printf("���Ŵ���18λ ��ѯʧ��\n");
		return;
	}
	Card *card2;
	//����card_service.cpp�ļ� ����ѯ����
	card2 = queryCard(card, name);
	if (card2 == NULL)
	{
		printf("�����ڸÿ� �»�ʧ�ܣ�\n");
		return;
	}
	printf("��������Ҫ�»����ŵ�����\n");
	char pas[20];
	scanf("%s", pas);
	if (strcmp(card2->aPwd, pas) == 0)
	{
		printf("������Ŀ���������ȷ ����Ϊ����֤����Ϣ\n");
	}
	else
	{
		printf("������Ŀ���������� �»�ʧ�ܣ�\n");
		return;
	}
	if (card2->nStatus != 1)
	{
		printf("��Ҫ�»��Ŀ���Ŀǰ�����ϻ� �»�ʧ�ܣ�\n");
		return;
	}
	//����������ȷ ����billing_service.cpp�»���������
	doSettle(card2);
	return;
}

/*
��ֵ����
*/
void addMoney()
{
	card = readCard();
	//���ļ��ж�ȡ�ļ��п���Ϣ
	char name[50];
	printf("��������Ҫ��ֵ�Ŀ���\n");
	scanf("%s", name);
	if (strlen(name) > 18)
	{
		printf("�����뿨�Ŵ���18λ ��ֵʧ�ܣ�\n");
		return;
	}
	if (queryCard(card, name) == NULL)
	{
		printf("�����ڸÿ��Ŷ�Ӧ��Ƭ ��ֵʧ�ܣ�\n");
		return;
	}
	printf("��������Ҫ��ֵ���ŵ�����\n");
	char pas[20];
	scanf("%s", pas);
	Card *card2;
	card2 = (Card*)malloc(sizeof(Card));
	//����card_service.cpp�ļ� ����ѯ����
	card2 = queryCard(card, name);
	if (strcmp(card2->aPwd, pas) != 0)
	{
		printf("�����뿨��������� ��ֵʧ��! \n");
		return;
	}
	printf("�������ֵ���\n");
	float money;
	scanf("%f", &money);
	//����������ȷ ����service.cpp��ֵ��������
	int ye = doaddMoney(card2, money);
	if (ye == 1)
	{
		printf("------��ֵ��Ϣ����------\n");
		printf("����\t��ֵ���   ���\n");
		printf("%s\t%.1f\t  %.1f\n", card2->aName, money, card2->fBalance);
		return;
	}
	else
	{
		printf("��ֵʧ��\n");
		return;
	}
}

/*
�˷Ѳ���
*/
void reFundMoney()
{
	card = readCard();
	//���ļ��ж�ȡ�ļ��п���Ϣ
	char name[50];
	printf("��������Ҫ�˿�Ŀ���\n");
	scanf("%s", name);
	if (strlen(name) > 18)
	{
		printf("�����뿨�Ŵ���18λ �˷�ʧ�ܣ�\n");
		return;
	}
	if (queryCard(card, name) == NULL)
	{
		printf("�����ڸÿ��Ŷ�Ӧ��Ƭ �˷�ʧ�ܣ�\n");
		return;
	}
	printf("��������Ҫ�˷ѿ��ŵ�����\n");
	char pas[20];
	scanf("%s", pas);
	Card *card2;
	card2 = (Card*)malloc(sizeof(Card));
	card2 = queryCard(card, name);
	if (strcmp(card2->aPwd, pas) != 0)
	{
		printf("�����뿨��������� �˷�ʧ��! \n");
		return;
	}
	//����������ȷ ����service.cpp�»���������
	int ye = dofeFundMoney(card2);
	if (ye == 1)
	{
		printf("------�˷���Ϣ����------\n");
		printf("����\t�˷ѽ��    ���\n");
		printf("%s\t%.1f\t  %.1f", card2->aName, card2->fBalance, 0);
	}
	else
	{
		printf("�ÿ���ʧЧ�������ϻ� �˷�ʧ�ܣ�\n");
		return;
	}
}

/*
��ѯͳ�Ʋ���
*/
void inQuirystat()
{
	Card *card = readCard();
	//���ļ��ж�ȡ�ļ��п���Ϣ
	printf("1.��ѯĳ�����Ѽ�¼\n");
	printf("2.ͳ����Ӫҵ��\n");
	printf("��������ѡ��:\n");
	int choice;
	scanf("%d", &choice);
	switch (choice) {
	case 1:
	{
		printf("��������Ҫ��ѯ�Ŀ���\n");
		char name[50];
		scanf("%s", name);
		if (strlen(name) > 18)
		{
			printf("�����뿨�Ŵ���18λ �˷�ʧ�ܣ�\n");
			return;
		}
		if (queryCard(card, name) == NULL)
		{
			printf("�����ڸÿ��Ŷ�Ӧ��Ƭ �˷�ʧ�ܣ�\n");
			return;
		}
		printf("��������Ҫ�˷ѿ��ŵ�����\n");
		char pas[20];
		scanf("%s", pas);
		Card *card2;
		card2 = (Card*)malloc(sizeof(Card));
		card2 = queryCard(card, name);
		if (strcmp(card2->aPwd, pas) != 0)
		{
			printf("�����뿨��������� ��ѯʧ��! \n");
			return;
		}
		//����������ȷ ����billing_service.cpp��ѯ�ÿ������ܶ��������
		float Total = inQuiryMon(card2);
		printf("�ÿ������ܽ��Ϊ\n");
		printf("%.1f\n", Total);
		break;
	}
	case 2:
	{
		//����������ȷ ����billing_service.cpp��ѯ��Ӫҵ���������
		float TotalMon = inQuiryTo();
		printf("��Ӫҵ��Ϊ\n");
		printf("%.1f\n", TotalMon);
		break;
	}
	default:
	{
		printf("����ѡ����� ��ѯͳ��ʧ�ܣ�\n");
		return;
	}
	}
}

/*
ɾ������
*/
void del()
{
	card = readCard();
	int t = 0;
	char name[50];
	printf("��������Ҫע���Ŀ���\n");
	scanf("%s", name);
	char pas[20];
	printf("��������Ҫ��Ҫ�Ŀ��ŵ�����\n");
	scanf("%s", pas);
	Card *card2;
	card2 = (Card*)malloc(sizeof(Card));
	while (card != NULL)
	{
		if (queryCard(card, name) != NULL)
		{
			card2 = queryCard(card, name);
			printf("�ҵ���Ӧ�������� ���ڲ�ѯ����Ϣ\n");
		}
		else
		{
			card = card->next;
			continue;
		}
		if (card->nStatus == 1)
		{
			printf("�ÿ�����ʹ�� ����ע��");
			break;
		}
		if (card->nStatus == 2)
		{
			printf("�ÿ���ע��");
			break;
		}
		t = 1;
		break;
	}
	if (t == 1)
	{   //����card_service.cppע����Ƭ��������
		int ye = delCard(card2);
		if (ye == 1)
		{
			printf("����\t�˻����\n");
			printf("%s\t%.1f", card2->aName, card2->fBalance);
			printf("\n");
			printf("ע���ɹ�\n");
		}
		else
		{
			printf("ע��ʧ��\n");
		}
	}
}

