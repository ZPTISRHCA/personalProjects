/***********************************************************
# File Name		: testThreadsDownload.cpp
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月11日 星期日 20时01分38秒
# Descrition	: 
***********************************************************/

#include "../include/downloadTool.h"
#include "../include/analysisURL.h"
#include <iostream>
#include <cstring>

using namespace std;
using namespace httpfiledownload;

int main(int argc, char* argv[]) {
//	char b[] = "http://news.qq.com/a/20161208/033976.html";

	//char b[] = "http://117.34.50.31/dlied6.qq.com/invc/qqpack/Wechat_Setup.exe?mkey=5848e65d40d80680&f=3480&c=0&p=.exe";
	//char b[] = "http://wap.sogou.com/app/redir.jsp?appdown=1&u=0Gd8piB6093-W-QPHXpNcQJvLH6U5CYd7_gTMbkenBQlia2gpsaPJH1VSh0l2hghy6rGmVCuxuQ.&docid=-1612059341574501111&sourceid=1306443573476071096&w=1906&stamp=20161208";
	
	//char b[] = "http://117.34.50.14/dlied6.qq.com/invc/qqpack/QQ_Setup.exe?mkey=5849216e40d80680&f=9555&c=0&filename=QQ_Setup.exe&p=.exe"; 
	char b[] = "http://p2.image.hiapk.com/uploads/allimg/150828/928-150RQQ642-56.jpg";
	//char b[] = "http://down8416.tkg5.com/?/61594/pconline/Ubuntu16.04%B9%D9%B7%BD%D5%FD%CA%BD%B0%E6.exe";
	AnalysisURL an(b);
	DownloadTool down(an);	
    return 0;
}
