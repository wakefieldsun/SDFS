/*
 * Log.h
 *
 *  Created on: 2013-7-16
 *      Author: sun
 */

#ifndef LOG_H_
#define LOG_H_

namespace sdfs {

class Log {
private:
	Log();
public:
	static void Debug(char *format, ...);
	static void Info(char *format, ...);
	static void Notice(char *format, ...);
	static void Warning(char *format, ...);
	static void Error(char *format, ...);
	static void Crit(char *format, ...);
	static void Emerge(char *format, ...);
};

} /* namespace sdfs */
#endif /* LOG_H_ */
