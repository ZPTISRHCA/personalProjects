/***********************************************************
# File Name		: ioHandler.h
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月07日 星期三 08时44分36秒
# Descrition	: 1.从socket中读取数据,往socket中写入数据  2.往文件中写入数据 
***********************************************************/

#ifndef HTTPFILEDOWN_IOHANDLER_H
#define HTTPFILEDOWN_IOHANDLER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "httpPacket.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "sem.h"

namespace httpfiledownload {

//对于socket的IO操作
class IOSocket {
    int m_fd;	//socket描述副

public:
    IOSocket(int fd):m_fd(fd) {}
    ~IOSocket() {
		close(m_fd); //关闭套接字
	}
	/*
	 * 发送size大小的数据
	 * 成功返回1，失败返回0
	 */
    inline int sendMsg(const char* msg, size_t size) { 
		if (send(m_fd, msg, size, 0) == -1) 
			return 0;
		return 1;
	}
	/*
	 * 从套接字里面接受数据，接受缓冲区大小为size，如果一次接收不完，需要多次接收
	 * 成果返回1，失败返回0
	 */
    inline int recvMsg(char* msg, size_t size) {
		int len = recv(m_fd, msg, size, 0);
		if (len < 0) {
			LogError("recv failure, ");
			if (errno == EAGAIN) {
				LogError("over time\n");
			} else if (errno == EBADF) {
				LogError("error socket\n");
			} else if (errno == EFAULT) {
				LogError("memory error\n");
			} else if (errno == EINTR) {
				LogError("an interruption\n");
			} else if (errno == ENOMEM) {
				LogError("memory less\n");
			} else if (errno == ENOTCONN) {
				LogError("not connect\n");
			} else if (errno == ENOTSOCK) {
				LogError("not socket\n");
			}
			exit(EXIT_FAILURE);
		} else if (len == 0) {
			LogError("connect has broken\n");
		} 
		return len;
}

};

//对文件的IO 操作
class IOFile {
private:
    FILE* m_fp; //文件描述符
	Sem	  m_sem;

public:
    IOFile(string fileName); //初始化为：文件名,如果文件存在就清空，否则创建
    ~IOFile() { fclose(m_fp); }
	/*
	 * 向文件中写入数据（封装在data中，包含：数据，开始位置，结束位置）
	 */
    void write(PacketData* data);
};

} // namespace httpfiledown

#endif

