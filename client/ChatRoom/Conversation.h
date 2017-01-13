#pragma once
#include "afxwin.h"
#include "FrameBL.h"
#include <list>

// Conversation 对话框

static HANDLE thread_exit = CreateEvent(NULL, TRUE, FALSE, NULL);	// 原子量，用于控制线程结束
static HANDLE handle_update_dlg;

struct hread_argument {
	CEdit *msg_edit;
	InfoList *user_info;
};


class Conversation : public CDialogEx
{
	DECLARE_DYNAMIC(Conversation)

public:
	Conversation(FrameBL *m_FrameBL, CString dir_account, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Conversation();
	BOOL OnInitDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit edit_text;
	afx_msg void OnBnClickedButton1();
	BOOL PreTranslateMessage(MSG* pMsg);

private:
	CString dir_account;
	FrameBL *m_FrameBL;
	std::vector<std::string> *msg_list;
	hread_argument m_thread_argument;

public:
	CStatic text_dir_account;
	CEdit msg_edit;
};


static DWORD WINAPI thread_update_dlg(PVOID pM) {
	hread_argument *argument = (hread_argument*)pM;
	if (!argument->user_info->msg_list.empty()) {
		std::vector<std::string>::iterator it = argument->user_info->msg_list.begin();
		argument->msg_edit->SetWindowText(_T(it->c_str()));
		printf("msg_list: %s\n", it->c_str());
		for (it++; it != argument->user_info->msg_list.end(); it++) {
			argument->msg_edit->SetSel(argument->msg_edit->GetWindowTextLength(), argument->msg_edit->GetWindowTextLength());
			argument->msg_edit->ReplaceSel(_T(std::string("\r\n" + *it).c_str()));
			printf("\t%s\n", it->c_str());
		}
		//argument->user_info->new_num = 0;
	}
	int message_num = argument->user_info->new_num;
	while (WaitForSingleObject(thread_exit, 0) != WAIT_OBJECT_0) {
		if (message_num != argument->user_info->new_num) {
			int new_num = argument->user_info->new_num;
			for (int i = message_num; i < new_num; i++) {
				argument->msg_edit->SetSel(argument->msg_edit->GetWindowTextLength(), argument->msg_edit->GetWindowTextLength());
				argument->msg_edit->ReplaceSel(_T(std::string("\r\n" + argument->user_info->msg_list[i]).c_str()));
				//printf("\t%s\n", ->c_str());
			}
			message_num = new_num;
		}
	}
	return EXIT_SUCCESS;
}