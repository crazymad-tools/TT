// Login.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatRoom.h"
#include "Login.h"
#include "afxdialogex.h"
#include "ChatRoomDlg.h"
#include "Conversation.h"
#include "def.h"

// Login 对话框

IMPLEMENT_DYNAMIC(Login, CDialogEx)

Login::Login(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	m_FrameBL = new FrameBL(1);
	// 打开控制台
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}	

Login::~Login()
{
	delete m_FrameBL;
	FreeConsole();
}

void Login::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, account_edit);
	DDX_Control(pDX, IDC_EDIT2, passwd_edit);
}


BEGIN_MESSAGE_MAP(Login, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Login::OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL2, &Login::OnBnClickedCancel2)
END_MESSAGE_MAP()


// Login 消息处理程序

BOOL Login::PreTranslateMessage(MSG* pMsg)
{
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

void Login::OnBnClickedButton1()
{
	CString  account;
	CString passwd;
	account_edit.GetWindowText(account);
	passwd_edit.GetWindowText(passwd);
	m_FrameBL->getSocket()->m_MessagePackage.set_account_passwd((uint8_t*)account.GetBuffer(), (uint8_t*)passwd.GetBuffer());
	uint8_t buf[256] = { 0 };
	m_FrameBL->getSocket()->m_MessagePackage.make_login_package(buf);
	send(m_FrameBL->getSocket()->fd, (char*)buf, BUFLENGTH, 0);
	memset(buf, 0, sizeof(buf));
	recv(m_FrameBL->getSocket()->fd, (char*)buf, BUFLENGTH, 0);
	if (strcmp((char*)buf+6, "login")) {
		return;
	}

	m_FrameBL->open_thread();
	this->ShowWindow(SW_HIDE);
	CChatRoomDlg  dlg(account, m_FrameBL);
	dlg.DoModal();
	SendMessage(WM_CLOSE);
	// TODO: 在此添加控件通知处理程序代码
}


void Login::OnBnClickedCancel2()
{
	system("start explorer http://www.crazymad.cn/tt_register.php");
	// TODO: 在此添加控件通知处理程序代码
}
