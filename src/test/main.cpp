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
#include "../thread/IRunnable.h"

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
	int value;
};

class TestRunner: public IRunnable
{
public:
	TestRunner(struct Share_Data &data)
	{
		m_data = &data;
		Log::Debug("init TestRunner");
	};
	~TestRunner()
	{
	};

	void *Run(void *arg)
	{
		//struct Share_Data *pData = (struct Share_Data *)arg;
		int *pInt = (int *)arg;
		while(1)
		{
			m_mutex.Lock();
			Log::Debug("count: %d", *pInt);
			if(*pInt < m_data->value){
				Log::Debug("exit thread: %d", m_data->value);
				m_mutex.Unlock();
				pthread_exit((void *)0);
			}
			int err = sprintf(m_data->buf, "value: %d", m_data->value++);
			if(err < 1)
			{
				Log::Error("error: %s", STRERROR(err));
			}
			Log::Debug("value: %d", m_data->value);
			m_mutex.Unlock();
			sleep(1);
		}
		pthread_exit((void *)0);
	};

	int getValue()
	{
		return m_data->value;
	}
private:
	struct Share_Data *m_data;
	Mutex m_mutex;
};


char* boolstr(int b)
{
	if(b)
		return "true";
	return "false";
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
	void *ptret;
//	waiter w;
	int count1 = 10;
	int count2 = 5;
	struct Share_Data sd;
	memset(&sd, 0, sizeof(sd));
	TestRunner runner(sd);
	Thread thread1(runner, &count1, true);
	Thread thread2(runner, &count2, true);
//	Log::Debug("LOG_DEBUG:%d LOG_INFO:%d LOG_NOTICE:%d LOG_WARNING:%d "\
//			"LOG_ERR:%d LOG_CRIT:%d LOG_ALERT:%d LOG_EMERG:%d", LOG_DEBUG, \
//			LOG_INFO, LOG_NOTICE, LOG_WARNING, LOG_ERR, LOG_CRIT, \
//			LOG_ALERT, LOG_EMERG);
	result = thread1.Start();
	if(result != 0)
		Log::Error("error:%s, create thread1", STRERROR(result));
	result = thread2.Start();
	if(result != 0)
		Log::Error("error:%s, create thread2", STRERROR(result));
	sleep(4);

	result = thread1.isAlive();
	Log::Debug("thread1 Alive: %s", boolstr(result));
	result = thread2.isAlive();
	Log::Debug("thread2 Alive: %s", boolstr(result));

	//thread1.Killself();
	//thread2.Killself();
	result = thread1.Join(&ptret);
	if(result != 0)
		Log::Error("error:%s, thread1.Join", STRERROR(result));
	result = thread2.Join(&ptret);
	if(result != 0)
		Log::Error("error:%s, thread2.Join", STRERROR(result));
	Log::Debug("core.value: %d", sd.value);
	Log::Debug("sd.buf: %s", sd.buf);

	//Log::Error("test");
	sleep(4);
	return 0;
}
