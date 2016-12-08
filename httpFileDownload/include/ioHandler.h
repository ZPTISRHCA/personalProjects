/***********************************************************
# File Name		: ioHandler.h
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月07日 星期三 08时44分36秒
# Descrition	: 
***********************************************************/

#ifndef HTTPFILEDOWN_IOHANDLER_H
#define HTTPFILEDOWN_IOHANDLER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace httpfiledownload {

class IOSocket {
    int m_fd;

public:
    IOSocket(int fd):m_fd(fd) {}
    ~IOSocket() {
		close(m_fd);
	}
    inline int sendMsg(const char* msg, size_t size) {
		if (send(m_fd, msg, size, 0) == -1) 
			return 0;
		return 1;
}
    inline int recvMsg(char* msg, size_t size) {
		if (recv(m_fd, msg, size, 0) == -1)
			return 0;
		return 1;
}

};



} // namespace httpfiledown

#endif

