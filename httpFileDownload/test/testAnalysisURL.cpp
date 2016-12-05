/***********************************************************
# File Name		: testAnalysisURL.cpp
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月04日 星期日 23时15分57秒
# Descrition	: 
***********************************************************/

#include<iostream>
#include <stdlib.h>
#include "../include/analysisURL.h"

using namespace std;
using namespace httpfiledownload;

int main(int argc, char* argv[]) {
	if (argc < 2)
		exit(1);
	AnalysisURL an(argv[1]);
	return 0;
}
