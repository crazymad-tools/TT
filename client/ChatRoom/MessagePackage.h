#pragma once

#include "def.h"

// 用于打包数据包
class MessagePackage
{
public:
	MessagePackage();
	~MessagePackage();

	void set_account_passwd(uint8_t *account_buf, uint8_t *passwd_buf);
	void make_conmmuni_package(uint8_t *buf, uint8_t *user_account, uint8_t *Msg, int Msg_length);
	void make_heart_package(uint8_t *buf);
	void make_login_package(uint8_t *buf);
	void make_loginout_package(uint8_t *buf);

private:;
	uint8_t user_msg_buf[42];
};

