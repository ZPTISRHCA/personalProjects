/***********************************************************
# File Name		: logger.h
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月01日 星期四 04时46分48秒
# Descrition	: 这个文件主要完成系统的日					 志
***********************************************************/
#ifndef HTTPFILEDOWNLOAD_LOGGER_H
#define HTTPFILEDOWNLOAD_LOGGER_H

namespace httpfiledownload {

class Logger {
public:
	enum Level {
		Error = 0,
		Warn = 1,
		Info = 2,
		Debug = 3,
	};

public:
    // 日志文件名，如果不设置，默认输出到标准输出
	static bool setLogFileName(const char* fn);
    // 设置日志级别
	static void setFilter(Level filter);
    // 写日志
	static void log(Level level, const char* fmt, ...);

private:
	static int s_fd;
	static Level s_filter;
};

#define LogError(fmt, args...)   Logger::log(Logger::Error  , fmt, ##args)
#define LogWarn(fmt, args...)    Logger::log(Logger::Warn   , fmt, ##args)
#define LogInfo(fmt, args...)    Logger::log(Logger::Info   , fmt, ##args)
#define LogDebug(fmt, args...)   Logger::log(Logger::Debug  , fmt, ##args)

} // namespace 

#endif
