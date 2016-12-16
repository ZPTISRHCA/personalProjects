/***********************************************************
# File Name		: sem.h
# Author		: zpt
# mail			: zpt1596723345@live.com
# Created Time	: 2016年12月11日 星期日 02时01分34秒
# Descrition	: 信号量，实现线程间数据的互斥访问
***********************************************************/

#ifndef HTTPFILEDOWNLOAD_SEM_H
#define HTTPFILEDOWNLOAD_SEM_H

#include "../include/logger.h"
#include <semaphore.h>
#include <stdlib.h>
#include <errno.h>

namespace httpfiledownload {

//信号量
class Sem {
	sem_t m_sem;

public:
	Sem() {
		if (sem_init(&m_sem, 0, 1) != 0) {
			LogError("sem init error\n");
			exit(0);
		}
	}
	//p操作，信号量值加1
	inline void semP() {
		if (sem_post(&m_sem) == -1) {
			LogError("thread failed to unlock semaphore\n");
		}
	}
	//v操作,信号量值减一
	inline void semV() {
		while (sem_wait(&m_sem) == -1) {
			if (errno != EINTR) {
				LogError("thread failed to lock semaphore\n");
				exit(0);
			}
		}
	}
};

} //namespace

#endif
