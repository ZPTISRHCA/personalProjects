#include "../include/httpPacket.h"
#include "../include/ioHandler.h"
#include "../include/conWebServer.h"
#include "../include/analysisURL.h"
#include <iostream>
#include <cstring>

using namespace std;
using namespace httpfiledownload;

int main() {
    //char b[] = {"http://www.weixinxiazai.cn/ruanjian/4310782021.html"};

	//char b[] = "http://117.34.50.31/dlied6.qq.com/invc/qqpack/Wechat_Setup.exe?mkey=5848e65d40d80680&f=3480&c=0&p=.exe";
	//char b[] = "http://wap.sogou.com/app/redir.jsp?appdown=1&u=0Gd8piB6093-W-QPHXpNcQJvLH6U5CYd7_gTMbkenBQlia2gpsaPJH1VSh0l2hghy6rGmVCuxuQ.&docid=-1612059341574501111&sourceid=1306443573476071096&w=1906&stamp=20161208";
	
	char b[] = "http://117.34.50.14/dlied6.qq.com/invc/qqpack/QQ_Setup.exe?mkey=5849216e40d80680&f=9555&c=0&filename=QQ_Setup.exe&p=.exe"; 

	AnalysisURL an(b);
	ConWebServer con(an);

	int fd = con.getClientFd();
	HttpPacket packetTool;
	string packet = packetTool.constructHeadPacket(an);
	IOSocket io(fd);
	io.sendMsg(packet.c_str(), packet.length());
	char buffer[1024];
	memset(buffer, 0, 1024);
	io.recvMsg(buffer, 1024);
	size_t len = strstr(buffer, "\r\n\r\n")-buffer;
	if (len > strlen(buffer))
		len = strlen(buffer);

	PacketData pd;
	packetTool.analysisPacket(buffer, pd, len);

    return 0;
}
