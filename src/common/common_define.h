#ifndef COMMON_DEFINE_H
#define COMMON_DEFINE_H

#include <iostream>
#include <pthread.h>
#include <string.h>
#include <vector>
#include <errno.h>
#include <sys/time.h>
#include <syslog.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_PATH_SIZE	255
#ifndef LINE_MAX
#define LINE_MAX		255
#endif

#define EPOLL_MAX_SIZE			1024
#define EPOLL_TIME_WAIT		10
#define THREAD_POOL_SIZE		4
#define THREAD_POOL_WAITTIME	-1
#define TASKQUEUE_MAX_SIZE	10240

#define STRERROR(no) (strerror(no) != NULL ? strerror(no) : "Unkown error")
//#define get_current_time() (g_schedule_flag ? g_current_time: time(NULL))
#define get_current_time() time(NULL)

//=========================LOGGER==============================//
#define ACCESS_LOG_PATH "/tmp/logs/sdfs/access"
#define ERROR_LOG_PATH "/tmp/logs/sdfs/error"
#define LOG_BUF_SIZE 1024
#define ROTATE_SIZE 1024*1024*10L
#define USE_CACHE	1
#define LOG_LEVEL LOG_DEBUG

//=========================CONFIG==============================//
#define CONF_PATH	"/usr/local/sdfs/conf"
#define TRACKER_CONF_NAME	"tracker.conf"
#define STORAGE_CONF_NAME	"storage.conf"

//=========================NETWORK==============================//
#define SOCK_STR_LEN 16
#define SRV_NAME_LEN 20

//==========================DEBUG===============================//
#ifndef DEBUG
#define DEBUG 1
#endif

//=========================THREAD==============================//
#define DEFAULT_STACK_SIZE 10*1024*1024
#define THREAD_COND_WAIT_SEC	10
#define THREAD_COND_WAIT_NSEC	0
typedef void *(*thread_func)(void *arg);

using namespace std;

#endif
