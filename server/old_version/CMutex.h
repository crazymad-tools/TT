/*********************************************************
 * Author			: crazymad
 * Last modified	: 2016-12-31 18:14
 * Filename			: CMutex.h
 * Description		: 
 *********************************************************/

#ifndef _CMUTEX_H
#define _CMUTEX_H

#include <pthread.h>

class CMutex {
private:
	pthread_mutex_t mutex;

public:
	CMutex() {
		mutex = PTHREAD_MUTEX_INITIALIZER;
	}
	~CMutex() {

	}
	void Lock() {
		pthread_mutex_lock(&mutex);
	}
	void Unlock() {
		pthread_mutex_unlock(&mutex);
	}
};

#endif

