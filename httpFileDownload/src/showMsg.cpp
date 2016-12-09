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

namespace httpfiledownload {

void* ShowMsg::showMsg(void* arg) {
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
		LogInfo("]%3d%    [%6d/%-6d] : %.0f", (int)(fBar*100), mg->m_curSize, mg->m_totalSize, mg->m_speed);
		usleep(20);
	}
	LogInfo("\n");
	return (void*)0;
}

} // namespace

