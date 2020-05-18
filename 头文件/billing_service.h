#ifndef BILLING_SERVICE_H_INCLUDED
#define BILLING_SERVICE_H_INCLUDED
typedef struct Billing
{
	char aCardName[18];//����
	time_t tStart;//�ϻ�ʱ��
	time_t tEnd;//�»�ʱ��
	float Amount;//���ѽ��
	float fBalance;//���
	struct Billing *next;
}Billing;
void doSettle(Card *card);
Billing* initial2();
Billing* readInfo();
Billing* saveBilling(Card *card, time_t t, int hour, int min);
float inQuiryTo();
float inQuiryMon(Card *card);
#endif // BILLING_SERVICE_H_INCLUDED

