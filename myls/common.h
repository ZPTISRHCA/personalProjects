/***********************************************************
# File Name		: common.h
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月26日 星期一 17时29分19秒
# Descrition	: 定义了一些系统所学要的长量，错误处理函数 
***********************************************************/
#ifndef	MYLSCOMMON_H
#define	MYLSCOMMON_H

namespace myls {

#define MAXFILESIZE 100
#define MAXROWLEN	80 //一行显示的最多字符数
#define NAMEMAX 100 //文件名最大长度


//支持参数
enum PARAM {
	NONE=0x0000, A=0x0001, L=0x0002
};

void myErr(const char *errStr, int line); 

} //namespace

#endif
