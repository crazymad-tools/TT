#pragma once

#include "MessageTable.h"
#include "MessagePackage.h"	// ���ڶ����ݰ����д��
#include "MessageAnal.h"
#include "UserInfo.h"

struct SocketParameters {
	SOCKET fd;							// �׽���
	struct sockaddr_in server_addr;		// ��������ַ
	MessageTable messages;				// ��Ϣ����
	MessagePackage m_MessagePackage;	// ���ڴ�����ݰ�
};

/*�߳̾��*/
static HANDLE handle_recv;			// ���ݽ����߳�
static HANDLE handle_anal;			// ���ݽ����߳�
static HANDLE handle_heart;			// ���������̣߳�ά������״̬��
/*�߳���*/
static HANDLE Mutex = CreateMutex(NULL, FALSE, NULL);

class MessageAnal;

// ����ܹ��е�BLҵ�����
class FrameBL
{
public:
	FrameBL(int);
	~FrameBL();

	void initalSocket();	// ��ʼ��socket
	void set_account_passwd(uint8_t *account_buf, uint8_t *passwd_buf);
	void open_thread();
	SocketParameters* getSocket();
	UserInfo* getUserInfo();
	MessageAnal* getAnal();
private:
	SocketParameters *m_socket;		// socket����
	UserInfo *m_UserInfo;			// �����б���Ϣ�б�
	MessageAnal *m_MessageAnal;
};


// ��Ϣ�����߳�
static DWORD WINAPI thread_recv(LPVOID pM) {
	SocketParameters *socket = (SocketParameters*)pM;
	Node *newNode = NULL;
	while (true) {
		uint8_t buf[256] = { 0 };
		int ret = recv(socket->fd, (char*)buf, BUFLENGTH, 0);
		if (ret > 0) {
			newNode = new Node(NULL, NULL, buf);
			WaitForSingleObject(Mutex, INFINITE);		// �����߳�������ֹ��ͻ
			socket->messages.head_push(newNode);
			ReleaseMutex(Mutex);						// �Ӵ��߳���
			/*send(1, (char*)buf+6, 6, 0);*/
			//printf("%s\n", buf + 6);
		}
		else {
			MessageBox(NULL, _T("����"), _T("����"), NULL);
			break;
		}
	}
	return TRUE;	
}
// ��Ϣ�����߳�
static DWORD WINAPI thread_anal(LPVOID pM) {
	FrameBL *PFrameBL = (FrameBL*)pM;
	SocketParameters *socket = PFrameBL->getSocket();

	Node tmp;
	while (true) {
		WaitForSingleObject(Mutex, INFINITE);			// �����߳�������ֹ��ͻ
		if (!socket->messages.empty()) {
			tmp = *socket->messages.rear();
			socket->messages.tail_pop();
		}
		else {
			ReleaseMutex(Mutex);						// ����߳���
			continue;
		}
		ReleaseMutex(Mutex);							// ����߳���
		printf("%s\n", (tmp.buf+6));
		PFrameBL->getAnal()->anal_buf(&tmp);
	}
}

// ���������߳�
static DWORD WINAPI thread_heart(LPVOID pM) {
	SocketParameters *socket = (SocketParameters*)pM;
	uint8_t buf[256] = { 0 };
	socket->m_MessagePackage.make_heart_package(buf);
	while (true) {
		Sleep(1000);
		send(socket->fd, (char*)buf, 256, 0);
	}
}
