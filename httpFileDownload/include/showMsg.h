/***********************************************************
# File Name		: showMsg.h
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月08日 星期四 08时03分43秒
# Descrition	: 系统的下载信息保存，信息显示
***********************************************************/

#ifndef HTTPFILEDOWN_SHOWMSG_H
#define HTTPFILEDOWN_SHOWMSG_H

#include <time.h>

namespace httpfiledownload {

class DownloadMsg {
	//开始下载时间
    clock_t m_beginTime;
	//文件总大小，已下载大小，上一次计算速度时的大小
    size_t  m_totalSize, m_curSize; 

public:
	/*
	 * 初始化文件总大小
	 */
    DownloadMsg(size_t total):m_totalSize(total), m_curSize(0), m_beginTime(0) {}
	//开始下载（系统开始计数）
    inline void startDownload() { 
		m_beginTime = clock(); 
	}
	//返回总大下
    inline size_t getTotalSize() { 
		return m_totalSize; 
	}
	//返回当前下载数据大小
    inline size_t getCurSize() { 
		return m_curSize; 
	}
	//返回已下载时间
    inline size_t getAlreadTime() { 
		clock_t now = clock(); 
		return (now-m_beginTime)/CLOCKS_PER_SEC;
	}

public: 
	//增加下载大小。成功读写文件就更新一下
    inline void setCurSize(size_t size) { m_curSize += size; }
};

/*
 * 显示下载信息
 */
class ShowMsg {
public:
	static const int BARLENGTH = 20; //进度条长度

private:
	DownloadMsg* m_info;
	bool    m_run;       //是否运行
	bool    m_stop;      //暂停显示

public:
	//初始化：总大小
	ShowMsg(DownloadMsg* info):m_info(info), m_run(true),m_stop(false) {}
	bool isRun() {
		return m_run;
	}
	bool isStop() {
		return m_stop;
	}
	//显示信息，为了便于封装成线程，所以设计成这个样子
	static void* showMsg(void*);
	//终止下载（会结束显示（不能继续））
	inline void over() {
		m_run = false;
	}
	//暂停下载，可以继续
	inline void stop() {
		m_stop = true;
	}
};

}// namespace httpfiledownload
#endif

