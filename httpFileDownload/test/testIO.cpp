/***********************************************************
# File Name		: testIO.cpp
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月07日 星期三 23时52分28秒
# Descrition	: 
***********************************************************/

#include "../include/httpPacket.h"
#include "../include/ioHandler.h"
#include "../include/conWebServer.h"
#include "../include/analysisURL.h"
#include <iostream>
#include <cstring>

using namespace std;
using namespace httpfiledownload;

int main() {
	//char b[] = "http://news.qq.com/a/20161208/033976.html";

	//char b[] = "http://117.34.50.31/dlied6.qq.com/invc/qqpack/Wechat_Setup.exe?mkey=5848e65d40d80680&f=3480&c=0&p=.exe";
	//char b[] = "http://wap.sogou.com/app/redir.jsp?appdown=1&u=0Gd8piB6093-W-QPHXpNcQJvLH6U5CYd7_gTMbkenBQlia2gpsaPJH1VSh0l2hghy6rGmVCuxuQ.&docid=-1612059341574501111&sourceid=1306443573476071096&w=1906&stamp=20161208";
	
	//char b[] = "http://117.34.50.14/dlied6.qq.com/invc/qqpack/QQ_Setup.exe?mkey=5849216e40d80680&f=9555&c=0&filename=QQ_Setup.exe&p=.exe"; 
	char b[] = "http://p2.image.hiapk.com/uploads/allimg/150828/928-150RQQ642-56.jpg";
	AnalysisURL an(b);
	ConWebServer con(an);

	int fd = con.getClientFd();
	HttpPacket packetTool;
	string packet = packetTool.constructHeadPacket(an);
	IOSocket io(fd);
	io.sendMsg(packet.c_str(), packet.length());
	char buffer[2048];
	size_t totalSize = 0, curSize = 0;
	memset(buffer, 0, 2048);
	size_t length = io.recvMsg(buffer, 2048);
	cout << "length: " << length << endl;
	PacketData pd;
	packetTool.analysisPacket(buffer, pd);
	totalSize = pd.m_contentLength;

	PacketConfig pg;
	pg.m_begin = 0;
	pg.m_end = pd.m_contentLength;

	packet = packetTool.constructGetPacket(an, pg);
	io.sendMsg(packet.c_str(), packet.length());
	memset(buffer, 0, 2048);
	length = io.recvMsg(buffer, 2048);

	length = length - packetTool.analysisPacket(buffer, pd);
	IOFile iof(an.getFileName());	
	pd.m_end = length;

	iof.write(&pd);
	
	curSize += pd.m_end;
	while (1) {
		if (totalSize <= curSize)
			break;
		memset(buffer, 0, 2048);
		length = io.recvMsg(buffer, 2048);

		pd.m_begin = curSize;
		pd.m_end = curSize+length;

		pd.m_data = buffer;
		iof.write(&pd);
		curSize += length;
	}
	if (totalSize != curSize)
		cout << "error" << endl;
	else 
		cout << "successful download" << endl;
    return 0;
}
