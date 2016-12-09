/***********************************************************
# File Name		: testShowMsg.cpp
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月08日 星期四 18时59分47秒
# Descrition	: 
***********************************************************/

#include <iostream>
#include <unistd.h>
#include "../include/showMsg.h"

using namespace httpfiledownload;

int main() {
	ShowMsg a(500);
	int speed = 20;
	size_t curSize = 0;
	pthread_t t;
	void* (*p)(void*);
	p = ShowMsg::showMsg;
	pthread_create(&t, NULL, p, (void*)&a);
	while (1) {
		a.setSpeed(++speed);
		sleep(1);
		curSize += speed;
		if (curSize >= 500) {
			a.setCurSize(500);
			break;
		}
		a.setCurSize(curSize);
	}
	sleep(1);
	a.stop();
	return 0;
}
