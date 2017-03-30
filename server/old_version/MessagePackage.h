/*********************************************************
 * Author			: crazymad
 * Last modified	: 2016-12-30 18:23
 * Filename			: MessagePackage.h
 * Description		: 
 *********************************************************/

#ifndef _MESSAGEPACKAGE_H
#define _MESSAGEPACKAGE_H

#include "def.h"
#include "CMutex.h"
#include <list>

class MessagePackage {
private:
	uint8_t user_info[40];
	CMutex *online_list_mutex;

public:
	MessagePackage(CMutex *online_list_mutex);
	void make_login_success(uint8_t *buf);
	void make_login_failed(uint8_t *buf);
	void make_heart(uint8_t *buf);
	void make_update(uint8_t *buf, std::list<std::pair<std::string, int> > &online_list);
	void make_new_msg(std::string name, uint8_t *buf, uint8_t *msg);
};

#endif

