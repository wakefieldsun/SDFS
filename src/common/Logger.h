/*
 * Logger.h
 *
 *  Created on: 2013-7-15
 *      Author: sun
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "common_define.h"
#include "LogContextBuilder.h"
#include <syslog.h>

namespace sdfs {

class Logger {
public:
	static Logger* getAccessLogger();
	static Logger* getErrorLogger();
	virtual ~Logger();

	void setRotate();
	void log(int priority, const char *strfmt, ...);

private:
	Logger(LogContext* context);
	Logger(const Logger &logger);
	//Logger &operator=(const Logger &logger);
	void logEx(const char *caption, \
			const char *text, const int length, const bool bNeedSync);
	int Sync( bool bNeedLock);
	int Check_rotate(bool bNeedLock);
	int Open();
	void Close();
	int Rotate();
private:
	static const Logger* m_access_logger;
	static const Logger* m_error_logger;

	LogContext *pLogContext;
};


} /* namespace sdfs */
#endif /* LOGGER_H_ */
