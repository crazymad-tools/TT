#pragma once

#define BUFLENGTH 256

#include <stdint.h>
//typedef __int8 uint8_t;
//typedef __int16 uint16_t;
//typedef __int32 uint32_t;
//typedef __int64 uint64_t;


#define BUFLENGTH 256			// 数据包最大长度
#define LOGIN 0x01				// 登录模式
#define HEART 0x02				// 心跳模式
#define COMMUNI 0x03			// 通信模式
#define BREAK 0x04				// 中断连接
#define LOGIN_RESPONSE 0x05		// 登陆消息
#define UPDATE 0x06				// 更新在线列表
static HANDLE UI_Mutex = CreateMutex(NULL, FALSE, NULL);