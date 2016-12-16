/***********************************************************
# File Name		: analysisURL.h
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月01日 星期四 08时40分07秒
# Descrition	: 通过URL的解析 1.主机名  2. 端口号（如果有）  3.路径  4.文件名
				  1，3 用于构造HTTP请求包，1，2 用于连接服务器，4 用于下载数据保存
***********************************************************/

#ifndef HTTPFILEDOWNLOAD_ANALYSISURL_H
#define HTTPFILEDOWNLOAD_ANALYSISURL_H

#include <string>

using std::string;

namespace httpfiledownload {

class AnalysisURL {
private:
    string			m_hostName;
    unsigned long   m_IP; //通过主机名计算得到
    unsigned int    m_port;
    string			m_path;
	string			m_fileName;

public:
    AnalysisURL(char*); //初始化为用户输入url
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
	inline const string& getFileName() const {
		return m_fileName;
	}
};

} // namespace httpfiledownload

#endif

