#pragma once
#include "afxwin.h"

#include "FrameBL.h"
#include "afxcmn.h"

// Login 对话框

class Login : public CDialogEx
{
	DECLARE_DYNAMIC(Login)

public:
	Login(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Login();
	BOOL PreTranslateMessage(MSG* pMsg);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HICON m_hIcon;

	DECLARE_MESSAGE_MAP()
public:
	CEdit account_edit;			// 账号编辑框
	CEdit passwd_edit;			// 密码编辑框
	afx_msg void OnBnClickedButton1();

private:
	FrameBL *m_FrameBL;
public:
	afx_msg void OnBnClickedCancel2();
};
