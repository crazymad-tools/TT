
// ChatRoomDlg.h : 头文件
//

#pragma once

#include "FrameBL.h"		// BL业务逻辑处理层
#include "MessageTable.h"	// 双向链表类（数据域是socket数据包）
#include "afxcmn.h"

static HANDLE handle_update;			// 更新消息线程

// CChatRoomDlg 对话框
class CChatRoomDlg : public CDialogEx
{
// 构造
public:
	CChatRoomDlg(CString self_name, FrameBL *m_FrameBL, CWnd* pParent = NULL);	// 标准构造函数
	~CChatRoomDlg();					// 析构函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATROOM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	FrameBL *m_FrameBL;

public:
	FrameBL* getFrameBL();
	CString self_name;

public:
	CListCtrl user_list_control;
	CListCtrl m_user_list_control;
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
};


// 更新消息列表
static DWORD WINAPI thread_update(PVOID pM) {
	CChatRoomDlg* ChatRoom = (CChatRoomDlg*)pM;
	FrameBL *PFrameBL = ChatRoom->getFrameBL();
	int last_online_num = 0;
	while (1) {
		if (PFrameBL->getUserInfo()->get_statu()) {
			WaitForSingleObject(UI_Mutex, INFINITE);			// 加上线程锁，防止冲突
			ChatRoom->m_user_list_control.DeleteAllItems();
			std::map<std::string, InfoList>* user_list = PFrameBL->getUserInfo()->getUserList();
			std::map<std::string, InfoList>::iterator it = user_list->begin();
			int now_online_num = user_list->size();
			if (now_online_num - last_online_num > 0) {
				int dis = now_online_num - last_online_num;
				last_online_num = now_online_num;
				for (int i = 0; i < dis; i++) {
					MessageBeep(MB_OK);
				}
			}
			int index = 0;
			for (; it != user_list->end(); it++) {
				CString account = it->first.c_str();	// 账户名称
				CString new_num;						// 新消息的数量
				if (account == ChatRoom->self_name) {
					continue;
				}
				new_num.Format("%d", it->second.new_num);
				//MessageBox(NULL, new_num, new_num, NULL);
				ChatRoom->m_user_list_control.InsertItem(index, account);
				ChatRoom->m_user_list_control.SetItemText(index, 1, new_num);
				printf("%s %d\t", it->first.c_str(), it->second.new_num);
				index++;
			}
			PFrameBL->getUserInfo()->set_statu(false);
			if (PFrameBL->getUserInfo()->get_message_statu()) {
				int index = 0;
				for (; it != user_list->end(); it++) {
					CString account;
					CString new_num;						// 新消息的数量
					if (account == ChatRoom->self_name) {
						continue;
					}
					new_num.Format("%d", it->second.new_num);
					//MessageBox(NULL, new_num, new_num, NULL);
					ChatRoom->m_user_list_control.SetItemText(index, 1, _T("&"));
					printf("%s %d\t", it->first.c_str(), it->second.new_num);
					index++;
				}
			}
			PFrameBL->getUserInfo()->set_message_statu(false);
			ReleaseMutex(UI_Mutex);								// 解除线程锁
		}
		Sleep(1000);
	}
	return TRUE;
}