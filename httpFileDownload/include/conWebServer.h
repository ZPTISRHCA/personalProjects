/***********************************************************
# File Name		: conWebServer.h
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月07日 星期三 08时01分23秒
# Descrition	: 连接服务器 
***********************************************************/
#ifndef HTTPFILEDOWNLOAD_CONWEBSERVER_H
#define HTTPFILEDOWNLOAD_CONWEBSERVER_H

#include "analysisURL.h"
#include <arpa/inet.h>

namespace httpfiledownload {


class ConWebServer {
	struct sockaddr_in  m_conAddr;
    int					m_clientFd;

public:
    ConWebServer(const AnalysisURL&);
    inline int getClientFd() const { 
		return m_clientFd; 
	}
};

}

#endif
