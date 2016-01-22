#pragma once


/**
 * 図面比較実行ダイアログ
 * 
 */
class zfcDlgSelectFolder : public CDialog
{
	DECLARE_DYNAMIC(zfcDlgSelectFolder)

public:
	zfcDlgSelectFolder(CWnd* pParent = NULL);   // 標準コンストラクター
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

// ダイアログ データ
	enum { IDD = IDD_DLG_SEL_FOLDER };

private:
	//	フォルダの存在チェック
	BOOL checkFolder( int nEditCtrlID ) const;

	//	フォルダを選択する
	BOOL selectFolder( CString& strSelFolder, const CString& strIniFolder, int nTitleID);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

private:
	//	旧図面フォルダ
	CString m_strFolderOldDwg;

	//	新図面フォルダ
	CString m_strFolderNewDwg;

	//	合成図面フォルダ
	CString m_strFolderCompoundDwg;

	// サブフォルダ比較フラグ
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
