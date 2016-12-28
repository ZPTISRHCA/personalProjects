/***********************************************************
# File Name		: analysisCommand.cpp
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月27日 星期二 20时26分46秒
# Descrition	: 
***********************************************************/

#include "analysisCommand.h"
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <iostream>
#include <dirent.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>

namespace myls {

AnalysisCmd::AnalysisCmd(int param, char *pathName[], bool result[], int count):m_flag(param)
{
	for (int i=0; i<count; ++i) {
		std::cout << pathName[i] << std::endl;
	}
	if (count == 0) { //命令没有携带文件，默认当ian路径
		strcpy(pathName[0], "./");
		pathName[0][2] = '\0';
		count++;
	}
	struct stat buf;
	for (int i=0; i<count; ++i) {
		if (stat(pathName[i], &buf) == -1) {
			result[i] = false;
			continue;
		}
		if (S_ISDIR(buf.st_mode)) { //如果是目录
			int len = strlen(pathName[i]);
			if (pathName[i][len-1] != '/') {
				char* tmp = new char[len+2];
				if (tmp == NULL) {
					myErr("memory allocate failure", __LINE__);
				}
				memcpy(tmp, pathName[i], len);
				tmp[len] = '/';
				tmp[len+1] = '\0';
				pathName[i] = tmp;
			}
			displayDir(pathName[i]);
		} 
		else {
			display(pathName[i]);
		}
	}
}

//根据文件类型显示(有l参数)
void AnalysisCmd::pFileType(mode_t mode)
{
	if (S_ISLNK(mode)) {//链接
		putchar('l');
	}
	else if (S_ISREG(mode)) {//一般
		putchar('-');
	}
	else if (S_ISDIR(mode)) {//目录
		putchar('d');
	}
	else if (S_ISCHR(mode)) {//字符设备
		putchar('c');
	}
	else if (S_ISBLK(mode)) { //块设备
		putchar('b');
	}
	else if (S_ISFIFO(mode)) {//pipe
		putchar('f');
	}
	else if (S_ISSOCK(mode)) { //socket
		putchar('s');
	}
}
//使用l参数时,显示文件用户权限
void AnalysisCmd::pJurisdiction(mode_t mode)
{
	//所有者
	if (mode & S_IRUSR) {
		putchar('r');
	}
	else
		putchar('-');
	if (mode & S_IWUSR) {
		putchar('w');
	}
	else
		putchar('-');
	if (mode & S_IXUSR) {
		putchar('x');
	}
	else
		putchar('-');
	//同一组
	if (mode & S_IRGRP) {
		putchar('r');
	}
	else
		putchar('-');
	if (mode & S_IWGRP) {
		putchar('w');
	}
	else
		putchar('-');
	if (mode & S_IXGRP) {
		putchar('x');
	} 
	else
		putchar('-');
	//其他人
	if (mode & S_IROTH) {
		putchar('r');
	}
	else
		putchar('-');
	if (mode & S_IWOTH) {
		putchar('w');
	}
	else
		putchar('-');
	if (mode & S_IXOTH) {
		putchar('x');
	}
	else
		putchar('-');
	putchar('.');
}
//使用l参数时,调用此函数显示buf中详细信息
void AnalysisCmd::pWithL(struct stat& buf) {
	char time[32];
	struct passwd *psd;
	struct group *grp;

	pFileType(buf.st_mode);
	pJurisdiction(buf.st_mode);

	psd = getpwuid(buf.st_uid); //获取用户名
	grp = getgrgid(buf.st_gid); //获取组名
	printf("%2d %-6s%-6s%4d", buf.st_nlink, psd->pw_name, grp->gr_name, buf.st_size);
	strcpy(time, ctime(&buf.st_mtime));
	time[strlen(time)-1] = '\0'; //去掉换行符
	printf(" %s", time);
}

//没有使用参数时,调用此函数打印文件名,打印时上下对齐
void AnalysisCmd::pWithN(char *fileName) 
{
	static int leaveLen = MAXROWLEN;//已行剩余长度,用于输出对齐

	if (leaveLen < m_maxLen) {
		printf("\n");
		leaveLen = MAXROWLEN;
	}
	int len = m_maxLen -strlen(fileName);
	printf("%-s", fileName);
	for (int i=0; i<len; ++i) {
		putchar(' ');
	}
	printf(" ");
	leaveLen -= (m_maxLen+1);
}

//根据参数显示一个文件
void AnalysisCmd::display(char *pathname) 
{
	char name[NAMEMAX+1];
	int j = 0;
	for (int i=0; i<strlen(pathname); i++)
	{
		//从路径中解析出文件名
		if (pathname[i] == '/') {
			j = 0;
			continue;
		}
		name[j] = pathname[i];
		j++;
	}
	name[j] = '\0';

	struct stat buf;
	if (lstat(pathname, &buf) == -1) {
		myErr("sta", __LINE__);
	}
	switch(m_flag) {
		case NONE:
			if (name[0] != '.') //隐藏文件不显示
				pWithN(name);
			break;
		case A:
			pWithN(name);
			break;
		case L:
			if (name[0] != '.') {
				pWithL(buf);
				printf(" %-s\n", name);
			}
			break;
		case A+L:
			pWithL(buf);
			printf(" %-s\n", name);
			break;
		default:
			break;
	}
}

//根据参数显示一个文件夹
void AnalysisCmd::displayDir(char *path) 
{
	DIR *dir;
	struct dirent *ptr;
	int count = 0; //记录当前文件夹下文件数目

	dir = opendir(path);
	if (dir == NULL) {
		myErr("opendir", __LINE__);
	}
	//计算当前文件夹下最长文件名长度
	while ((ptr = readdir(dir)) != NULL) {
		if (m_maxLen < strlen(ptr->d_name)) {
			m_maxLen = strlen(ptr->d_name);
		}
		count++;
	}
	closedir(dir);

	char** filenames = new char*[count];
	if (filenames == NULL) {
		myErr("memory allocate failure", __LINE__);
	}
	for (int i=0; i<count; ++i) {
		filenames[i] = new char[m_maxLen+1];
		if (filenames[i] == NULL) {
			myErr("memory allocate failure", __LINE__);
		}
	}
	int len = strlen(path);
	dir = opendir(path);
	for (int i=0; i<count; ++i) {
		ptr = readdir(dir);
		if (ptr == NULL) {
			myErr("readdir error", __LINE__);
		}
		strncpy(filenames[i], path, len); //拷贝路径
		filenames[i][len] = '\0'; //使其成为字符串
		strcat(filenames[i], ptr->d_name); //拷贝文件名
		filenames[i][len+strlen(ptr->d_name)] = '\0';
	}
	for (int i=0; i<count; ++i) {
		display(filenames[i]); //显示各个文件
	}
	closedir(dir);
	if ((m_flag & L) == 0) //不是l参数
		puts("");
}

} // namespace
