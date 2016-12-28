/***********************************************************
# File Name		: main.cpp
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月27日 星期二 13时35分05秒
# Descrition	: 
***********************************************************/

#include "analysisCommand.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace myls;

int main(int argc, char* argv[]) {
	int param = 0x0000;
	char* pathname[MAXFILESIZE]; //最多一次显示MAXROWLEN个目录或文件
	bool result[MAXFILESIZE];
	memset(&result, 0x01, MAXFILESIZE);
	int count = 0;

	for (int i=1; i<argc; ++i) {
		int j = 0;
		if (argv[i][j] == '-') {
			++j;
			while (argv[i][j] != '\0') {
				switch(argv[i][j]) {
					case 'a':
						param |= A;
						break;
					case 'l':
						param |= L;
						break;
					default:
						std::cerr << "无效选项 -" << argv[i][j] << std::endl;
						exit(EXIT_FAILURE);
				}
				++j;
			}
		} else {
			pathname[count++] = argv[i];
		}

	}

	if (count == 0) {
		pathname[0] = new char[3];
		if (pathname[0] == NULL) {
			std::cerr << "memory allocate failure" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	int errNums = 0;
	AnalysisCmd an(param, pathname, result, count);

	for (int i=0; i<count; ++i) {
		if (result[i] == true)
			continue;
		std::cerr << argv[0] << ": 无法访问"	<< pathname[i] << " 没有那个文件或目录" << std::endl;
	}
	return 0;
}
