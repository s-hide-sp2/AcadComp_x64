#pragma once


/**
 * �}�ʔ�r���s�_�C�A���O
 * 
 */
class zfcDlgSelectFolder : public CDialog
{
	DECLARE_DYNAMIC(zfcDlgSelectFolder)

public:
	zfcDlgSelectFolder(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~zfcDlgSelectFolder();

	//	�V���O���g���I�u�W�F�N�g��Ԃ�
	static zfcDlgSelectFolder& instance();

	///////////////////////////////////////////////////////
	//
	//	Get/Set

	const CString& folderOldDwg() const;
	const CString& folderNewDwg() const;
	const CString& folderCompoundDwg() const;

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_SEL_FOLDER };

private:
	//	�t�H���_�̑��݃`�F�b�N
	BOOL checkFolder( int nEditCtrlID ) const;

	//	�t�H���_��I������
	BOOL selectFolder( CString& strSelFolder, const CString& strIniFolder );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

private:
	//	���}�ʃt�H���_
	CString m_strFolderOldDwg;

	//	�V�}�ʃt�H���_
	CString m_strFolderNewDwg;

	//	�����}�ʃt�H���_
	CString m_strFolderCompoundDwg;

public:
	afx_msg void OnBnClickedBtnFolderOldDwg();
	afx_msg void OnBnClickedBtnFolderNewDwg();
	afx_msg void OnBnClickedBtnFolderCompoundDwg();
	afx_msg void OnBnClickedExecute();
};

inline const CString& zfcDlgSelectFolder::folderOldDwg() const
{
	return m_strFolderOldDwg;
}

inline const CString& zfcDlgSelectFolder::folderNewDwg() const
{
	return m_strFolderOldDwg;
}

inline const CString& zfcDlgSelectFolder::folderCompoundDwg() const
{
	return m_strFolderCompoundDwg;
}

