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
	case LOGIN_RESPONSE:			// ��½��Ӧ
		anal_login(node->buf);
		break;
	case HEART:						// ������
		break;
	case UPDATE:					// ���������б�
		WaitForSingleObject(UI_Mutex, INFINITE);			// �����߳�������ֹ��ͻ
		anal_update(node->buf);
		ReleaseMutex(UI_Mutex);								// ����߳���
		break;
	case COMMUNI:					// �µ���Ϣ
		WaitForSingleObject(UI_Mutex, INFINITE);			// �����߳�������ֹ��ͻ
		anal_new_msg(node->buf);
		ReleaseMutex(UI_Mutex);								// ����߳���
		break;
	}
}
// ������½��
bool MessageAnal::anal_login(uint8_t *buf) {
	return (!strcmp("login", (char*)(buf + 6)));
}
// �����б���°�
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
// �����µ���Ϣ��
void MessageAnal::anal_new_msg(uint8_t *buf) {
	std::string account = (char*)(buf + 6);
	std::string msg = (char*)(buf + 26);
	printf("new message from %s what is '%s'\n", account.c_str(), msg.c_str());
	m_UserInfo->new_message(account, (char*)(buf + 26));
	MessageBeep(MB_ICONHAND);
}
