// Conversation.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatRoom.h"
#include "Conversation.h"
#include "afxdialogex.h"


// Conversation 对话框

IMPLEMENT_DYNAMIC(Conversation, CDialogEx)

Conversation::Conversation(FrameBL *m_FrameBL, CString dir_account, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent), m_FrameBL(m_FrameBL)
{
	this->dir_account = dir_account;
	std::map<std::string, InfoList>::iterator it = m_FrameBL->getUserInfo()->getUserList()->find(std::string(dir_account.GetBuffer()));
	if (it != m_FrameBL->getUserInfo()->getUserList()->end()) {
		printf("找到组织了:%s\n", it->first.c_str());
		msg_list = &(it->second.msg_list);
		m_thread_argument.user_info = &(it->second);
	}
}

Conversation::~Conversation()
{
	if (WaitForSingleObject(handle_update_dlg, 100) == WAIT_TIMEOUT) {
		TerminateThread(thread_update_dlg, 0);
		TerminateThread(handle_update_dlg, 0);
	}
	CloseHandle(handle_update_dlg);
	m_FrameBL->getUserInfo()->set_statu(true);
	m_FrameBL->getUserInfo()->set_message_statu(true);
}

BOOL Conversation::OnInitDialog() {
	CDialogEx::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	text_dir_account.SetWindowTextA(dir_account);
	/*if (!msg_list->empty()) {
		std::vector<std::string>::iterator it = msg_list->begin();
		edit_text.SetWindowText(_T(it->c_str()));
		printf("msg_list: %s\n", it->c_str());
		for (it++; it != msg_list->end(); it++) {
			edit_text.SetSel(edit_text.GetWindowTextLength(), edit_text.GetWindowTextLength());
			edit_text.ReplaceSel(_T(std::string("\r\n"+*it).c_str()));
			printf("\t%s\n", it->c_str());
		}
	}*/
	m_thread_argument.msg_edit = &edit_text;
	handle_update_dlg = CreateThread(NULL, 0, thread_update_dlg, &m_thread_argument, 0, NULL);

	return TRUE;
}

void Conversation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, edit_text);
	DDX_Control(pDX, USER_ACCOUNT, text_dir_account);
	DDX_Control(pDX, IDC_EDIT1, msg_edit);
}


BEGIN_MESSAGE_MAP(Conversation, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Conversation::OnBnClickedButton1)
END_MESSAGE_MAP()


// Conversation 消息处理程序

// 发送消息
void Conversation::OnBnClickedButton1()
{
	CString msg;
	msg_edit.GetWindowText(msg);
	msg_edit.SetWindowTextA(_T(""));
	char c_msg[BUFLENGTH] = { 0 };
	char account[20] = { 0 };
	strcpy(c_msg, msg.GetBuffer());
	strcpy(account, dir_account.GetBuffer());
	int length = strlen(c_msg);
	uint8_t buf[BUFLENGTH];
	m_FrameBL->getSocket()->m_MessagePackage.make_conmmuni_package(buf, (uint8_t*)account, (uint8_t*)c_msg, length);
	send(m_FrameBL->getSocket()->fd, (char*)buf, BUFLENGTH, 0);
	msg_list->push_back("you: "+std::string(msg.GetBuffer()));
	m_thread_argument.user_info->new_num++;
	//MessageBox(msg);
	// TODO: 在此添加控件通知处理程序代码
}

BOOL Conversation::PreTranslateMessage(MSG* pMsg) {
	/*屏蔽ESC关闭窗口*/
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	//屏蔽回车关闭窗体，但会导致回车在窗体上失效
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN &&pMsg->wParam)
	{
		/*Enter登录*/
		//CheckPsw();
		return TRUE;
	}
	else
	{
		return CDialog::PreTranslateMessage(pMsg);
	}
}
