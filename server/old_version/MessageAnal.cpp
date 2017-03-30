/*********************************************************
 * Author			: crazymad
 * Last modified	: 2016-12-29 12:07
 * Filename			: MessageAnal.cpp
 * Description		: 
 *********************************************************/

#include "MessageAnal.h"
#include <string.h>

uint16_t MessageAnal::getMode() {
	return mode;
}

uint8_t* MessageAnal::getMsg() {
	return msg;
}

uint8_t* MessageAnal::getAccountFrom() {
	return account_from;
}

uint8_t* MessageAnal::getAccountTo() {
	return account_to;
}

uint8_t* MessageAnal::getPasswdFrom() {
	return passwd_from;
}

int MessageAnal::getClientFd() {
	return client_fd;
}

void MessageAnal::anal_buf(uint8_t *buf, int fd) {
	client_fd = fd;
	memcpy(&mode, buf+44, 2);
	mem(account_from);
	mem(account_to);
	mem(passwd_from);
	mem(msg);
	switch (mode) {
		case LOGIN:
			anal_login(buf);
			break;
		case HEART:
			break;
		case BREAK:
			break;
		case COMMUNI:
			anal_communi(buf);
			break;
	}
}

void MessageAnal::anal_login(uint8_t *buf) {
	memcpy(account_from, buf+4, 20);
	memcpy(passwd_from, buf+24, 20);
}

void MessageAnal::anal_heart(uint8_t *buf) {
	memcpy(account_from, buf+6, 20);
}
	
void MessageAnal::anal_beak(uint8_t *buf) {
	memcpy(account_from, buf+4, 20);
}
	
void MessageAnal::anal_communi(uint8_t *buf) {
	uint32_t buf_length;
	memcpy(&buf_length, buf, 4);
	memcpy(account_from, buf+4, 20);
	memcpy(account_to, buf+46, 20);
	memcpy(msg, buf+66, buf_length-66);
	printf("msg:%s from:%s to:%s\n", (char*)msg, (char*)account_from, (char*)account_to);
}

