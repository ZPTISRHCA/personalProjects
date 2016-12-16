/***********************************************************
# File Name		: logger.h
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月01日 星期四 04时46分48秒
# Descrition	: 系统的日志封装，真个系统日志分为四级，开发时默认为Debug，使用时默认Info
***********************************************************/
#ifndef HTTPFILEDOWNLOAD_LOGGER_H
#define HTTPFILEDOWNLOAD_LOGGER_H

namespace httpfiledownload {

class Logger {
public:
	enum Level { //日志分级
		Error = 0, //系统运行致命错误，必须停止运行
		Warn  = 1, //系统运行错误，但不致命
		Info  = 2, //主要用于系统运行中的显示和提示
		Debug = 3, //主要用于开发输出调试信息
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
