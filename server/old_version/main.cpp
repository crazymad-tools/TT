/*********************************************************
 *
 * Author			: crazy_mad
 * Create time		: 2016-12-29 11:55
 * Filename			: main.cpp
 * Description		: 课程设计《即时通讯系统》的服务器部分
 *
 *********************************************************/

#include "def.h"
#include "MessageTable.h"
#include "MessageAnal.h"
#include "MessagePackage.h"
#include "CMutex.h"

#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

struct socket_parameter {
	int server_fd;
	int max_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	fd_set fdsr;
	struct timeval tv;
};

std::list<int> client_fd;										// 未登录成功的fd链表
std::list<std::pair<std::string, int> > online_list;			// 登陆成功的fd链表
std::map<std::string, std::string> user_info;					// 所有用户的账号密码信息

CMutex online_list_mutex;
socket_parameter m_socket;		// socket参数
MessageTable list_msg;			// 消息列表
MessageAnal anal;				// 解析器
MessagePackage package(&online_list_mutex);			// 打包数据包

void socket_initial();
void user_list_initial();

void add_update(std::string name, int new_fd, bool change_client_fd) {
	if (change_client_fd) {
		for (std::list<int>::iterator it = client_fd.begin(); it != client_fd.end(); it++) {
			if (new_fd == *it) {
				client_fd.erase(it);
				break;
			}
		}
	}
	online_list_mutex.Lock();
	online_list.push_back(std::pair<std::string, int>(name, new_fd));
	online_list_mutex.Unlock();
	uint8_t buf[BUFLENGTH] = {0};
	uint16_t mode = UPDATE;
	memcpy(buf+44, &mode, 2);
	Node *newNode = new Node(new_fd, buf);
	list_msg.front_push(newNode);
}

void* thread_recv(void*) {
	while (true) {
		FD_ZERO(&m_socket.fdsr);
		FD_SET(m_socket.server_fd, &m_socket.fdsr);												// 将服务器套接字加入轮询队列
		for (std::list<int>::iterator it = client_fd.begin(); it != client_fd.end(); it++) {	// 将所有套接字加入轮询队列
			if (*it) {
				FD_SET(*it, &m_socket.fdsr);
			}
		}	
		for (std::list<std::pair<std::string, int> >::iterator it = online_list.begin(); it != online_list.end(); it++) {
			if (it->second) {
				FD_SET(it->second, &m_socket.fdsr);
			}
		}
		int ret = select(m_socket.max_fd+1, &m_socket.fdsr, NULL, NULL, &m_socket.tv);
		if (ret < 0) {
			printf("select error: %s\n", strerror(errno));
			exit(1);
		} else if (ret == 0) {
			continue;
		}
		for (std::list<int>::iterator it = client_fd.begin(); it != client_fd.end();) {		// 遍历所有套接字，查询是否有新的数据需要接受
			if (FD_ISSET(*it, &m_socket.fdsr)) {
				uint8_t buf[BUFLENGTH] = {0};
				int ret = read(*it, buf, BUFLENGTH);
				Node *newNode = new Node(*it, buf);
				list_msg.front_push(newNode);
				if (ret <= 0) {
					printf("client_fd %d 掉线\n", *it);
					std::list<int>::iterator tmp = it;
					it++;
					close(*tmp);
					client_fd.erase(tmp);
					continue;
				}
				char heart[10] = {0};
				memcpy(heart, buf+46, 5);
			}
			it++;
		}	
		for (std::list<std::pair<std::string, int> >::iterator it = online_list.begin(); it != online_list.end();) {
			if (FD_ISSET(it->second, &m_socket.fdsr)) {
				uint8_t buf[BUFLENGTH] = {0};
				int ret = read(it->second, buf, BUFLENGTH);
				Node *newNode = new Node(it->second, buf);
				list_msg.front_push(newNode);
				if (ret <= 0) {
					printf("online %d 掉线\n", it->second);
					close(it->second);
					std::list<std::pair<std::string, int> >::iterator tmp = it;
					it++;
					add_update(tmp->first, tmp->second, false);
					online_list_mutex.Lock();
					online_list.remove(std::pair<std::string, int>(tmp->first, tmp->second));
					online_list_mutex.Unlock();
					continue;
				}
				char heart[10] = {0};
				memcpy(heart, buf+46, 5);
			}
			it++;
		}
		if (FD_ISSET(m_socket.server_fd, &m_socket.fdsr)) {			// 假如本地套接字发生改变，表明有新的连接请求
			socklen_t len = sizeof(m_socket.client_addr);
			memset(&m_socket.client_addr, 0, sizeof(m_socket.client_addr));
		 	int new_fd = 0;
			new_fd = accept(m_socket.server_fd, (struct sockaddr*)&m_socket.client_addr, &len);
			client_fd.push_back(new_fd);
	   		m_socket.max_fd = m_socket.max_fd > new_fd ? m_socket.max_fd : new_fd;   
			printf("new connect %d %d\n", new_fd, m_socket.max_fd);
		}
	}
	return NULL;
}

void* thread_anal(void*) {
	while (true) {
		if (!list_msg.empty()) {
			anal.anal_buf(list_msg.rear()->buf, list_msg.rear()->client_fd);
			list_msg.rear_pop();
			uint16_t mode = anal.getMode();
			switch (mode) {
				case LOGIN: {									// 回应登陆请求
					printf("login\n");
					uint8_t buf[BUFLENGTH] = {0};
					package.make_login_success(buf);
					std::string name = (char*)anal.getAccountFrom();
					std::string passwd = (char*)anal.getPasswdFrom();
					std::map<std::string, std::string>::iterator it = user_info.find(name);
					if (it == user_info.end()) {				// 即不存在该用户
						package.make_login_failed(buf);
					} else {
						if (passwd == it->second) {				// 登陆成功
							package.make_login_success(buf);
							int new_fd = anal.getClientFd();
							add_update(name, new_fd, true);
						} else {								// 密码错误
							package.make_login_failed(buf);
						}
					}
					write(anal.getClientFd(), buf, BUFLENGTH);	// 发送数据包
					break;
				}
				case HEART: {									// 回应心跳请求
					//printf("heart\n");
					uint8_t buf[BUFLENGTH] = {0};
					package.make_heart(buf);
					write(anal.getClientFd(), buf, BUFLENGTH);
					break;
				}
				case COMMUNI: {									// 回应通信请求
					uint8_t buf[BUFLENGTH] = {0};
					std::string name = (char*)anal.getAccountFrom();
					package.make_new_msg(name, buf, anal.getMsg());
					name = (char*)anal.getAccountTo();
					for (std::list<std::pair<std::string, int> >::iterator it = online_list.begin(); it != online_list.end(); it++){
						if (it->first == name) {
							write(it->second, buf, BUFLENGTH);
							break;
						}
					}
					break;
				}
				case UPDATE: {
					printf("update\n");
					write(1, "update\n", 8);
					uint8_t buf[BUFLENGTH] = {0};
					package.make_update(buf, online_list);
					for (std::list<std::pair<std::string, int> >::iterator it = online_list.begin(); it != online_list.end(); it++) {
						write(it->second, buf, BUFLENGTH);
						std::cout << "send to " << it->first << " update" << std::endl; 
					}
					break;
				}
			}
		}
	}
	return NULL;
}

int main(int argc, char* argv[]) {
	pthread_t handle_recv;			// 数据接受线程文件描述符
	pthread_t handle_anal;			// 数据处理线程文件描述符
	socket_initial();				// 初始化socket
	user_list_initial();			// 初始化用户列表

	pthread_create(&handle_recv, NULL, thread_recv, NULL);	// 开启数据接受线程
	pthread_create(&handle_anal, NULL, thread_anal, NULL);	// 开启数据处理线程

	pthread_join(handle_recv, NULL);						
	pthread_join(handle_anal, NULL);
	return 0;
}

void socket_initial() {
	int on = 1;
	m_socket.server_fd = socket(AF_INET, SOCK_STREAM, 0);
	m_socket.server_addr.sin_family = AF_INET;
	m_socket.server_addr.sin_port = htons(8083);
	m_socket.server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (setsockopt(m_socket.server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
		printf("setsocket error: %s\n", strerror(errno));
		exit(1);
	} else {
		printf("setsockopt success!\n");
	}
	if (bind(m_socket.server_fd, (struct sockaddr*)&m_socket.server_addr, sizeof(m_socket.server_addr)) == -1) {
		printf("bind error: %s\n", strerror(errno));
		exit(1);
	} else {
		printf("bind success!\n");
	}
	listen(m_socket.server_fd, 20);

	m_socket.tv.tv_sec = 30;
	m_socket.tv.tv_usec = 0;
	m_socket.max_fd = m_socket.server_fd;
	socklen_t len = sizeof(m_socket.client_addr);
	memset(&m_socket.client_addr, 0, sizeof(m_socket.client_addr));
}

void user_list_initial() {
	std::ifstream fin("user.list"); 
	std::string username;
	std::string userpasswd;
	std::cout << std::endl;
	while (fin >> username >> userpasswd) {
		std::cout << username << ' ' << userpasswd << std::endl;
		user_info.insert(std::pair<std::string, std::string>(username, userpasswd));
	}
	fin.close();
}
