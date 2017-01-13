/*********************************************************
 * Author			: crazymad
 * Last modified	: 2016-12-29 12:07
 * Filename			: MessageAnal.h
 * Description		: 
 *********************************************************/

#ifndef _MESSAGEANAL_H
#define _MESSAGEANAL_H

#include "def.h"
#define mem(a) memset((a), 0, sizeof((a)))

class MessageAnal {
private:
	uint8_t account_from[20];		
	uint8_t account_to[20];			 
	uint8_t passwd_from[20];
	uint8_t msg[256];
	uint16_t mode;
	int client_fd;

public:
	uint16_t getMode();
	uint8_t* getMsg();
	uint8_t* getAccountFrom();
	uint8_t* getAccountTo();
	uint8_t* getPasswdFrom();;	
	int getClientFd();
	void anal_buf(uint8_t *buf, int fd);

private:
	void anal_login(uint8_t *buf);
	void anal_heart(uint8_t *buf);
	void anal_beak(uint8_t *buf);
	void anal_communi(uint8_t *buf);
};

#endif

