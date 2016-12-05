/***********************************************************
# File Name		: logger.cpp
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月01日 星期四 05时08分48秒
# Descrition	: 日志函数实现 
***********************************************************/

#include "../include/logger.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>

namespace httpfiledownload {

// 默认输出到标准输出
int Logger::s_fd = STDIN_FILENO;
// 默认日志级别为Debug
Logger::Level Logger::s_filter = Logger::Debug;

void Logger::setFilter(Level filter)
{
    s_filter = filter;
}

// 设置输出日志到文件
// 文件描述符等待进程结束自动关闭
bool Logger::setLogFileName(const char * fn)
{
    s_fd = open(fn, O_CREAT | O_APPEND | O_WRONLY | O_NOFOLLOW | O_NOCTTY,
               S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (s_fd < 0) {
        return false;
    }
    return true;
}

void Logger::log(Level level, const char * fmt, ...)
{
    if (level > s_filter)
        return;

    // 单日志允许的最大输出
    const int buf_size = 2048;
    char buf[buf_size];

    va_list ap;
    va_start(ap, fmt);
    int len = vsnprintf(buf, buf_size, fmt, ap);
    va_end(ap);

    // 如果输出日志过长，截断则自动追加换行，避免两条日志连在一块
    if (len > buf_size) len = buf_size;
    buf[buf_size-1] = '\n';
    write(s_fd, buf, len);
    // 注意：如果用printf等来输出，需要
    //      buf[buf_size-2] = '\n';
    //      buf[buf_size-1] = '\0'
}

} // namespace
