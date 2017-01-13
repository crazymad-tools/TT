#include "stdafx.h"
#include "MessageAnal.h"

MessageAnal::MessageAnal(UserInfo *m_UserInfo)
{
	this->m_UserInfo = m_UserInfo;
}


MessageAnal::~MessageAnal()
{

}

void MessageAnal::anal_buf(Node *node) {
	uint32_t buf_length = *(uint32_t*)(node->buf);
	uint16_t mode = *(uint16_t*)(node->buf+4);
	switch (mode) {
	case LOGIN_RESPONSE:			// 登陆回应
		anal_login(node->buf);
		break;
	case HEART:						// 心跳包
		break;
	case UPDATE:					// 更新在线列表
		WaitForSingleObject(UI_Mutex, INFINITE);			// 加上线程锁，防止冲突
		anal_update(node->buf);
		ReleaseMutex(UI_Mutex);								// 解除线程锁
		break;
	case COMMUNI:					// 新的消息
		WaitForSingleObject(UI_Mutex, INFINITE);			// 加上线程锁，防止冲突
		anal_new_msg(node->buf);
		ReleaseMutex(UI_Mutex);								// 解除线程锁
		break;
	}
}
// 解析登陆包
bool MessageAnal::anal_login(uint8_t *buf) {
	return (!strcmp("login", (char*)(buf + 6)));
}
// 解析列表更新包
void MessageAnal::anal_update(uint8_t *buf) {
	m_UserInfo->set_statu(true);
	uint16_t user_num = *(uint16_t*)(buf + 6);
	printf("update num: %d\n", user_num);
	m_UserInfo->clear_list();
	for (int i = 0; i < (int)user_num; i++) {
		std::string account = (char*)(buf + (i * 20) + 8);
		m_UserInfo->insert_list(account);
		printf("%s\t", account.c_str());
	}
	printf("\n");
}
// 解析新的消息包
void MessageAnal::anal_new_msg(uint8_t *buf) {
	std::string account = (char*)(buf + 6);
	std::string msg = (char*)(buf + 26);
	printf("new message from %s what is '%s'\n", account.c_str(), msg.c_str());
	m_UserInfo->new_message(account, (char*)(buf + 26));
	MessageBeep(MB_ICONHAND);
}
