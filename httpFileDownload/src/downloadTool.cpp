/***********************************************************
# File Name		: downloadTool.cpp
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月09日 星期五 19时20分54秒
# Descrition	: 
***********************************************************/

#include "../include/downloadTool.h"
#include "../include/httpPacket.h"
#include "../include/ioHandler.h"
#include "../include/conWebServer.h"
#include "../include/analysisURL.h"
#include "../include/logger.h"

#include <string.h>

#include <stdio.h>
#include <iostream>

using namespace std;

namespace httpfiledownload {

DownloadTool::DownloadTool(AnalysisURL& an, size_t bufSize, int tNums):m_bufSize(bufSize), m_analysisURL(an), m_stop(false) {
	bool threads = true; //默认多线程下载
	if (tNums == 1)
		threads = false; //用户指定单线程下载

	if (tNums > THREADMAXNUMS) //用户指定线程数超过系统指定最大值
		tNums = THREADMAXNUMS;
	m_threadNums = tNums;

	char *buffer = new char[m_bufSize];
	if (buffer == NULL) {
		LogError("buffer allocate failure\n");
		exit(0);
	}

	PacketData pd;
	size_t totalSize = 0, dataLength = 0;
	IOSocket* io;
	if (threads == false) { //单线程下载
		dataLength = dealFirstPacket(io, an, buffer, m_bufSize, 0, 0, pd); //获取连接fd
		totalSize = pd.m_contentLength;
	} else { //用户希望多线程下载
		//发送探测包，获取连接fd
		dataLength = dealFirstPacket(io, an, buffer, m_bufSize, 0, 1, pd); 
		if (pd.m_end == 1 && pd.m_begin == 0) { //支持多线程下载
			totalSize = pd.m_contentLength;
		} else if (pd.m_begin == 0 && pd.m_end == 0) { //服务器不支持多线程,单线程下载
			threads = false;
			if (pd.m_contentLength != 0) { //找到文件长度
				totalSize = pd.m_contentLength;
			}
			//重新请求数据,如果totalSize为0，请求总文件，否则请求总长度
			//dataLength = dealFirstPacket(fd, an, buffer, m_bufSize, 0, totalSize, pd); 
		} else {
			LogError("system error\n");
			exit(0);
		}
	}

	if (threads == true) { //准备多线程下载
		LogDebug("total %d threads to download\n", m_threadNums);
		m_pt = new pthread_t[m_threadNums]; //初始化线程id
		if (m_pt == NULL) {
			LogError("pthred_t allocate failure\n");
			exit(1);
		}
	} else { //准备单线程下载
		LogDebug("total 1 threads to download\n");
	}
	

	m_downloadMsg = new DownloadMsg(totalSize); //初始化下载信息
	if (m_downloadMsg == NULL) {
		LogError("downlaodMsg allocate failure\n");
		exit(0);
	}
	m_showMsg = new ShowMsg(totalSize); //初始化显示信息
	if (m_showMsg == NULL) {
		LogError("show download message init failure\n");
		exit(0);
	}
	
	m_ioFile = new IOFile(m_analysisURL.getFileName()); //初始化下载文件
	if (m_ioFile == NULL) {
		LogError("ioFile allocate failure\n");
		exit(0);
	}
	//初始化完毕
	m_downloadMsg->startDownload();
	pthread_t show;
	//pthread_create(&show, NULL, ShowMsg::showMsg, (void*)m_showMsg);
	if (threads == true) {//多线程下载
		createDownloadThread(); //创建多线程下载,参数无用
	} else { //单线程下载
		if (totalSize > 0) {
			ThreadDownloadMsg td(*this, io, dataLength, 0, totalSize-1, pd.m_data, false); //读文件
			downloadThread((void*)&td);//读取数据
		} else {
			ThreadDownloadMsg td(*this, io, dataLength, 0, totalSize, pd.m_data, false); //读文件
			downloadThread((void*)&td);//读取数据
		}
	}
	m_showMsg->over();
	//释放资源
	delete io;
	io = NULL;
	delete []buffer;
	buffer = NULL;
	if (threads == true) {
		delete []m_pt;
		m_pt = NULL;
	}
}

DownloadTool::~DownloadTool() {
//释放资源
	if (m_ioFile != NULL)
		delete m_ioFile;
	if (m_downloadMsg != NULL)
		delete m_downloadMsg;
	if (m_showMsg != NULL)
		delete m_showMsg;
}


size_t DownloadTool::dealFirstPacket(IOSocket*& io, AnalysisURL& an, char* buffer, size_t bufferSize, size_t begin, size_t end, PacketData& pd) {
	LogDebug("thread %d is going to send first Packet\n", pthread_self());
	memset(buffer, 0, bufferSize);
	ConWebServer con(an);     //连接web服务器
	int fd = con.getClientFd();
	LogDebug("thread %d connect server ok\n", pthread_self());
	io = new IOSocket(fd); //socket读写器
	if (io == NULL) {
		LogError("iosocket allocat failure\n");
		exit(EXIT_FAILURE);
	}
	HttpPacket packetTool;          //http包操作器
	PacketConfig pg(begin, end); //http请求包配置信息包
	string packet;
	packet = packetTool.constructGetPacket(an, pg);//构造http请求包
	io->sendMsg(packet.c_str(), packet.length()); //发送http请求包
	LogDebug("thread %d send request packet success\n", pthread_self());
	size_t length = io->recvMsg(buffer, m_bufSize); //接收响应，返回为响应包长度
	length -= packetTool.analysisPacket(buffer, pd); //解析响应包，获取响应包中数据长度
	return length;
}

/*
 * 主要功能：（复用）
 * 1.发送头包探测系统
 * 2. 多线程下载的线程处理函数
 */
void* DownloadTool::downloadThread(void* arg) {
	ThreadDownloadMsg* msg= (ThreadDownloadMsg*)arg;

	//初始化下载操作器和资源
	DownloadTool& tool = msg->m_downloadTool; //下载管理器
	size_t curSize = msg->m_begin; //下载开始位置
	size_t totalSize = msg->m_end; //结束位置
	size_t dataLength = msg->m_dataLength; //探测包接收到的数据长度
	IOSocket* io = msg->m_ioSocket;   //socket
	bool threads = msg->m_threads;
	AnalysisURL& an = tool.getAnalysisURL();  //url分析器
	size_t bufferSize = tool.getBufferSize(); //获取缓冲区长度
	char *buffer = new char[bufferSize]; //初始化缓冲区
	if (buffer == NULL) {
		LogError("buffer allocate failure\n");
		exit(0);
	}
	memset(buffer, 0, bufferSize);

	//TaskLog& taskLog = msg->getTaskLog(); //任务管理其
	IOFile* iof = tool.getFile();
	PacketData pd;      //http包解析结果存放，写入文件数据包（共用）
	//初始化完成
	
	//如果是多线程下载，改变：fd, dataLength
	if (threads == true) { //多线程下载,发送请求包,获取第一个包数据长度
		dataLength = tool.dealFirstPacket(io, an, buffer, bufferSize, curSize, totalSize, pd);
		LogDebug("thread %d request success <%d-%d>\n", pthread_self(), pd.m_begin, pd.m_end);
	} else { //单线程下载
		if (curSize == 0 && totalSize == 0) {
			LogDebug("thread %d request success.\n");
		}else {
			LogDebug("thread %d request success <%d-%d>\n", pthread_self(), curSize, totalSize);
		}
		pd.m_data = msg->m_buffer;
		pd.m_end = dataLength;
	}

	memcpy(buffer, pd.m_data, dataLength); //将探测包接到的数据拷贝到buffer，为了统一接口

	while (1) {
		pd.m_begin = curSize; //指定写入数据范围
		pd.m_end = curSize+dataLength;
		pd.m_data = buffer; //配置写入包
		iof->write(&pd); //写入文件
		curSize += dataLength; //更改当前下载位置
		tool.update(pd.m_begin, pd.m_end);  //更新数据
	//	LogDebug("thread %d update <%d-%d>\n", pthread_self(), pd.m_begin, pd.m_end);
		if (curSize >= totalSize && totalSize != 0) //任务没有执行完
			break;
		memset(buffer, 0, bufferSize); //清空缓冲区
		dataLength = io->recvMsg(buffer, bufferSize);//接收数据（该数据可直接写入文件），获取数据长度
		if (dataLength == 0 && totalSize == 0) //TCP缓冲区中已经没有数据
			break;
	}//下载完成
	if (threads == true) {
		if (io != NULL)
			delete io;
	}

	if (totalSize != curSize-1 && totalSize != 0) { //任务执行出错，下载长度和应该下载长度不相等
		LogError("thread %d download <%d-%d> failure\n", pthread_self(), msg->m_begin, msg->m_end);
		exit(1);
	} else { //任务执行完成
		if (totalSize == 0)
			LogDebug("download <%d-%d> successful\n", msg->m_begin, curSize);
		else 
			LogDebug("thread %d download <%d-%d> successful\n", pthread_self(), msg->m_begin, totalSize);
	}
    return NULL;
}

void DownloadTool::update(size_t begin, size_t end) {
	m_sem.semV(); //获得锁
//更新数据DownloadMs
	m_downloadMsg->setCurSize(end-begin);
	
//更新数据ShowMsg
	m_showMsg->setCurSize(m_downloadMsg->getCurSize());
	m_showMsg->setSpeed(m_downloadMsg->getSpeed());
	
//更新数据TaskLog
	//m_taskLog.

	m_sem.semP(); //释放锁
}

/*
 * 创建多线程下载
 */
void DownloadTool::createDownloadThread() {
	size_t perThreadDownload = m_downloadMsg->getTotalSize()/m_threadNums;//计算每个线程下载长度
	size_t begin = 0, end = begin+perThreadDownload; //每个线程下载范围

	ThreadDownloadMsg** td = new ThreadDownloadMsg*[m_threadNums];
	if (*td == NULL) {
		LogError("ThreadDownloadMsg* allocate fialure");
		exit(EXIT_FAILURE);
	}
	cout << this->m_bufSize << endl;
	for (int i=0; i<m_threadNums; i++) {
		if (i == m_threadNums-1) {//如果是最后一个线程，下载剩余所有数据
			end = m_downloadMsg->getTotalSize();
		//两个参数为0,NULL，只是由于单线程下载复用线程函数，为了统一接口
		}
		td[i] = new ThreadDownloadMsg(*this, NULL, 0, begin, end-1, NULL, true); //初始化线程任务
		if (td[i] == NULL) {
			LogError("ThreadDownloadmsg allocate failure\n");
			exit(EXIT_FAILURE);
		}
		pthread_create(m_pt+i, NULL, downloadThread, (void*)(td[i]));//创建线程
		LogDebug("create thread %d ID: %d  :<%d-%d>\n", i+1, m_pt[i], begin, end);
		begin = end; //计算下一线程下载任务范围
		end += perThreadDownload;
	}
	for (int i=0; i<m_threadNums; ++i) { //等待所有下载线程完成
		if (0 != pthread_join(m_pt[i], NULL)) {
			LogWarn("wait thread %d error\n", m_pt[i]);
		}
	}
	//free memory
	for (int i=0; i<m_threadNums; ++i) {
		delete td[i];
	}
	delete []td;
	td = NULL;
}

} //namespace


