#pragma once

#include "UserInfo.h"
#include "MessageTable.h"

// 数据包解析类
class MessageAnal
{
public:
	MessageAnal(UserInfo *m_UserInfo);
	~MessageAnal();

	void anal_buf(Node *node);
	bool anal_login(uint8_t *buf);
	void anal_update(uint8_t *buf);
	void anal_new_msg(uint8_t *buf);

private:
	UserInfo *m_UserInfo;
};

