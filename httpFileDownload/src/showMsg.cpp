/***********************************************************
# File Name		: showMsg.cpp
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月08日 星期四 08时10分22秒
# Descrition	: 
***********************************************************/

#include "../include/showMsg.h"
#include "../include/logger.h"
#include <unistd.h>

#include <iostream>
using namespace std;

namespace httpfiledownload {

void* ShowMsg::showMsg(void* arg) {
	//添加暂停显示
	ShowMsg* mg = (ShowMsg*)arg;
	DownloadMsg* info = mg->m_info;

	size_t curSize = 0, lastSize = 0, totalSize = 0;
	double speed = 0.0, fBar = 0.0;
	int curNums = 0;

	totalSize = info->getTotalSize();
	int waitTime = 1*1000;
	while (mg->isRun()) {
		curSize = info->getCurSize();
		speed =	(double (curSize-lastSize)*1000000)/(waitTime*1024);
		lastSize = curSize;

		fBar = (double)curSize/totalSize; 
		curNums = BARLENGTH*fBar;
		for (int i = 0; i<BARLENGTH + 60; i++)
			LogInfo("\b");
		LogInfo("[");
		for (int i = 0; i<curNums; i++)
			LogInfo("=");
		for (int i=0; i<BARLENGTH-curNums; i++)
			LogInfo(" ");
		if (totalSize == 0) {
			LogInfo("]%3d%    [%6d/  ~   ] : %.0fKB/S", (int)(fBar*100), curSize, speed);
		} else {
			LogInfo("]%3d%    [%6d/%-6d] : %.0fKB/S", (int)(fBar*100), curSize, totalSize, speed);
		}
		if (curSize >= totalSize)
			break;
		while (mg->isStop()) {
			;
		}
		usleep(waitTime); 
	}
	LogInfo("\n");
	return (void*)0;
}

} // namespace

