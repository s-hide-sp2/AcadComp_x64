// DialogSetting.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "StdAfx.h"
#include "resource.h"
#include "DialogSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DialogSetting �_�C�A���O


DialogSetting::DialogSetting(CWnd* pParent /*=NULL*/)
	: CDialog(DialogSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(DialogSetting)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void DialogSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DialogSetting)
	DDX_Control(pDX, IDC_EDIT_TOL, m_edtTol);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DialogSetting, CDialog)
	//{{AFX_MSG_MAP(DialogSetting)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DialogSetting ���b�Z�[�W �n���h��

void DialogSetting::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	
	CDialog::OnCancel();
}

void DialogSetting::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	CString strInput;
	double  dRet;

	m_edtTol.GetWindowText(strInput);

	if (!ACString::StringToNumber(strInput, dRet)) {
		CString strTitle;
		this->GetWindowText(strTitle);
		MessageBox(_T("���͂������l�͖����ł��B"), strTitle, MB_OK | MB_ICONWARNING);//2006.07.20
		return;
	}	

	ACProfileManager pMng;
	
	// ���e�덷���v���t�@�C���ɏ������ށB
	pMng.SetUserTol(dRet);
	ACCompBase::SetTol(dRet);

	CDialog::OnOK();
}

BOOL DialogSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������	
	ACProfileManager pMng;
	CString strTol;
	double dTol;

	dTol = pMng.GetUserTol();
	
	strTol = ACString::NumberToString(dTol);
	
	// ��ʂ̓��̓{�b�N�X�Ɣ�r�N���X�ɁA�擾�������e�덷���Z�b�g����B
	m_edtTol.SetWindowText(strTol);
	//ACCompBase::SetTol(dTol);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}
