/*********************************************************
 * Author			: crazymad
 * Last modified	: 2016-12-29 13:13
 * Filename			: MessageTable.cpp
 * Description		: 
 *********************************************************/


#include "MessageTable.h"

MessageTable::MessageTable() {
	head = new Node(0, NULL);
	tail = head;
}
MessageTable::~MessageTable() {
	while (head != tail) {
		rear_pop();
	}
	delete head;
}

void MessageTable::front_push(Node *newNode) {
	if (head == tail) {
		rear_push(newNode);
		return;
	}
	head->next->prev = newNode;
	newNode->next = head->next;
	newNode->prev = head;
	head->next = newNode;
	m_size++;
}

void MessageTable::front_pop() {
	if (head->next == tail) {
		rear_pop();
		return;
	}
	Node *tmp = head->next;
	head->next = tmp->next;
	tmp->next->prev = head;
	delete tmp;
	m_size--;
}
void MessageTable::rear_push(Node *newNode) {
	tail->next = newNode;
	newNode->prev = tail;
	tail = newNode;
	m_size++;
}
void MessageTable::rear_pop() {
	tail = tail->prev;
	delete tail->next;
	tail->next = NULL;
	m_size--;
}
Node* MessageTable::front() {
	return head->next;
}
Node* MessageTable::rear() {
	return head == tail ? NULL : tail;
}
bool MessageTable::empty() {
	return head == tail;
}
int MessageTable::size() {
	return m_size;	
}

