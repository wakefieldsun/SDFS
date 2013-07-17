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

#define STRERROR(no) (strerror(no) != NULL ? strerror(no) : "Unkown error")
//#define get_current_time() (g_schedule_flag ? g_current_time: time(NULL))
#define get_current_time() time(NULL)

#define ACCESS_LOG_PATH "/tmp/logs/sdfs/access"
#define ERROR_LOG_PATH "/tmp/logs/sdfs/error"
#define LOG_BUF_SIZE 1024
#define ROTATE_SIZE 1024*1024*10L
#define USE_CACHE	1
#define LOG_LEVEL LOG_DEBUG

#ifndef DEBUG
#define DEBUG 1
#endif

#define DEFAULT_STACK_SIZE 20*1024

typedef void *(*thread_func)(void *arg);

//extern bool g_schedule_flag;
//extern time_t g_current_time;

using namespace std;

#endif
