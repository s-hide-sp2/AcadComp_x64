#if !defined(AFX_DIALOGSETTING_H__C8EDB3C4_7A25_40EC_84AF_B124D0D2D957__INCLUDED_)
#define AFX_DIALOGSETTING_H__C8EDB3C4_7A25_40EC_84AF_B124D0D2D957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSetting.h : �w�b�_�[ �t�@�C��
//

#include "ACString.h"
#include "ACProfileManager.h"
#include "ACCompBase.h"

/////////////////////////////////////////////////////////////////////////////
// DialogSetting �_�C�A���O

class DialogSetting : public CDialog
{
// �R���X�g���N�V����
public:
	DialogSetting(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(DialogSetting)
	enum { IDD = IDD_DLG_SETTING };
	CEdit	m_edtTol;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(DialogSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(DialogSetting)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DIALOGSETTING_H__C8EDB3C4_7A25_40EC_84AF_B124D0D2D957__INCLUDED_)
