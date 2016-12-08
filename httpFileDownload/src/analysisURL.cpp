/***********************************************************
# File Name		: ../src/analysisURL.cpp
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月01日 星期四 08时49分31秒
# Descrition	: 
***********************************************************/

#include "../include/analysisURL.h"
#include "../include/logger.h"
#include <stdlib.h>
#include <netdb.h>

namespace httpfiledownload {

AnalysisURL::AnalysisURL(char* _url) {
    if (_url == NULL) {
		LogError("The URL can't be NULL\n");
		exit(EXIT_FAILURE);
    }

    string url = string(_url); 
	LogDebug("download from: %s\n", _url);

    size_t hostHead = 0, hostEnd = url.length();   

    hostHead = url.find_first_of("://", hostHead);
    if (hostHead != string::npos && hostHead+1 < url.length()) {
		hostHead += 3;	
    } else
		hostHead = 0;

    hostEnd = url.find_first_of('/', hostHead);
    if (hostEnd == string::npos) { //get path
		m_path = "/";
		hostEnd = url.length();
    } else {
		m_path = url.substr(hostEnd);
    }
	LogDebug("The path is: %s\n", m_path.c_str());

	url = url.substr(hostHead, hostEnd-hostHead);
	hostHead = 0, hostEnd = url.length();
	hostHead = url.find_first_of('@', hostHead);
	if (hostHead != string::npos) {
		url = url.substr(hostHead+1);
	}
	hostHead = 0, hostEnd = url.length();
	
    hostEnd = url.find_first_of(':', hostHead);
    if (hostEnd == string::npos) { //get port
		m_port = 80;	
		hostEnd = url.length();
    } else { 
		m_port = (unsigned int)strtol(url.substr(hostEnd+1, url.length()-hostEnd).c_str(), NULL, 10);
    }
	LogDebug("The port is: %u\n", m_port); 
    m_hostName = url.substr(hostHead, hostEnd-hostHead); //get hostName

	LogDebug("The hostName is: %s\n", m_hostName.c_str());
    struct hostent* m_pURL = gethostbyname(m_hostName.c_str());
    if (m_pURL == NULL) {
		LogError("The URL is wrong\n");
		exit(EXIT_FAILURE);
    }
    m_IP =  *(unsigned long*)m_pURL->h_addr_list[0];
	LogDebug("The analysis IP is : %u\n", m_IP);
}

} // namespace
