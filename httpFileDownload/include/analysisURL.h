/***********************************************************
# File Name		: analysisURL.h
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月01日 星期四 08时40分07秒
# Descrition	: 该文件主要是完成URL的解析					 然后获得需要的参数，用于                   构造HTTP请求包 
***********************************************************/

#ifndef HTTPFILEDOWNLOAD_ANALYSISURL_H
#define HTTPFILEDOWNLOAD_ANALYSISURL_H

#include <string>

using std::string;

namespace httpfiledownload {

class AnalysisURL {
private:
    string			m_hostName;
    unsigned long   m_IP;
    unsigned int    m_port;
    string			m_path;

public:
    AnalysisURL(char*);
    inline const string& getHostName()const  {
		return m_hostName;
	}
    inline unsigned long getIP() const {
		return m_IP;
	}
    inline unsigned int getPort() const {
		return m_port;
	}
    inline const string& getPath() const {
		return m_path;
	}
};

} // namespace httpfiledownload

#endif

