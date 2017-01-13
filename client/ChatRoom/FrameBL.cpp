#include "stdafx.h"
#include "FrameBL.h"


FrameBL::FrameBL(int)
{
	m_socket = new SocketParameters();
	m_UserInfo = new UserInfo();
	m_MessageAnal = new MessageAnal(m_UserInfo);
	initalSocket();
}

FrameBL::~FrameBL()
{
}

// ��ʼ���ͻ��ˣ������ӿͻ���
void FrameBL::initalSocket() {
	WSADATA wsadata;
	WSAStartup(0x0202, &wsadata);
	bool opt = true;
	m_socket->fd = socket(AF_INET, SOCK_STREAM, 0);
	m_socket->server_addr.sin_family = AF_INET;
	m_socket->server_addr.sin_port = htons(8083);
	m_socket->server_addr.sin_addr.s_addr = inet_addr("115.159.154.139");
	setsockopt(m_socket->fd, SOL_SOCKET, SO_REUSEADDR, (char *FAR)&opt, sizeof(opt));		// ����socketģʽ
	
	if (connect(m_socket->fd, (struct sockaddr*)&m_socket->server_addr, sizeof(m_socket->server_addr)) == -1) {
		MessageBox(NULL, _T("���Ӳ��Ϸ�����"), _T("����"), NULL);
	}
}

void FrameBL::set_account_passwd(uint8_t *account_buf, uint8_t *passwd_buf) {
	m_socket->m_MessagePackage.set_account_passwd(account_buf, passwd_buf);
}

SocketParameters* FrameBL::getSocket() {
	return m_socket;
}

UserInfo* FrameBL::getUserInfo() {
	return m_UserInfo;
}

MessageAnal* FrameBL::getAnal() {
	return m_MessageAnal;
}

void FrameBL::open_thread() {
	handle_recv = CreateThread(NULL, 0, thread_recv, m_socket, 0, NULL);		// �������������߳�
	handle_anal = CreateThread(NULL, 0, thread_anal, this, 0, NULL);		// �������������߳�
	handle_heart = CreateThread(NULL, 0, thread_heart, m_socket, 0, NULL);		// �������������߳�
}