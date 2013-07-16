/*
 * LogContext.cpp
 *
 *  Created on: 2013-7-15
 *      Author: sun
 */

#include "LogContextBuilder.h"
#include <stdio.h>
#include <string.h>

namespace sdfs {

//LogContext::~LogContext() {
//	// TODO Auto-generated destructor stub
//	delete m_buf;
//}

LogContext* LogContextBuilder::getAccessLogger(const char *filename, \
		int bufsize, int64_t rotatesize, bool bCache, int level)
{
	LogContext *lc = new LogContext;
	if(bufsize < 1024)
			bufsize = 1024;
	lc->m_buf = new char[bufsize];
	lc->p_cur_buf = lc->m_buf;
	lc->m_size = bufsize;
	lc->rotate_size = rotatesize;
	lc->bCache = bCache;
	lc->m_level = level;
	lc->m_time_precision = LOG_TIME_PRECISION_MSECOND;
	strcpy(lc->filename, filename);
	return lc;
}

LogContext* LogContextBuilder::getErrorLogger(const char *filename, \
		int bufsize, int64_t rotatesize, bool bCache, int level)
{
	LogContext *lc = new LogContext;
	if(bufsize < 1024)
			bufsize = 1024;
	lc->m_buf = new char[bufsize];
	lc->p_cur_buf = lc->m_buf;
	lc->m_size = bufsize;
	lc->rotate_size = rotatesize;
	lc->bCache = bCache;
	lc->m_level = level;
	lc->m_time_precision = LOG_TIME_PRECISION_MSECOND;
	strcpy(lc->filename, filename);
	return lc;
}

} /* namespace sdfs */
