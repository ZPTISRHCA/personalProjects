/***********************************************************
# File Name		: httpPacket.h
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月05日 星期一 23时28分07秒
# Descrition	: 构造http包:构造头包（主要用于获取要下载内容大小，是否支持多线程）
				  解析http包:对于多线程下载的包，解析出下载的范围和数据；对于但线程下载的包                   只解析出数据
***********************************************************/

#ifndef HTTPFILEDOWNLOAD_HTTPPACKET_H
#define HTTPFILEDOWNLOAD_HTTPPACKET_H

#include "../include/logger.h"
#include "../include/analysisURL.h"

namespace httpfiledownload {

/*
 * 解析包数据存储类，保存数据、下载数据的开始位置、下载数据的结束位置、总大小 
 */
struct PacketData {
    char*  m_data;			//download data
	size_t m_begin;			//download range begin
	size_t m_end;			//download range end
    size_t m_contentLength; //download file total size

    PacketData():m_data(NULL), m_begin(0), m_end(0), m_contentLength(0) {}
};

/*
 * 构造数据包的配置信息，包含：请求文件开始位置，请求结束位置
 */
class PacketConfig {
public:
	size_t m_begin;
	size_t m_end;

public:
	PacketConfig(size_t begin, size_t end):m_begin(begin), m_end(end) {}
};

/*
 *构造与解析http包
 */
class HttpPacket {
public:
	/*
	 *构造请求包，不要求请求数据，但是要求必须获得所有回应头信息
	 *an: 用于获取path、host
	 *conf: 用于指定请求包请求数据的范围（对于单线程必须使范围等于总大小）
	 */ 
    string constructGetPacket(const AnalysisURL& an, const PacketConfig& conf);
	/*
	 * 解析服务器响应数据包，将解析到的数据存放于pd中
	 * 数据有：总大小，下载数据在整个文件中的开始位置与结束位置（多线程数据包）
	 * 对于非多线程数据包，需指定结束位置等于总大小
	 */
    size_t analysisPacket(char* packet, PacketData& pd); 
};

} // namespace

#endif
