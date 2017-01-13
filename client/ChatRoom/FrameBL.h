#pragma once

#include "MessageTable.h"
#include "MessagePackage.h"	// 用于对数据包进行打包
#include "MessageAnal.h"
#include "UserInfo.h"

struct SocketParameters {
	SOCKET fd;							// 套接字
	struct sockaddr_in server_addr;		// 服务器地址
	MessageTable messages;				// 消息队列
	MessagePackage m_MessagePackage;	// 用于打包数据包
};

/*线程句柄*/
static HANDLE handle_recv;			// 数据接收线程
static HANDLE handle_anal;			// 数据解析线程
static HANDLE handle_heart;			// 心跳机制线程（维持在线状态）
/*线程锁*/
static HANDLE Mutex = CreateMutex(NULL, FALSE, NULL);

class MessageAnal;

// 三层架构中的BL业务处理层
class FrameBL
{
public:
	FrameBL(int);
	~FrameBL();

	void initalSocket();	// 初始化socket
	void set_account_passwd(uint8_t *account_buf, uint8_t *passwd_buf);
	void open_thread();
	SocketParameters* getSocket();
	UserInfo* getUserInfo();
	MessageAnal* getAnal();
private:
	SocketParameters *m_socket;		// socket参数
	UserInfo *m_UserInfo;			// 在线列表及消息列表
	MessageAnal *m_MessageAnal;
};


// 消息接受线程
static DWORD WINAPI thread_recv(LPVOID pM) {
	SocketParameters *socket = (SocketParameters*)pM;
	Node *newNode = NULL;
	while (true) {
		uint8_t buf[256] = { 0 };
		int ret = recv(socket->fd, (char*)buf, BUFLENGTH, 0);
		if (ret > 0) {
			newNode = new Node(NULL, NULL, buf);
			WaitForSingleObject(Mutex, INFINITE);		// 加上线程锁，防止冲突
			socket->messages.head_push(newNode);
			ReleaseMutex(Mutex);						// 接触线程锁
			/*send(1, (char*)buf+6, 6, 0);*/
			//printf("%s\n", buf + 6);
		}
		else {
			MessageBox(NULL, _T("掉线"), _T("掉线"), NULL);
			break;
		}
	}
	return TRUE;	
}
// 消息处理线程
static DWORD WINAPI thread_anal(LPVOID pM) {
	FrameBL *PFrameBL = (FrameBL*)pM;
	SocketParameters *socket = PFrameBL->getSocket();

	Node tmp;
	while (true) {
		WaitForSingleObject(Mutex, INFINITE);			// 加上线程锁，防止冲突
		if (!socket->messages.empty()) {
			tmp = *socket->messages.rear();
			socket->messages.tail_pop();
		}
		else {
			ReleaseMutex(Mutex);						// 解除线程锁
			continue;
		}
		ReleaseMutex(Mutex);							// 解除线程锁
		printf("%s\n", (tmp.buf+6));
		PFrameBL->getAnal()->anal_buf(&tmp);
	}
}

// 心跳机制线程
static DWORD WINAPI thread_heart(LPVOID pM) {
	SocketParameters *socket = (SocketParameters*)pM;
	uint8_t buf[256] = { 0 };
	socket->m_MessagePackage.make_heart_package(buf);
	while (true) {
		Sleep(1000);
		send(socket->fd, (char*)buf, 256, 0);
	}
}
