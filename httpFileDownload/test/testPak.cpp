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

	char b[] = "http://117.34.50.31/dlied6.qq.com/invc/qqpack/Wechat_Setup.exe?mkey=5848e65d40d80680&f=3480&c=0&p=.exe";

	AnalysisURL an(b);
	/*
	cout << "hostName: " << an.getHostName() << endl;
	cout << "path: " << an.getPath() << endl;
	cout << "port: " << an.getPort() << endl << endl;
	*/
	ConWebServer con(an);

	int fd = con.getClientFd();
	HttpPacket packetTool;
	string packet = packetTool.constructHeadPacket(an);
	IOSocket io(fd);
	io.sendMsg(packet.c_str(), packet.length());
	char buffer[1024];
	memset(buffer, 0, 1024);
	io.recvMsg(buffer, 1024);

	PacketData pd;
	packetTool.analysisPacket(buffer, pd);

    return 0;
}
