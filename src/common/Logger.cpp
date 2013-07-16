/*
 * Logger.cpp
 *
 *  Created on: 2013-7-15
 *      Author: sun
 */


#include <limits.h>
//#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include "Logger.h"
#include "common_define.h"


namespace sdfs {


const Logger* Logger::m_access_logger = new Logger(LogContextBuilder::getAccessLogger());
const Logger* Logger::m_error_logger = new Logger(LogContextBuilder::getErrorLogger());

Logger::Logger(LogContext *context) {
	// TODO Auto-generated constructor stub
	this->pLogContext = context;
	Open();
}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
	Close();
}

Logger* Logger::getAccessLogger()
{
	return (Logger *)m_access_logger;
}
Logger* Logger::getErrorLogger()
{
	return (Logger *)m_error_logger;
}

int Logger::Open()
{
	if((pLogContext->m_fd = open(pLogContext->filename, O_WRONLY | \
				O_CREAT | O_APPEND, 0644))< 0)
	{
		fprintf(stderr, "create log: %s, errno:%d, info:%s\n", \
				pLogContext->filename,errno, STRERROR(errno));
		pLogContext->m_fd = STDERR_FILENO;
		return errno;
	}
	pLogContext->current_size = lseek(pLogContext->m_fd, 0, SEEK_END);
	if(pLogContext->current_size < 0)
	{
		fprintf(stderr, "lseek log: %s, errno: %s\n", \
				pLogContext->filename, STRERROR(errno));
		return errno;
	}
	return 0;
}

void Logger::log(const int priority, const char *text, const int len)
{
	bool bNeedSync;
	char *caption;


	switch(priority)
	{
	case LOG_DEBUG:
		bNeedSync = true;
		caption = "DEBUG";
		break;
	case LOG_INFO:
		bNeedSync = true;
		caption = "INFO";
		break;
	case LOG_NOTICE:
		bNeedSync = false;
		caption = "NOTICE";
		break;
	case LOG_WARNING:
		bNeedSync = false;
		caption = "WARNING";
		break;
	case LOG_ERR:
		bNeedSync = false;
		caption = "ERROR";
		break;
	case LOG_CRIT:
		bNeedSync = true;
		caption = "CRIT";
		break;
	case LOG_ALERT:
		bNeedSync = true;
		caption = "ALERT";
		break;
	case LOG_EMERG:
		bNeedSync = true;
		caption = "EMERG";
		break;
	default:
		bNeedSync = false;
		caption = "UNKOWN";
		break;
	}

	logEx(caption, text, len, bNeedSync);

}

void Logger::logEx(const char *caption, \
		const char *text, const int length, const bool bNeedSync)
{
	struct tm _tm;
	int time_fragement;
	int result;
	//time
	struct timeval tv;

	if (pLogContext->m_time_precision == LOG_TIME_PRECISION_SECOND)
	{
		tv.tv_sec = get_current_time();
		tv.tv_usec = 0;
	}
	else
	{
		gettimeofday(&tv, NULL);
	}
	if(pLogContext->m_time_precision == LOG_TIME_PRECISION_SECOND)
	{
		//second
		time_fragement = 0;
	}
	else if(pLogContext->m_time_precision == LOG_TIME_PRECISION_MSECOND)
	{
		time_fragement = tv.tv_usec / 1000;
	}
	else
		time_fragement = tv.tv_usec;

	localtime_r(&tv.tv_sec, &_tm);
	//lock
	if((result = pthread_mutex_lock(&pLogContext->lock)) != 0)
	{
		fprintf(stderr, "file: "__FILE__", line: %d, call: " \
				"pthread_mutex_lock error, info: %s\n", \
				__LINE__, STRERROR(result));
		return ;
	}
	//if buf enough to storage log text
	if(pLogContext->m_size < length+64)
	{
		fprintf(stderr, "file: "__FILE__", line: %d, call: " \
						"info: not enough buf to create log\n", \
						__LINE__);
		return;
	}
	//if rest buf was not enough to store, sync buf to disk
	if( pLogContext->m_buf + pLogContext->m_size - pLogContext->p_cur_buf < \
			length+64)
	{
		Sync(false);
	}
	//generate string
	//time
	if(pLogContext->m_time_precision == LOG_TIME_PRECISION_SECOND)
	{
		result = sprintf(pLogContext->p_cur_buf, "[%04d-%02d-%02d %02d:%02d:%02d] ", \
							_tm.tm_year+1900, _tm.tm_mon+1, _tm.tm_mday, \
							_tm.tm_hour, _tm.tm_min, _tm.tm_sec);
	}
	else
		result = sprintf(pLogContext->p_cur_buf, "[%04d-%02d-%02d %02d:%02d:%02d.%03d] ", \
							_tm.tm_year+1900, _tm.tm_mon+1, _tm.tm_mday, \
							_tm.tm_hour, _tm.tm_min, _tm.tm_sec, time_fragement);
	pLogContext->p_cur_buf += result;
	//caption
	if(caption != NULL)
	{
		result = sprintf(pLogContext->p_cur_buf, "%s -", caption);
		pLogContext->p_cur_buf += result;
	}
	//text
	memcpy(pLogContext->p_cur_buf, text, length);
	pLogContext->p_cur_buf += length;
	*(pLogContext->p_cur_buf) = '\n';
	pLogContext->p_cur_buf++;

	if(!pLogContext->bCache || bNeedSync)
		Sync(false);

	//unlock
	if((result = pthread_mutex_unlock(&pLogContext->lock)) != 0)
	{
		fprintf(stderr, "file: "__FILE__", line: %d, call " \
				"pthread_mutex_unlock error, info: %s\n", \
				__LINE__, STRERROR(result));
	}
}

int Logger::Sync(bool bNeedLock)
{
	int result;
	int write_size;

	write_size = pLogContext->p_cur_buf - pLogContext->m_buf;
	//check buf
	if(write_size == 0)
	{
		if(!pLogContext->bRotate)
			return 0;
		else
			return Check_rotate(bNeedLock);
	}
	if(bNeedLock && (result = pthread_mutex_lock(&pLogContext->lock)) != 0)
	{
		fprintf(stderr, "file: "__FILE__", line: %d, call" \
				"pthread_mutex_lock error, info: %s\n", \
				__LINE__, STRERROR(result));
		return result;
	}
	//write to buf
	//if filesize will extends threshold
	if(pLogContext->current_size + write_size > pLogContext->rotate_size){
		pLogContext->bRotate = true;
		Check_rotate(bNeedLock);
	}

	//write to log file
	do{
	result = write(pLogContext->m_fd, pLogContext->m_buf, write_size);
	if(result != write_size)
	{
		fprintf(stderr, "file: "__FILE__", line: %d, call" \
						"write error, info: %s\n", \
						__LINE__, STRERROR(result));
		break;
	}
	if(pLogContext->m_fd != STDERR_FILENO)
		if(fsync(pLogContext->m_fd) != 0)
		{
			result = errno != 0 ? errno :EIO;
			fprintf(stderr, "file: "__FILE__", line: %d, call fsync, " \
					"errno: %d, info: %s", __LINE__, errno, STRERROR(errno));
			break;
		}
	if(pLogContext->bRotate)
	{
		result = Check_rotate(bNeedLock);
		break;
	}
	}while(0);
	if(bNeedLock && (result = pthread_mutex_unlock(&pLogContext->lock)) != 0)
	{
		fprintf(stderr, "file: "__FILE__", line: %d, call" \
				"pthread_mutex_unlock error, info: %s\n", \
				__LINE__, STRERROR(result));
		return result;
	}
	return result;
}

int Logger::Check_rotate(bool bNeedLock)
{
	int result;
	//check fd
	if (pLogContext->m_fd == STDERR_FILENO)
	{
		if (pLogContext->current_size > 0)
		{
			pLogContext->current_size = 0;
		}
		return ENOENT;
	}
	//是否需要同步
	if (bNeedLock)
	{
		pthread_mutex_lock(&(pLogContext->lock));
	}
	//是否需要旋转
	if (pLogContext->bRotate)
	{
		result = Rotate();
		pLogContext->bRotate = false;
	}
	else
	{
		result = 0;
	}
	//unlock
	if (bNeedLock)
	{
		pthread_mutex_unlock(&(pLogContext->lock));
	}
	return result;
}

int Logger::Rotate()
{
	struct tm tm;
	time_t current_time;
	char new_filename[MAX_PATH_SIZE + 32];

	if (*(pLogContext->filename) == '\0')
	{
		return ENOENT;
	}

	close(pLogContext->m_fd);

	current_time = get_current_time();
	localtime_r(&current_time, &tm);
	sprintf(new_filename, "%s.%04d%02d%02d_%02d%02d%02d", \
			pLogContext->filename, \
			tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, \
			tm.tm_hour, tm.tm_min, tm.tm_sec);
	if (rename(pLogContext->filename, new_filename) != 0)
	{
		fprintf(stderr, "file: "__FILE__", line: %d, " \
			"rename %s to %s fail, errno: %d, error info: %s", \
			__LINE__, pLogContext->filename, new_filename, \
			errno, STRERROR(errno));
	}

	return Open();
}

void Logger::setRotate()
{
	pLogContext->bRotate = true;
}

void Logger::Close()
{
	if (pLogContext->m_fd >= 0 && pLogContext->m_fd != STDERR_FILENO)
	{
		Sync(true);

		close(pLogContext->m_fd);
		pLogContext->m_fd = STDERR_FILENO;

		pthread_mutex_destroy(&pLogContext->lock);
	}

	if (pLogContext->m_buf != NULL)
	{
		//free(pLogContext->m_buf);
		delete pLogContext->m_buf;
		pLogContext->m_buf = NULL;
		pLogContext->p_cur_buf = NULL;
	}
}

} /* namespace sdfs */
