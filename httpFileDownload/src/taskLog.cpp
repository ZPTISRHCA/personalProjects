/***********************************************************
# File Name		: taskLog.cpp
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月10日 星期六 23时58分12秒
# Descrition	: 
***********************************************************/

#include "../include/taskLog.h"
#include "../include/logger.h"

namespace httpfiledownload {

TaskNode::TaskNode(int no, string filename, size_t totalSize, size_t thread[][2], bool stop):m_taskNo(no), m_fileName(filename), m_totalSize(totalSize), m_stop(stop) {
		for (int i=0; i<THREADMAXNUMS; ++i) {
			for (int j=0; j<2; j++) {
				m_threadLog[i][j] = thread[i][j]; //初始化下载任务信息
			}
			m_threadLog[i][2] = m_threadLog[i][0]; //当前下载位置=任务开始位置
		}
}
	
std::ostream& operator <<(std::ostream& out, const TaskNode& node) { //显示信息
		size_t curSize = 0;
		for (int i=0; i<THREADMAXNUMS; ++i) {
			curSize += (node.m_threadLog[i][2]-node.m_threadLog[i][0]);
		}
		out << "NO: " << node.m_taskNo << "  fileName: " << node.m_fileName << "  stop: ";
		node.m_stop == false? out << "no" << endl : out << "yes" << endl;
		out << "  totalSize: " << node.m_totalSize << "  curSize: " << curSize << endl;
}


std::ostream& operator <<(std::ostream& out, const FinishTask& finish) { //显示信息
		out << "fileName: " << finish.m_fileName << " < size: " << finish.m_totalSize << " >" << endl;
		return out;
}


bool newTask(TaskNode* add);	//新建下载任务
bool newFinishTask(FinishTask* finish);	//新增完成任务
void searchCurTask();			//查看正在下载任务列表
void searchFinishTask();		//查看已完成任务列表
bool stopTask(int no);			//暂停一个任务
bool continueTask(int no);		//继续下载一个任务
void stopAll();					//暂停所有任务
void continueAll();				//继续下载所有任务
bool deleteCurTask(int no);		//删除一个任务

} //namespace
