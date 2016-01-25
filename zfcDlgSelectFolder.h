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

	///////////////////////////////////////////////////////
	//
	//	Get/Set

	const CString& folderOldDwg() const;
	void setFolderOldDwg(const CString& value);

	const CString& folderNewDwg() const;
	void setFolderNewDwg(const CString& value);

	const CString& folderCompoundDwg() const;
	void setFolderCompoundDwg(const CString& value);

	BOOL compareSubFolder() const;
	void setCompareSubFolder(BOOL value);

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_SEL_FOLDER };

private:
	//	�t�H���_�̑��݃`�F�b�N
	BOOL checkFolder( int nEditCtrlID ) const;

	//	�t�H���_��I������
	BOOL selectFolder( CString& strSelFolder, const CString& strIniFolder, int nTitleID);

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

	// �T�u�t�H���_��r�t���O
	BOOL m_bCompareSubFolder;

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

inline void zfcDlgSelectFolder::setFolderOldDwg(const CString& value)
{
	m_strFolderOldDwg = value;
}

inline const CString& zfcDlgSelectFolder::folderNewDwg() const
{
	return m_strFolderNewDwg;
}

inline void zfcDlgSelectFolder::setFolderNewDwg(const CString& value)
{
	m_strFolderNewDwg = value;
}

inline const CString& zfcDlgSelectFolder::folderCompoundDwg() const
{
	return m_strFolderCompoundDwg;
}

inline void zfcDlgSelectFolder::setFolderCompoundDwg(const CString& value)
{
	m_strFolderCompoundDwg = value;
}

inline BOOL zfcDlgSelectFolder::compareSubFolder() const
{
	return m_bCompareSubFolder;
}

inline void zfcDlgSelectFolder::setCompareSubFolder(BOOL value)
{
	m_bCompareSubFolder = value;
}
