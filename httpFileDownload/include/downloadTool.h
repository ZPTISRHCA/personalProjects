/***********************************************************
# File Name		: process.h
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月09日 星期五 17时20分35秒
# Descrition	: 1.实现线程互斥访问（信号量）  2.下载控制
***********************************************************/

#ifndef HTTPFILEDOWNLOAD_DOWNLOADTOOL_H
#define HTTPFILEDOWNLOAD_DOWNLOADTOOL_H

#include "showMsg.h"
#include "analysisURL.h"
#include "sem.h"
#include "ioHandler.h"
//#include "taskLog.h"
#include <pthread.h>

namespace httpfiledownload {

const size_t BUFFERSIZE = 2048; //缓冲区默认大小
const int THREADMAXNUMS = 2;    //多线程下载时，下载线程数

class DownloadTool;

//下载线程需要信息
struct ThreadDownloadMsg {
	DownloadTool&	m_downloadTool;	//下载类，主要用于获取，更新数据
	IOSocket*     	m_ioSocket;		//socket
	size_t			m_dataLength;   //探测包接收到的数据长度
	size_t			m_begin;        //线程任务开始地方
	size_t			m_end;          //线程人物结束地方
	char*			m_buffer;		//单线程下载的第一个数据包	
	bool			m_threads;		//多线程下载信息包/单线程下载信息包

	ThreadDownloadMsg(DownloadTool& tool, IOSocket* io, size_t length, size_t begin, size_t end, char* buf, bool threads):m_downloadTool(tool), m_ioSocket(io), m_dataLength(length), m_begin(begin), m_end(end), m_buffer(buf), m_threads(threads) {}
};

//下载类
class DownloadTool {
	size_t			m_bufSize;		//当前缓冲区大小
	int				m_threadNums;	//但前下载线程数量
	pthread_t*		m_pt;			//线程描述符
	DownloadMsg*	m_downloadMsg;	//系统下载信息保存
	AnalysisURL&	m_analysisURL;	//各线程共享，主要用于子线程连接服务器
	ShowMsg*		m_showMsg;		//显示下载信息
	IOFile*			m_ioFile;       //文件操作
	Sem				m_sem;          //系统信息锁，更新数据时互斥访问
	bool			m_stop;         //是否完成下载，完成后，主要用于
//	TaskLog&		m_taskLog;      //下载任务管理器

private:
	size_t dealFirstPacket(IOSocket*& io, AnalysisURL& an, char* buffer, size_t bufferSize, size_t begin, size_t end, PacketData& pd);
	//创建下载线程
	void createDownloadThread();
	/*
	 * 创建下载线程，参数用于指定线程任务
	 */
	void createDownloadThread(size_t begin, size_t end); 
	/*
	 * 线程执行函数，返回值暂时无用
	 * 参数实际上是:ThreadDownloadRange*
	 * 功能：
	 */
	static void* downloadThread(void*); 
	/*
	 * 更新系统下载数据，显示数据
	 * 参数为，完成下载的范围
	 */
	void update(size_t begin, size_t end);

public:
	Sem				m_fileSem;      //
	/*
	 * 
	 */
	DownloadTool(AnalysisURL& an, size_t bufSize=BUFFERSIZE, int tNums=THREADMAXNUMS);
	~DownloadTool();
	inline DownloadMsg* getDownloadMsg() {
		return m_downloadMsg;
	}
	inline ShowMsg* getShowMsg() {
		return m_showMsg;
	}
	inline size_t getBufferSize() {
		return m_bufSize;
	}
	inline int getThreadNums() {
		return m_threadNums;
	}
	//获取文件IO
	inline IOFile* getFile() {
		return m_ioFile;
	}
	/*inline TaskLog& getTaskLog() {
		return m_taskLog;
	}*/
	//获取URL解析结果
	inline AnalysisURL& getAnalysisURL() {
		return m_analysisURL;
	}
	//创建显示线程
	void createShowMsgThread() {
		ShowMsg::showMsg((void*)m_showMsg); //显示下载进度信息
	}
};

}// namespace


#endif
