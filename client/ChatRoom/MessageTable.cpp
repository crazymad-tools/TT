#include "stdafx.h"
#include "MessageTable.h"


MessageTable::MessageTable() : m_length (100)
{
	head = new Node;
	tail = head;
	m_size = 0;
}


MessageTable::~MessageTable()
{
}
// β�巨
void MessageTable::tail_push(Node *newNode) {
	tail->next = newNode;
	newNode->prev = tail;
	tail = newNode;
	m_size++;
}
// ͷ�巨
void MessageTable::head_push(Node *newNode) {
	if (head == tail) {
		tail_push(newNode);
		return;
	}
	head->next->prev = newNode;
	newNode->next = head->next;
	newNode->prev = head;
	head->next = newNode;
	m_size++;
}
// βɾ
void MessageTable::tail_pop() {
	if (head == tail) {
		return;
	}
	tail = tail->prev;
	delete tail->next;
	tail->next = NULL;
	m_size--;
}
// ͷɾ
void MessageTable::head_pop() {
	if (tail == head) {
		return;
	}
	else if (head->next == tail) {
		tail_pop();
		return;
	}
	Node *tmp = head->next;
	head->next = head->next->next;
	head->next->prev = head;
	delete tmp;
	m_size--;
}

bool MessageTable::empty() {
	return head == tail;
}

Node* MessageTable::front() {
	return head->next;
}

Node* MessageTable::rear() {
	return head == tail ? NULL : tail;
}

int MessageTable::size() {
	return m_size;
}