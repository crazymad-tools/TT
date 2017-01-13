
// ChatRoomDlg.h : ͷ�ļ�
//

#pragma once

#include "FrameBL.h"		// BLҵ���߼������
#include "MessageTable.h"	// ˫�������ࣨ��������socket���ݰ���
#include "afxcmn.h"

static HANDLE handle_update;			// ������Ϣ�߳�

// CChatRoomDlg �Ի���
class CChatRoomDlg : public CDialogEx
{
// ����
public:
	CChatRoomDlg(CString self_name, FrameBL *m_FrameBL, CWnd* pParent = NULL);	// ��׼���캯��
	~CChatRoomDlg();					// ��������

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATROOM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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


// ������Ϣ�б�
static DWORD WINAPI thread_update(PVOID pM) {
	CChatRoomDlg* ChatRoom = (CChatRoomDlg*)pM;
	FrameBL *PFrameBL = ChatRoom->getFrameBL();
	int last_online_num = 0;
	while (1) {
		if (PFrameBL->getUserInfo()->get_statu()) {
			WaitForSingleObject(UI_Mutex, INFINITE);			// �����߳�������ֹ��ͻ
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
				CString account = it->first.c_str();	// �˻�����
				CString new_num;						// ����Ϣ������
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
					CString new_num;						// ����Ϣ������
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
			ReleaseMutex(UI_Mutex);								// ����߳���
		}
		Sleep(1000);
	}
	return TRUE;
}