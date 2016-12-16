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


double DownloadMsg::getSpeed() {
	double differenceTime = (clock()-m_lastTime)/CLOCKS_PER_SEC; 
	size_t differenceSize = m_curSize-m_lastSize;
//	cout << clock() << "   " << m_lastTime << endl;
//	cout << differenceTime << "  " << differenceSize << endl;
	m_lastTime = clock();
	m_lastSize = m_curSize;
	return (differenceSize/differenceTime); 
}

void* ShowMsg::showMsg(void* arg) {
	//添加暂停显示
	ShowMsg* mg = (ShowMsg*)arg;

	double fBar = 0;
	int curNums = 0;
	while (mg->m_run) {
		fBar = (double)mg->m_curSize/mg->m_totalSize; 
		curNums = BARLENGTH*fBar;
		for (int i = 0; i<BARLENGTH + 60; i++)
			LogInfo("\b");
		LogInfo("[");
		for (int i = 0; i<curNums; i++)
			LogInfo("=");
		for (int i=0; i<BARLENGTH-curNums; i++)
			LogInfo(" ");
		if (mg->m_totalSize == 0) {
			LogInfo("]%3d%    [%6d/  ~   ] : %.0f", (int)(fBar*100), mg->m_curSize, mg->m_speed);
		} else {
			LogInfo("]%3d%    [%6d/%-6d] : %.0f", (int)(fBar*100), mg->m_curSize, mg->m_totalSize, mg->m_speed);
		}
		usleep(20);
	}
	LogInfo("\n");
	return (void*)0;
}

} // namespace

