/***********************************************************
# File Name		: common.cpp
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月28日 星期三 20时29分04秒
# Descrition	: 
***********************************************************/

#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


namespace myls {

void myErr(const char *errStr, int line) {
	fprintf(stderr, "line: %d ", line);
	perror(errStr);
	exit(1);
}

}


