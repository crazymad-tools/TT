#include "stdafx.h"
#include "UserInfo.h"


UserInfo::UserInfo()
{
	statu = true;			// 表明UI层的列表需要更新
	message_statu = true;	// 表明UI层的新消息列需要更新
}


UserInfo::~UserInfo()
{
}

void UserInfo::set_statu(bool flag) {
	statu = flag;
}

void UserInfo::set_message_statu(bool flag) {
	message_statu = flag;
}

void UserInfo::clear_list() {
	user_list.clear();
}

void UserInfo::insert_list(std::string account) {
	user_list.insert(std::pair<std::string, InfoList>(account, InfoList()));
}

void UserInfo::new_message(std::string account, char buf[]) {
	std::map<std::string, InfoList>::iterator it = user_list.find(account);
	printf("%s add a new msg\n", it->first.c_str());
	if (it != user_list.end()) {
		it->second.msg_list.push_back(std::string(account) + ": " + buf);
		it->second.new_num++;
	}
	message_statu = true;
	statu = true;
}

void UserInfo::reset_new_num(std::string account) {
	std::map<std::string, InfoList>::iterator it = user_list.find(account);
	if (it != user_list.end()) {
		it->second.new_num = 0;
	}
}

bool UserInfo::get_statu() {
	return statu;
}

bool UserInfo::get_message_statu() {
	return message_statu;
}

std::map<std::string, InfoList>* UserInfo::getUserList() {
	return &user_list;
}
