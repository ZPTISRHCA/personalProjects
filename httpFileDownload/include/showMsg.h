/***********************************************************
# File Name		: showMsg.h
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月08日 星期四 08时03分43秒
# Descrition	: 
***********************************************************/

#ifndef HTTPFILEDOWN_SHOWMSG_H
#define HTTPFILEDOWN_SHOWMSG_H

#include <time.h>

namespace httpfiledownload {

	/*
class DownloadMsg {
    clock_t m_beginTime, m_lastTime;
    size_t  m_totalSize, m_curSize, m_lastSize; 

public:
    DownloadMsg(size_t total):m_totalSize(total), m_lastSize(0), \
			      m_curSize(0),m_beginTime(0), m_lastTime(0) {}
    inline void startDownload() { m_beginTime = clock(); }
    inline double getSpeed() { 
	/*
	clock_t differenceTime = (clock()-m_lastTime)/CLOCKS_PER_SEC; 
	size_t differenceSize = m_curSize-m_lastSize;
	cout << differenceTime << endl;
	cout << differenceSize << endl;
	cout << differenceSize/differenceTime << endl;
	m_lastTime = clock();
	m_lastSize = m_curSize;
	return (differenceSize/differenceTime); 
	return 0.0;
    }
    inline size_t getTotalSize() { return m_totalSize; }
    inline size_t getCurSize() { return m_curSize; }
    inline size_t getRemainingSize() { return m_totalSize-m_curSize; }
    inline size_t getAlreadTime() { clock_t now = clock(); return (now-m_beginTime)/CLOCKS_PER_SEC; }
    inline size_t getRemainingTime() { return getRemainingSize()/getSpeed(); } 

public: 
    inline void setCurSize(size_t size) { m_curSize += size; }
};
*/

class ShowMsg {
public:
	static const int BARLENGTH = 20;

	double m_speed;
	clock_t m_startTime;
	size_t m_curSize;
	size_t m_totalSize;
	bool m_run;

public:
	ShowMsg(size_t totalSize):m_curSize(0), m_totalSize(totalSize), m_run(true) {
		m_startTime = clock();
	}
	static void* showMsg(void*);
	inline void setSpeed(double speed) {
		m_speed = speed;
	}
	inline void setCurSize(size_t curSize) {
		m_curSize = curSize;
	}
	inline void stop() {
		m_run = false;
	}
};

}// namespace httpfiledownload
#endif

