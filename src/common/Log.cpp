/*
 * Log.cpp
 *
 *  Created on: 2013-7-16
 *      Author: sun
 */


#include <stdarg.h>
#include "Log.h"
#include "common_define.h"
#include "Logger.h"

namespace sdfs {

Log::Log() {

}


void Log::Debug(char *format, ...)
{
#ifdef DEBUG
	char text[LINE_MAX];
	int len = 0;
	va_list ap;
	va_start(ap, format);
	len = vsnprintf(text, sizeof(text), format, ap);
	va_end(ap);
	Logger::getAccessLogger()->log(LOG_DEBUG, text, len);
#endif
	return;
}

void Log::Info(char *format, ...)
{
#if LOG_LEVEL >= LOG_INFO
	char text[LINE_MAX];
	int len = 0;
	va_list ap;
	va_start(ap, format);
	len = vsnprintf(text, sizeof(text), format, ap);
	va_end(ap);
	Logger::getAccessLogger()->log(LOG_INFO, text, len);
#endif
}

void Log::Notice(char *format, ...)
{
#if LOG_LEVEL >= LOG_NOTICE
	char text[LINE_MAX];
	int len = 0;
	va_list ap;
	va_start(ap, format);
	len = vsnprintf(text, sizeof(text), format, ap);
	va_end(ap);
	Logger::getAccessLogger()->log(LOG_NOTICE, text, len);
#endif
}

void Log::Warning(char *format, ...)
{
#if LOG_LEVEL >= LOG_WARNING
	char text[LINE_MAX];
	int len = 0;
	va_list ap;
	va_start(ap, format);
	len = vsnprintf(text, sizeof(text), format, ap);
	va_end(ap);
	Logger::getAccessLogger()->log(LOG_WARNING, text, len);
#endif
}

void Log::Error(char *format, ...)
{
#if LOG_LEVEL >= LOG_ERR
	char text[LINE_MAX];
	int len = 0;
	va_list ap;
	va_start(ap, format);
	len = vsnprintf(text, sizeof(text), format, ap);
	va_end(ap);
	Logger::getErrorLogger()->log(LOG_ERR, text, len);
#endif
}

void Log::Crit(char *format, ...)
{
#if LOG_LEVEL >= LOG_CRIT
	char text[LINE_MAX];
	int len = 0;
	va_list ap;
	va_start(ap, format);
	len = vsnprintf(text, sizeof(text), format, ap);
	va_end(ap);
	Logger::getErrorLogger()->log(LOG_CRIT, text, len);
#endif
}

void Log::Emerge(char *format, ...)
{
#if LOG_LEVEL >= LOG_EMERG
	char text[LINE_MAX];
	int len = 0;
	va_list ap;
	va_start(ap, format);
	len = vsnprintf(text, sizeof(text), format, ap);
	va_end(ap);
	Logger::getErrorLogger()->log(LOG_EMERG, text, len);
#endif
}

} /* namespace sdfs */
