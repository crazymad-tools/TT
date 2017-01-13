#pragma once

#include "def.h"

// 节点结构体
struct Node {
	Node(Node *next = NULL, Node *prev = NULL, uint8_t *buf = NULL) {
		this->next = next;
		this->prev = prev;
		if (buf) {
			memcpy(this->buf, buf, BUFLENGTH);
		}	
	}
	void operator=(Node &tmp) {
		memcpy(buf, tmp.buf, BUFLENGTH);
	}
	uint8_t buf[BUFLENGTH];		// socket数据包
	Node *next;
	Node *prev;
};

// 双向链表类
class MessageTable
{
public:
	MessageTable();
	~MessageTable();

	void head_push(Node *newNode);		// 头插法
	void head_pop();		// 头删
	void tail_push(Node *newNode);		// 尾插法
	void tail_pop();		// 尾删
	Node* front();			// 获取头节点
	Node* rear();			// 获取尾节点
	bool empty();			// 判断队列是否为
	int size();				// 

private:
	Node *head;			// 头节点
	Node *tail;			// 尾节点
	int m_size;			// 当前队列长度
	int m_length;		// 最长队列长度		
};
