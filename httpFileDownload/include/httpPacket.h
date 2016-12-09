/***********************************************************
# File Name		: httpPacket.h
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月05日 星期一 23时28分07秒
# Descrition	: 主要功能有：构造http包，解析http包 
***********************************************************/

#ifndef HTTPFILEDOWNLOAD_HTTPPACKET_H
#define HTTPFILEDOWNLOAD_HTTPPACKET_H

#include "../include/logger.h"
#include "../include/analysisURL.h"

namespace httpfiledownload {


struct PacketData {
    char*  m_data; //download data
	size_t m_begin; //download range begin
	size_t m_end; //download range end
    size_t m_contentLength; //download file total size

    PacketData():m_data(NULL), m_begin(0), m_end(0), m_contentLength(0) {}
};

struct PacketConfig {
	size_t m_begin;
	size_t m_end;
};

class HttpPacket {
public:
    string constructGetPacket(const AnalysisURL& an, const PacketConfig& conf);
    string constructHeadPacket(const AnalysisURL& an);

    size_t analysisPacket(char* packet, PacketData& pd); 
};

} // namespace

#endif
