#pragma once

#define BUFLENGTH 256

#include <stdint.h>
//typedef __int8 uint8_t;
//typedef __int16 uint16_t;
//typedef __int32 uint32_t;
//typedef __int64 uint64_t;


#define BUFLENGTH 256			// ���ݰ���󳤶�
#define LOGIN 0x01				// ��¼ģʽ
#define HEART 0x02				// ����ģʽ
#define COMMUNI 0x03			// ͨ��ģʽ
#define BREAK 0x04				// �ж�����
#define LOGIN_RESPONSE 0x05		// ��½��Ϣ
#define UPDATE 0x06				// ���������б�
static HANDLE UI_Mutex = CreateMutex(NULL, FALSE, NULL);