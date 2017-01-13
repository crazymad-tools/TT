#pragma once

#include "def.h"
#include <map>
#include <string>
#include <vector>

struct InfoList {
	InfoList() {
		new_num = 0;
	}
	std::vector<std::string> msg_list;			// ������Ϣ���б�
	int new_num;								// �µ���Ϣ���б�
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
	std::map<std::string, InfoList> user_list;			// �����û�Ŀ¼�Լ��û����͹���������Ϣ
	bool statu;											// ����Ƿ������߻���������Ϣ
	bool message_statu;									// ����Ƿ�������Ϣ
};
