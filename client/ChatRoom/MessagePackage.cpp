#include "stdafx.h"
#include "MessagePackage.h"


MessagePackage::MessagePackage()
{

}

MessagePackage::~MessagePackage()
{

}

void MessagePackage::set_account_passwd(uint8_t *account_buf, uint8_t *passwd_buf) {
	memset(user_msg_buf, 0, sizeof(user_msg_buf));
	memcpy(user_msg_buf, account_buf, 20);
	memcpy(user_msg_buf + 20, passwd_buf, 20);
}

// ���������
void MessagePackage::make_heart_package(uint8_t *buf) {
	memset(buf, 0, BUFLENGTH);
	uint32_t buf_length = 40 + 2 + 4 + 5;		// �˺š����롢�������͡����ݰ��ܳ���
	uint16_t op = HEART;
	buf_length += strlen("heart");
	memcpy(buf, (uint8_t*)&buf_length, 4);
	memcpy(buf + 4, user_msg_buf, 40);
	memcpy(buf + 44, (uint8_t*)&op, 2);
	memcpy(buf + 46, (uint8_t*)("heart"), 5);
}
// �����½�����
void MessagePackage::make_login_package(uint8_t *buf) {
	memset(buf, 0, BUFLENGTH);
	uint32_t buf_length = 40 + 2 + 4 + 5;		// �˺š����롢�������͡����ݰ��ܳ���
	uint16_t op = LOGIN;
	buf_length += strlen("login");
	memcpy(buf, (uint8_t*)&buf_length, 4);
	memcpy(buf + 4, user_msg_buf, 40);
	memcpy(buf + 44, (uint8_t*)&op, 2);
	memcpy(buf + 46, (uint8_t*)("login"), 5);
}
// ���ͨ�������
void MessagePackage::make_conmmuni_package(uint8_t *buf, uint8_t *user_account, uint8_t *Msg, int Msg_length) {
	memset(buf, 0, BUFLENGTH);
	uint32_t buf_length = 40 + 2 + 4 + 20 + Msg_length;		// �˺š����롢�������͡����ݰ��ܳ���
	uint16_t op = COMMUNI;
	buf_length += strlen("login");
	memcpy(buf, (uint8_t*)&buf_length, 4);
	memcpy(buf + 4, user_msg_buf, 40);
	memcpy(buf + 44, (uint8_t*)&op, 2);
	memcpy(buf + 46, user_account, 20);
	memcpy(buf + 66, Msg, Msg_length);
	printf("send msg:%s length:%d\n", (char*)Msg, Msg_length);
}
// ����ǳ���
void MessagePackage::make_loginout_package(uint8_t *buf) {
	memset(buf, 0, BUFLENGTH);
	uint32_t buf_length = 40 + 2 + 4 + 6;		// �˺š����롢�������͡����ݰ��ܳ���
	uint16_t op = LOGIN;
	buf_length += strlen("login");
	memcpy(buf, (uint8_t*)&buf_length, 4);
	memcpy(buf + 4, user_msg_buf, 40);
	memcpy(buf + 44, (uint8_t*)&op, 2);
	memcpy(buf + 46, (uint8_t*)("logout"), 6);
}
