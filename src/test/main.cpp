#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include "waiter.h"
#include "Customer.h"
#include "../common/Logger.h"


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
	waiter w;
	Customer c1("sun"), c2("liu");
	c1.addObserver((Observer *)&w);
	c2.addObserver((Observer *)&w);

	c1.notifyObserver();
	c2.notifyObserver();

	Logger *logger = Logger::getAccessLogger();
	logger->log(LOG_INFO, "hello!%s", "sdfs");

	return 0;
}
