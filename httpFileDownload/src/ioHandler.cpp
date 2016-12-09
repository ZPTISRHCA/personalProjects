/***********************************************************
# File Name		: ioHandler.cpp
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月07日 星期三 21时20分41秒
# Descrition	: 
***********************************************************/

#include "../include/ioHandler.h"
#include <stdlib.h>
#include <cstring>


namespace httpfiledownload {


IOFile::IOFile(string fileName) {
	m_fp = (FILE*)fopen(fileName.c_str(), "wb");
	if (m_fp == NULL) {
		LogError("open %s error\n", fileName.c_str());
		exit(0);
	}
	LogDebug("open %s success\n", fileName.c_str());
}

void IOFile::write(PacketData* data) {
	if (data == NULL) {
		LogDebug("buffer is NULL\n");
		exit(0);
	}
	size_t size = data->m_end - data->m_begin;
	fseek(m_fp, data->m_begin, SEEK_SET);
	LogDebug("write file < %-5d-%5d >  total: %d\n", data->m_begin, data->m_end, size);
	size_t len = fwrite(data->m_data, size, 1, m_fp);
	if (len == -1) {
		LogDebug("file write error\n");
		LogError("error\n");
		exit(0);
	}
}

}
