/*********************************************************
 * Author			: crazymad
 * Last modified	: 2016-12-29 13:13
 * Filename			: MessageTable.h
 * Description		: 
 *********************************************************/

#ifndef _MESSAGETABLE_H
#define _MESSAGETABLE_H

#include "def.h"

struct Node {
	Node (int fd, uint8_t *buf) {
		if (buf)
			memcpy(this->buf, buf, BUFLENGTH);
		prev = NULL;
		next = NULL;
		this->client_fd = fd;
	}
	uint8_t buf[BUFLENGTH];
	int client_fd;
	Node *prev;
	Node *next;
};

class MessageTable {
private:
	Node *head;
	Node *tail;
	int m_size;
	int m_length;

public:
	MessageTable();
	~MessageTable();

	void front_push(Node *newNode);		// 头插法
	void front_pop();					// 头删
	void rear_push(Node *newNode);		// 尾插法
	void rear_pop();					// 尾扇
	Node* front();						// 获取头节点
	Node* rear();						// 获取尾节点
	bool empty();						// 判断链表是否为空
	int size();							// 获取链表长度

};



#endif

