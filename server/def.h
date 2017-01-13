/*********************************************************
 * Author			: crazymad
 * Last modified	: 2016-12-29 11:51
 * Filename			: def.h
 * Description		: 
 *********************************************************/

#ifndef _DEF_H
#define _DEF_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#define BUFLENGTH 256			// 数据包最大长度是256个字节
#define LOGIN 0X01				// 登录请求
#define HEART 0x02				// 心跳请求
#define COMMUNI	0x03			// 通信请求
#define BREAK 0x04				// 登出请求
#define LOGIN_RESPONSE 0x05		// 登陆回应
#define UPDATE 0x06				// 更新在线列表

#endif

