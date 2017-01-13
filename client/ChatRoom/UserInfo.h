#pragma once

#include "def.h"
#include <map>
#include <string>
#include <vector>

struct InfoList {
	InfoList() {
		new_num = 0;
	}
	std::vector<std::string> msg_list;			// 所有消息的列表
	int new_num;								// 新的消息的列表
};

class UserInfo
{
public:
	UserInfo();
	~UserInfo();

	void set_statu(bool flag);
	void set_message_statu(bool flag);
	void clear_list();
	void insert_list(std::string account);
	void new_message(std::string account, char buf[]);
	void reset_new_num(std::string account);
	bool get_statu();
	bool get_message_statu();
	std::map<std::string, InfoList>* getUserList();

private:
	std::map<std::string, InfoList> user_list;			// 保存用户目录以及用户发送过来的新信息
	bool statu;											// 标记是否有下线或者上线信息
	bool message_statu;									// 标记是否有新消息
};
