#pragma once
#include "afxwin.h"

#include "FrameBL.h"
#include "afxcmn.h"

// Login �Ի���

class Login : public CDialogEx
{
	DECLARE_DYNAMIC(Login)

public:
	Login(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Login();
	BOOL PreTranslateMessage(MSG* pMsg);

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	HICON m_hIcon;

	DECLARE_MESSAGE_MAP()
public:
	CEdit account_edit;			// �˺ű༭��
	CEdit passwd_edit;			// ����༭��
	afx_msg void OnBnClickedButton1();

private:
	FrameBL *m_FrameBL;
public:
	afx_msg void OnBnClickedCancel2();
};
