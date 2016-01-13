#if !defined(AFX_JHDLGCHKRESULT_H__3E0DAFE8_8092_4691_B44A_242C1B291743__INCLUDED_)
#define AFX_JHDLGCHKRESULT_H__3E0DAFE8_8092_4691_B44A_242C1B291743__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// jhDlgChkResult.h : �w�b�_�[ �t�@�C��
//

#include "jhListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// jhDlgChkResult �_�C�A���O

class jhDlgChkResult : public CAcUiDialog
{
	friend class jhListCtrl;
// �R���X�g���N�V����
public:
	jhDlgChkResult(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(jhDlgChkResult)
	enum { IDD = IDD_DLG_CHK_RESULT };
	jhListCtrl	m_lvChkResult;
	//}}AFX_DATA

	//	������
	BOOL Initialize();

	//	�_�C�A���O��\������
	BOOL Show();

	static jhDlgChkResult& Instance();

	//	�_�C�A���O�\���t���O��Ԃ�
	BOOL GetShow() const;

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(jhDlgChkResult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	virtual void OnDblclkListCtrl(NMHDR* pNMHDR, LRESULT* pResult);

private:
	//	���X�g�r���[�Ƀf�[�^���Z�b�g
	Jhc::Result SetDataToListView();

	//	�`�F�b�N���e������W���擾����
	BOOL GetPointsFromContents( AcGePoint2dArray& points, CString& strContents ) const;

	//	�`�F�b�N���e������W���擾����
	Jhc::Result GetEntityGeom( AcGePoint2d& pntCenter, double& dW, double& dH, LPCTSTR lpszHandle ) const;

	//	�Y�[������
	void Zoom( const AcGePoint2dArray& points, double dW = 20.0, double dH = 20.0 );

	//	�v�f���n�C���C�g����
	Jhc::Result Highlight( LPCTSTR lpszHandle );

	//	�v�f���n�C���C�g����
	Jhc::Result Highlight( CString& strHandles );
	
	//	�I������
	void EndProc();

	//	�_�C�A���O�\���t���O���Z�b�g����
	void SetShow( BOOL value );

	//	�񕝂�Ԃ�
	long GetColWidth( long lCol ) const;

	AcDbObjectId GetEntIdOfHighlight() const;
	void SetEntIdOfHighlight( const AcDbObjectId& value );

	//	�n�C���C�g����������
	Jhc::Result Unhighlight();

	//	�n�C���C�g����������
	Jhc::Result Unhighlight( const AcDbObjectId& objId ) const;

	//	�}�`�n���h�����L�[�ɃG���e�B�e�C���I�[�v��
	Jhc::Result OpenEntity( AcDbVoidPtrArray& entArray, LPCTSTR lpszHandle ) const;

	//	�}�ʓ��`�F�b�N���̐}�ʃt�@�C���p�X
	void SetFilePathInChkCurDwg( LPCTSTR value );
	LPCTSTR GetFilePathInChkCurDwg() const;

private:

	//	�\���t���O
	BOOL m_bShow;

	//	�_�C�A���O�V���O���g���I�u�W�F�N�g
	static jhDlgChkResult m_dlg;

	//	���X�g�r���[��
	std::vector<long> m_vecColWidth;

	//	�E�B���h�E�ʒu
	RECT m_rectWnd;

	//	�n�C���C�g�v�f��ID
	//AcDbObjectId		m_entIdOfHighlight;
	AcDbObjectIdArray	m_highlightIdArray;

	//	�}�ʓ��`�F�b�N���̐}�ʃt�@�C���p�X
	TCHAR m_szFilePathInChkCurDwg[MAX_PATH];

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(jhDlgChkResult)
	afx_msg LONG onAcadKeepFocus( UINT, LONG );
	afx_msg void OnClose();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//	�}�ʓ��`�F�b�N���̐}�ʃt�@�C���p�X
inline void jhDlgChkResult::SetFilePathInChkCurDwg( 
	LPCTSTR value	//(i)
	)
{
	memset( m_szFilePathInChkCurDwg, 0, sizeof(m_szFilePathInChkCurDwg) );
	//--2008/1/30 Unicode �Ή�
	//_tcsncpy( m_szFilePathInChkCurDwg, value, sizeof(m_szFilePathInChkCurDwg)-1);
	_tcsncpy( m_szFilePathInChkCurDwg, value, sizeof(m_szFilePathInChkCurDwg)/sizeof(TCHAR)-1);

}

inline LPCTSTR jhDlgChkResult::GetFilePathInChkCurDwg() const
{
	return ( m_szFilePathInChkCurDwg );
}

//	�_�C�A���O�\���t���O��Ԃ�
inline BOOL jhDlgChkResult::GetShow() const
{
	return ( m_bShow );
}

//	�_�C�A���O�\���t���O���Z�b�g����
inline void jhDlgChkResult::SetShow( BOOL value )
{
	m_bShow = value;
}


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_JHDLGCHKRESULT_H__3E0DAFE8_8092_4691_B44A_242C1B291743__INCLUDED_)
