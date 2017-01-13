#pragma once

#include "def.h"

// �ڵ�ṹ��
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
	uint8_t buf[BUFLENGTH];		// socket���ݰ�
	Node *next;
	Node *prev;
};

// ˫��������
class MessageTable
{
public:
	MessageTable();
	~MessageTable();

	void head_push(Node *newNode);		// ͷ�巨
	void head_pop();		// ͷɾ
	void tail_push(Node *newNode);		// β�巨
	void tail_pop();		// βɾ
	Node* front();			// ��ȡͷ�ڵ�
	Node* rear();			// ��ȡβ�ڵ�
	bool empty();			// �ж϶����Ƿ�Ϊ
	int size();				// 

private:
	Node *head;			// ͷ�ڵ�
	Node *tail;			// β�ڵ�
	int m_size;			// ��ǰ���г���
	int m_length;		// ����г���		
};
