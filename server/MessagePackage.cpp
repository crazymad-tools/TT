#include "MessagePackage.h"

MessagePackage::MessagePackage(CMutex *online_list_mutex) {
	this->online_list_mutex = online_list_mutex;
}

void MessagePackage::make_login_success(uint8_t *buf) {
	memset(buf, 0, BUFLENGTH);
	uint32_t buf_length = 4 + 2 + 5;
	uint16_t mode = LOGIN;
	memcpy(buf+6, "login", 5);
}
void MessagePackage::make_login_failed(uint8_t *buf) {
	memset(buf, 0, BUFLENGTH);
	uint32_t buf_length = 4 + 2 + 6;
	uint16_t mode = LOGIN;
	memcpy(buf+6, "failed", 5);

}
void MessagePackage::make_heart(uint8_t *buf) {
	memset(buf, 0, BUFLENGTH);
	uint32_t buf_length = 4 + 2 + 5;
	uint16_t mode = HEART;
	memcpy(buf+6, "heart", 5);
}
void MessagePackage::make_update(uint8_t *buf, std::list<std::pair<std::string, int> > &online_list) {
	memset(buf, 0, BUFLENGTH);
	uint32_t buf_length = 4 + 2 + 2;
	uint16_t mode = UPDATE; 	
	uint16_t online_num = online_list.size();
	memcpy(buf+4, &mode, 2);
	memcpy(buf+6, &online_num, 2);
	int index = 0;
	online_list_mutex->Lock();			// 线程锁，上锁
	for (std::list<std::pair<std::string, int> >::iterator it = online_list.begin(); it != online_list.end(); it++, index++) {
		//std::cout << index+1 << ':' << it->first << ' ';
		printf("%d:%s:%d ", index+1, it->first.c_str(), buf_length);
		memcpy(buf+buf_length, (uint8_t*)(it->first.c_str()), 20);
		buf_length +=  20;
	}
	//std::cout << std::endl;
	printf("\n");
	online_list_mutex->Unlock();		// 线程锁，解锁
	for (int i = 0; i < index; i++) {
		uint8_t name[20] = {0};
		memcpy(name, buf+8+(i*20), 20);
		//printf("%d:%s ", i+1, (char*)name);
	}
	//printf("\n");
	memcpy(buf, &buf_length, 4);
}
void MessagePackage::make_new_msg(std::string name, uint8_t *buf, uint8_t *msg) {
	memset(buf, 0, BUFLENGTH);
	uint32_t buf_length = 4 + 2 + 20 +strlen((char*)msg);
	uint16_t mode = COMMUNI;
	memcpy(buf+4, &mode, 2);
	memcpy(buf+6, (uint8_t*)name.c_str(), 20);
	memcpy(buf+26, msg, strlen((char*)msg));
}

