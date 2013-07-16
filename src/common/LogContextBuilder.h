/*
 * LogContext.h
 *
 *  Created on: 2013-7-15
 *      Author: sun
 */

#ifndef LOGCONTEXT_H_
#define LOGCONTEXT_H_

#include "common_define.h"
#include <syslog.h>

namespace sdfs {

#define LOG_TIME_PRECISION_SECOND 's' //second
#define LOG_TIME_PRECISION_MSECOND 'm' //millisecond
#define LOG_TIME_PRECISION_USSECOND 'u' //microsecond

typedef struct LogContext{
	int					m_level;
	int					m_fd;
	char				*m_buf;
	char				*p_cur_buf;
	int					m_size;
	pthread_mutex_t		lock;
	int64_t				rotate_size;
	int64_t				current_size;
	bool				bCache;
	bool				bRotate;
	char				m_time_precision;
	char				filename[MAX_PATH_SIZE];
}LogContext;

class LogContextBuilder
{
public:
	static LogContext* getAccessLogger(const char *filename=ACCESS_LOG_PATH, \
			int bufsize=LOG_BUF_SIZE, int64_t rotatesize=ROTATE_SIZE, bool bCache=USE_CACHE, int level=LOG_LEVEL);
	static LogContext* getErrorLogger(const char *filename=ERROR_LOG_PATH, \
			int bufsize=LOG_BUF_SIZE, int64_t rotatesize=ROTATE_SIZE, bool bCache=USE_CACHE, int level=LOG_LEVEL);
};

} /* namespace sdfs */
#endif /* LOGCONTEXT_H_ */
