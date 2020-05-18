#ifndef BILLING_SERVICE_H_INCLUDED
#define BILLING_SERVICE_H_INCLUDED
typedef struct Billing
{
	char aCardName[18];//卡号
	time_t tStart;//上机时间
	time_t tEnd;//下机时间
	float Amount;//消费金额
	float fBalance;//余额
	struct Billing *next;
}Billing;
void doSettle(Card *card);
Billing* initial2();
Billing* readInfo();
Billing* saveBilling(Card *card, time_t t, int hour, int min);
float inQuiryTo();
float inQuiryMon(Card *card);
#endif // BILLING_SERVICE_H_INCLUDED

