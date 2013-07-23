#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include "../common/common_define.h"
#include "waiter.h"
#include "Customer.h"
#include "../common/Log.h"
#include "../thread/Thread.h"
#include "../thread/Mutex.h"
#include "../thread/IRunnable.h"
#include "../thread/ThreadPool.h"

const int PORT = 1221;
const int LISTENQ = 1024;

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
	TestRunner(struct Share_Data *data)
	{
		m_data = data;
		Log::Debug("init TestRunner");
	};
	~TestRunner()
	{
	};

	void *Run(void *arg)
	{
		//struct Share_Data *pData = (struct Share_Data *)arg;
		int fd = (int)arg;
		char buf[LINE_MAX];
		memset(buf, 0, LINE_MAX);
		Log::Debug("start reading...");
		int nCount = read(fd, buf, LINE_MAX);
		Log::Debug("end reading...");
		if(nCount > 0)
			printf("recv: %s\n", buf);
		return NULL;
	};

	void Stop()
	{
		Log::Debug("TestRunner Stop");
	}

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

int setNoblock(int sockfd)
{
	if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1)
	{
		return -1;
	}
	return 0;
}

int startSocket()
{
	struct sockaddr_in addr;
	int result;
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	result = setNoblock(fd);
	if(result < 0)
	{
		Log::Error("error:%s, setNoblock", STRERROR(result));
		return result;
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	int len = sizeof(struct sockaddr);
	result = bind(fd, (struct sockaddr *)&addr, len);
	if(result != 0)
	{
		Log::Error("error:%s, bind", STRERROR(result));
		return result;
	}
	listen(fd, LISTENQ);
	Log::Info("start listening on Port: %d", PORT);
	return fd;
}

int main()
{
	int result;
	void *ptret;
	bool iscontinue = true;
	epoll_event event, keyboradevent;
	epoll_event events[EPOLL_MAX_SIZE];
	struct sockaddr_in their_addr;
	unsigned int len;
	int plfd;
	int listenfd = startSocket();

	if((plfd = epoll_create(EPOLL_MAX_SIZE))<0)
	{
		Log::Error("error:%s, epoll_create", STRERROR(plfd));
		return 1;
	}
	event.events = EPOLLIN|EPOLLET;
	event.data.fd = listenfd;

	keyboradevent.events =  EPOLLIN|EPOLLET;
	keyboradevent.data.fd = 0;

	result = epoll_ctl(plfd, EPOLL_CTL_ADD, listenfd, &event);
	if(result != 0)
	{
		Log::Error("error:%s, epoll_ctl", STRERROR(result));
		return 1;
	}

	result = epoll_ctl(plfd, EPOLL_CTL_ADD, 0, &keyboradevent);
	if(result != 0)
	{
		Log::Error("error:%s, epoll_ctl", STRERROR(result));
		return 1;
	}
	TestRunner runner(NULL);
	ThreadPool *threadpool = new ThreadPool(runner);
	char buf[LINE_MAX];
	Thread poolthread(threadpool);
	poolthread.setIsWait(false);
	poolthread.Start();
	while(iscontinue)
	{
		int nfds = epoll_wait(plfd, events, EPOLL_MAX_SIZE, -1);
		Log::Debug("main: epoll_wait returned: %d", nfds);
		if(nfds < 0)
		{
			Log::Error("error, epoll_wait");
			exit(1);
		}
		for (int n = 0; n < nfds; ++n) {
			Log::Debug("main: events[%d].data.fd = %d", n, events[n].data.fd);
		     if (events[n].data.fd == listenfd) {
		    	 int new_fd = accept(listenfd, (struct sockaddr *) &their_addr,
		    	                                 &len);

		    	 setNoblock(new_fd);
		    	 Log::Debug("got a connect: %d!", new_fd);
		    	 threadpool->addTask(new_fd);
//		    	 sleep(2);
		    	 //thread2.Notify(new_fd);
		     }
		     else if(events[n].data.fd == 0)
		     {
		    	 int count = read(events[n].data.fd, buf, LINE_MAX);
		    	 if(count > 0 && buf[0] == 'q'){
		    		 iscontinue = false;
					 epoll_ctl(plfd, EPOLL_CTL_DEL, events[n].data.fd, &events[n]);
					 epoll_ctl(plfd, EPOLL_CTL_DEL, listenfd, &event);
		    		 continue;
		    	 }
		     }
		}
	}
	poolthread.Stop();
	shutdown(listenfd, SHUT_RDWR);
	close(listenfd);
	delete threadpool;
	return 0;
}
