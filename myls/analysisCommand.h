/***********************************************************
# File Name		: analysisCommand.h
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月26日 星期一 17时50分28秒
# Descrition	: 
***********************************************************/

#ifndef MYLSANALYSISCOMMAND_H
#define MYLSANALYSISCOMMAND_H

#include "common.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace myls {

class AnalysisCmd {
private:
	int m_flag;
	int m_maxLen;//存放某目录下最长文件名的长度

private:
	void pFileType(mode_t mode);
	void pJurisdiction(mode_t mode);
	void pWithL(struct stat& buf);
	void pWithN(char* name);
	void display(char* pathname);
	void displayDir(char* path);

public:
	//显示信息,并找出错误目录或文件
	////参数:参数集和,目录文件名,err set, 大小
	AnalysisCmd(int param, char **pathName, bool result[], int count);
};

} // namespace


#endif
