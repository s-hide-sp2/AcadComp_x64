// zfcDlgSelectFolder.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "resource.h"
#include "zfcDlgSelectFolder.h"
#include "afxdialogex.h"


// zfcDlgSelectFolder �_�C�A���O

IMPLEMENT_DYNAMIC(zfcDlgSelectFolder, CDialog)

zfcDlgSelectFolder::zfcDlgSelectFolder(CWnd* pParent /*=NULL*/)
	: CDialog(zfcDlgSelectFolder::IDD, pParent)
	, m_strFolderOldDwg(_T(""))
	, m_strFolderNewDwg(_T(""))
	, m_strFolderCompoundDwg(_T(""))
{

}

zfcDlgSelectFolder::~zfcDlgSelectFolder()
{
}

//	�t�H���_�̑��݃`�F�b�N
BOOL zfcDlgSelectFolder::checkFolder( int nEditCtrlID ) const
{
	BOOL bResult = TRUE;
	CEdit* pEdt = (CEdit*)GetDlgItem(nEditCtrlID);	// dynamic_cast���Ɖ��̂�null�ɂȂ�
	assert( pEdt != nullptr );
	CString strPath;
	CFileStatus fileStatus;

	pEdt->GetWindowText( strPath );
	bResult = CFile::GetStatus( strPath, fileStatus );
	
	if( bResult )
		bResult = ( CFile::Attribute::directory & fileStatus.m_attribute );

	if( !bResult ){
		AfxMessageBox( IDS_INVALID_FOLDER, MB_OK | MB_ICONEXCLAMATION );
		pEdt->SetFocus();
		pEdt->SetSel(0,-1);
	}

	return bResult;
}

//	�t�H���_��I������
BOOL zfcDlgSelectFolder::selectFolder( CString& strSelFolder, const CString& strIniFolder, int nTitleID )
{
	CString strTitle;
	AfxFormatString1( strTitle, nTitleID, _T("") );
	CWinAppEx* pApp = (CWinAppEx*)acedGetAcadWinApp();
	BOOL bSelect = pApp->GetShellManager()->BrowseForFolder(strSelFolder, this, strIniFolder, strTitle );

	return bSelect;
}

void zfcDlgSelectFolder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_FOLDER_OLD_DWG, m_strFolderOldDwg);
	DDX_Text(pDX, IDC_EDT_FOLDER_NEW_DWG, m_strFolderNewDwg);
	DDX_Text(pDX, IDC_EDT_FOLDER_COMPOUND_DWG, m_strFolderCompoundDwg);
}


BEGIN_MESSAGE_MAP(zfcDlgSelectFolder, CDialog)
	ON_BN_CLICKED(IDC_BTN_FOLDER_OLD_DWG, &zfcDlgSelectFolder::OnBnClickedBtnFolderOldDwg)
	ON_BN_CLICKED(IDC_BTN_FOLDER_NEW_DWG, &zfcDlgSelectFolder::OnBnClickedBtnFolderNewDwg)
	ON_BN_CLICKED(IDC_BTN_FOLDER_COMPOUND_DWG, &zfcDlgSelectFolder::OnBnClickedBtnFolderCompoundDwg)
	ON_BN_CLICKED(IDEXECUTE, &zfcDlgSelectFolder::OnBnClickedExecute)
END_MESSAGE_MAP()


// zfcDlgSelectFolder ���b�Z�[�W �n���h���[

//	���}�ʃt�H���_�I���{�^���N���b�N
void zfcDlgSelectFolder::OnBnClickedBtnFolderOldDwg()
{
	if( !UpdateData(TRUE) )
		return;

	if( selectFolder(m_strFolderOldDwg, m_strFolderOldDwg, IDS_SELECT_OLD_DWG_FOLDER) ){
		UpdateData(FALSE);
	}
}

//	�V�}�ʃt�H���_�I���{�^���N���b�N
void zfcDlgSelectFolder::OnBnClickedBtnFolderNewDwg()
{
	if( !UpdateData(TRUE) )
		return;

	if( selectFolder(m_strFolderNewDwg, m_strFolderNewDwg, IDS_SELECT_NEW_DWG_FOLDER) ){
		UpdateData(FALSE);
	}
}

//	�����}�ʃt�H���_�I���{�^���N���b�N
void zfcDlgSelectFolder::OnBnClickedBtnFolderCompoundDwg()
{
	if( !UpdateData(TRUE) )
		return;

	if( selectFolder(m_strFolderCompoundDwg, m_strFolderCompoundDwg, IDS_SELECT_COMPOUND_DWG_FOLDER) ){
		UpdateData(FALSE);
	}
}

//	���s�{�^���N���b�N
void zfcDlgSelectFolder::OnBnClickedExecute()
{
	int nEdtCtrIds[] = { IDC_EDT_FOLDER_OLD_DWG, IDC_EDT_FOLDER_NEW_DWG, IDC_EDT_FOLDER_COMPOUND_DWG };

	if( UpdateData(TRUE) ){
		for each( auto id in nEdtCtrIds ){ if( !checkFolder(id) ) return; }

		OnOK();
	}
}
