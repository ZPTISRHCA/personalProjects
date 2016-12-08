/***********************************************************
# File Name		: conWebServer.cpp
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月07日 星期三 08时16分06秒
# Descrition	: 
***********************************************************/

#include "../include/conWebServer.h"
#include "../include/logger.h"

#include <sys/types.h>
#include <cstring>
#include <stdlib.h>
#include <sys/socket.h>

#include <sys/ioctl.h>

namespace httpfiledownload {


ConWebServer::ConWebServer(const AnalysisURL& an) {
    //memset(&m_conAddr, 0, sizeof(m_conAddr);
    m_clientFd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_clientFd == -1) {
		LogError("create socket of client failure\n");
		exit(0);
    }
    m_conAddr.sin_family = AF_INET;
    m_conAddr.sin_addr.s_addr = an.getIP();
    m_conAddr.sin_port = htons(an.getPort());
    if (connect(m_clientFd, (struct sockaddr*)&m_conAddr, \
		(socklen_t)sizeof(m_conAddr)) == -1) {
		LogError("connect failed");
		exit(0);
    }
	LogDebug("connect success ...\n");
}


}//namespace
