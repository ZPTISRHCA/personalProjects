/***********************************************************
# File Name		: taskLog.h
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月10日 星期六 19时03分51秒
# Descrition	: 保存系统下载项目信息，包括：正在下载项目，已完成项目。
				  用于系统断点下载和下载记录查看
***********************************************************/

#ifndef HTTPFILEDOWNLOAD_TASKLOG_H
#define HTTPFILEDOWNLOAD_TASKLOG_H

#include <iostream>
#include "../include/downloadTool.h"
#include <string>
#include <list>

using std::string;
using std::list;
using std::endl;
using std::ostream;

namespace httpfiledownload {

/*
 * 正在下载项目
 */
class TaskNode {
public:
	int	   m_taskNo;	//项目编号
	string m_fileName;	//下载文件名称
	size_t m_totalSize; //要下载文件总大小
	//各线程下载进度（任务开始位置，结束位置，当前位置），没有用的到线程初始化为0
	size_t m_threadLog[THREADMAXNUMS][3]; 
	bool   m_stop;		//任务状态，false代表正在下载，true代表暂停任务

	TaskNode(int no, string filename, size_t totalSize, size_t thread[][2], bool stop);
	friend ostream& operator <<(ostream& out, const TaskNode& node); //显示信息
};

//已完成项目
class FinishTask {
public:
	string m_fileName; //文件名
	size_t m_totalSize;//文件大小

	FinishTask(string name, size_t size):m_fileName(name), m_totalSize(size) {}
	friend ostream& operator <<(ostream& out, const FinishTask& finsh); //显示信息
};

/*
 * 下载任务管理，系统下载过的和正在下载的信息全部都会被记录，并最终保存在文件中
 */
class TaskLog {
private:
	list<TaskNode*>   m_curTask;	//正在下载任务列表
	list<FinishTask*> m_finishTask; //已下载任务列表
	size_t m_taskNo;				//下一个下载任务编号（每次默认为1）

private:
	bool saveTask();	//保存正在下载任务列表
	bool saveFinish();	//保存已下载任务列表

public:
	TaskLog():m_taskNo(1){}
	bool newTask(TaskNode* add);	//新建下载任务
	bool newFinishTask(FinishTask* finish);	//新增完成任务
	void searchCurTask();			//查看正在下载任务列表
	void searchFinishTask();		//查看已完成任务列表
	bool stopTask(int no);			//暂停一个任务
	bool continueTask(int no);		//继续下载一个任务
	void stopAll();					//暂停所有任务
	void continueAll();				//继续下载所有任务
	bool deleteCurTask(int no);		//删除一个任务
};

} //namespace 

#endif
