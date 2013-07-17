#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include "../common/common_define.h"
#include "waiter.h"
#include "Customer.h"
#include "../common/Log.h"
#include "../thread/Thread.h"
#include "../thread/Mutex.h"

using namespace std;
using namespace sdfs;
/*
struct serverCommandPacketHead{
	unsigned int uMsgId;
	unsigned int uSrcClientIp;
	unsigned short usSrcClientPort;
	unsigned int uSrcEntityType;
	unsigned int uSrcProcessId;
	unsigned long long ullSrcTaskId;
	unsigned int uDestEntityType;
	unsigned long long ullDestTaskId;
	unsigned int uDestProcessId;
	string sTransId;
	unsigned int uMsgLen;
};

typedef struct tagSERVERCOMMANDPACKET {

	serverCommandPacketHead scph;
	char serverCommandPacketContent[0];

} SERVERCOMMANDPACKET, * PSERVERCOMMANDPACKET;


typedef struct tagSERVERCOMMANDPACKET {
struct {
unsigned int uMsgId;
unsigned int uSrcClientIp;
unsigned short usSrcClientPort;
unsigned int uSrcEntityType;
unsigned int uSrcProcessId;
unsigned long long ullSrcTaskId;
unsigned int uDestEntityType;
unsigned long long ullDestTaskId;
unsigned int uDestProcessId;
std::string sTransId;
unsigned int uMsgLen;


}serverCommandPacketHead;

#define uMsgLen serverCommandPacketHead.uMsgLen

#define sTransId serverCommandPacketHead.sTransId

char serverCommandPacketContent[0];

} SERVERCOMMANDPACKET, * PSERVERCOMMANDPACKET;
*/

/*
struct data
{
	char key[10];
	char value[10];
};

string getSid()
{
	return string("123");
}
*/

struct Share_Data
{
	char buf[LINE_MAX];
	Mutex mutex;
	int value;
};

void *test_func(void *arg)
{
	struct Share_Data *pData = (struct Share_Data *)arg;
	while(1)
	{
		//pData->mutex.Lock();
		//Log::Debug("Thread working...\n");
		Log::Debug("I'm %x", pthread_self());
		sprintf(pData->buf, "value: %d", pData->value++);
		Log::Debug("value: %d", pData->value);
		sleep(1);
		//pData->mutex.Unlock();
	}
	return NULL;
}

int main()
{
//	data d;
//	strcpy(d.key,"key");
//	strcpy(d.value,"value");
//	SERVERCOMMANDPACKET *pScp = (SERVERCOMMANDPACKET *)malloc(sizeof(SERVERCOMMANDPACKET)+sizeof(data));
//	char *pStr = (char *)((char *)pScp+sizeof(SERVERCOMMANDPACKET));
//	memcpy(pStr, &d, sizeof(data));
//	//A.B.a= 1;
//	//cout<<A.B.a<<endl;
//	SERVERCOMMANDPACKET *pRecv = pScp;
//	data *pData = (data *)((char *)pRecv+sizeof(SERVERCOMMANDPACKET));
//	cout<<pData->key<<endl;
//	cout<<pData->value<<endl;
//	SERVERCOMMANDPACKET scp;
//	serverCommandPacketHead *p_cph = (serverCommandPacketHead *)(&scp);
//	string str("123");
//	p_cph->sTransId=getSid();
//	cout<<p_cph->sTransId<<endl;
//	vector<data> vTest;
//	vTest.push_back(d);
//	data d2 = vTest.back();
//	printf("pData: 0x%8X, d: 0x%8X\n", (unsigned int)&d2, (unsigned int)&d);
	//string str("123");
	//cout<<str<<endl;
//	free(pScp);
	int result;
//	waiter w;
//	Customer c1("sun"), c2("liu");
//	c1.addObserver((Observer *)&w);
//	c2.addObserver((Observer *)&w);
//
//	c1.notifyObserver();
//	c2.notifyObserver();
//
//	printf("LOG_DEBUG:%d LOG_INFO:%d\n", LOG_DEBUG, LOG_INFO);
//	Log::Debug("hello world!%s", "sdfs");
	struct Share_Data data;
	data.value=0;
	//memset(&data, 0, sizeof(struct Share_Data));
	Thread thread1(test_func, &data);
	Thread thread2(test_func, &data);
////	Log::Debug("Start...");
////	Log::Debug("LOG_DEBUG:%d LOG_INFO:%d LOG_NOTICE:%d LOG_WARNING:%d "\
////			"LOG_ERR:%d LOG_CRIT:%d LOG_ALERT:%d LOG_EMERG:%d", LOG_DEBUG, \
////			LOG_INFO, LOG_NOTICE, LOG_WARNING, LOG_ERR, LOG_CRIT, \
////			LOG_ALERT, LOG_EMERG);
	result = thread1.Start();
	if(result != 0)
		Log::Error("error:%s, create thread1", STRERROR(result));
	result = thread2.Start();
	if(result != 0)
		Log::Error("error:%s, create thread2", STRERROR(result));
	sleep(10);
	thread1.Killself();
	thread2.Killself();
	//Log::Error("test");
	sleep(10);
	return 0;
}
