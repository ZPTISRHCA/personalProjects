/***********************************************************
# File Name		: ../src/httpPacket.cpp
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月07日 星期三 04时02分56秒
# Descrition	: 
***********************************************************/

#include "../include/logger.h"
#include "../include/httpPacket.h"
#include <cstring>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
using namespace std;

namespace httpfiledownload {
	
string HttpPacket::constructGetPacket(const AnalysisURL& an, const PacketConfig& conf) {
	string req;
	if (conf.m_begin == 0 && conf.m_end == 0) { //用户指定单线程下载，包中不含有Range字段
		req = "GET " + an.getPath() + " HTTP/1.1\r\nHost: " + an.getHostName() + "\r\nUser-Agent: my client\r\nAccept: */*\r\n\r\n";
	} else {
		char cBg[20], cEd[20];
		sprintf(cBg, "%zu", conf.m_begin);
		sprintf(cEd, "%zu", conf.m_end);
		string bg = string(cBg), ed = string(cEd); 
		req = "GET " + an.getPath() + " HTTP/1.1\r\nHost: " + an.getHostName() + "\r\nUser-Agent: my client\r\nAccept: */*\r\nRange: bytes=" + bg + "-" + ed + "\r\n\r\n";
	}
    return req;
}
/*
 * 如果支持多线程下载，对于首包，pd.begin=0, pd.end = 1， pd.contentLength=constentLength
 * 其他不支持多线程,pd.begin = pd.end = 0。如果找到文件总大小会pd.contentLength=contentLength,否则pd.contentLength =0
 */
size_t HttpPacket::analysisPacket(char* packet, PacketData& pd) { 
	size_t headEnd = strstr(packet, "\r\n\r\n")-packet; //find packet head end
	if (headEnd > strlen(packet))
		headEnd = strlen(packet);

	char end = packet[headEnd];
	packet[headEnd] = '\0';
	string pt(packet);

	size_t pos = pt.find("Content-Range: bytes");

	if (pos != string::npos ) {
		LogDebug("find content-range   ");
		pt = pt.substr(pos+21);
		pos = pt.find_first_of("\r\n");
		if (pos == string::npos) {
			LogError("can't find the conetent-range end\n");
			exit(1);
		} 
		pt = pt.substr(0, pos);
		LogDebug("pt: %s  ", pt.c_str());
		pos = pt.find('/');
		pd.m_contentLength = atoi(pt.substr(pos+1).c_str());
		size_t bg = pt.find('-');
		pd.m_begin = atoi(pt.substr(0, bg).c_str());
		pd.m_end = atoi(pt.substr(bg+1, pos-bg-1).c_str());

		LogDebug("begin: %d   end: %d    contentLenght: %d\n",\
				pd.m_begin, pd.m_end, pd.m_contentLength);
	} else {
		LogDebug("can't find the content-range   ");
		pos = pt.find("Content-Length:");
		if (pos == string::npos) {
			LogError("can't find the content-length   ");
			pos = pt.find("Location:"); //是否已经重定向
			if (pos != string::npos) { //重定向
				size_t newURLEnd = pt.find("\r\n", pos);
				pos += 10;
				string newURL = pt.substr(pos, newURLEnd-pos);
				LogError("\nThe url has changed to <%s> please try again\n", newURL.c_str());
				exit(1);
			}
		} else {			
			LogDebug("find content-length  ");
			pd.m_contentLength = atoi(pt.substr(pos+16).c_str());
			LogDebug("content-length: %d\n", pd.m_contentLength);
		}
	}
	packet[headEnd] = end;
	pd.m_data = packet+headEnd+4;
	LogDebug("find data\n");
	return headEnd+4;
}

} //namespace
