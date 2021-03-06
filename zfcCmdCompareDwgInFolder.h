#pragma once
#include "zfccommand.h"

class zfcCmdCompareDwgInFolder : public zfcCommand
{
public:
	zfcCmdCompareDwgInFolder(void);
	virtual ~zfcCmdCompareDwgInFolder(void);

	//	図面比較を実行する
	virtual bool execute();

	/////////////////////////////////////////////////
	//
	//	Get/Set

	void setFolderOldDwg(const CString& value );
	const CString& folderOldDwg() const;

	void setFolderNewDwg( const CString& value );
	const CString& folderNewDwg() const;

	void setfolderOutput(const CString& value);
	const CString& folderOutput() const;

	BOOL doesCompareSubFolder() const;
	void setCompareSubFolder(BOOL value);

protected:
	//	各種フォルダを選択する
	bool selectFolder();

	//	フォルダ下の図面ファイルを取得
	void getDwgInFolder( zfc::pathContainer& conPath, const CString& strFolder, const CString& strParentFolder ) const;

	//	ファイルパスを検出
	bool findPath( CString& strTitle, CString& strPath, const CString& strFind, const zfc::pathContainer& conPath ) const;

	//	図面を比較する
	void compare( zfc::pathContainer::const_reference pairNew, const zfc::pathContainer& conPathOld );

	//	処理済み図面を追加
	void addProcessed( const CString& strTitle, const CString& strPath );
	
	//	未処理図面を追加
	void addUnProcessed( const CString& strTitle, const CString& strPath );

	//	処理済みフォルダを追加
	void addProcessedFolder( const CString& strKey, const CString& strValue );

	//	未処理フォルダを追加
	void addUnProcessedFolder( const CString& strKey, const CString& strValue );

	// 旧図面フォルダにしかないファイル情報をログ出力
	void writeLogOnlyExistInOldDwgFolder(zfc::pathContainer& conPathOld) const;

	// 新図面フォルダにしかないファイル情報をログ出力
	void writeLogOnlyExistInNewDwgFolder() const;

	// 比較結果をログ出力する
	void writeLogResult() const;

	// 比較を実行する
	void compare(const CString& strFolderOldDwg, const CString& strFolderNewDwg);

	// サブフォルダの比較を実行する
	void compareSubFolder();

	//	サブフォルダを取得
	void getSubFolder( zfc::pathContainer& conPath, const CString& strFolder, const CString& strParentFolder ) const;

	//	サブフォルダを比較する
	void compareSubFolder( zfc::pathContainer::const_reference pairFolderNew, const zfc::pathContainer& conFolderOld );

	// 旧図面フォルダにしかないサブフォルダ情報をログ出力
	void writeLogOnlyExistSubFolderInOld(zfc::pathContainer& conSubFolderOld) const;

	// 新図面フォルダにしかないサブフォルダ情報をログ出力
	void writeLogOnlyExistSubFolderInNew() const;

private:
	//	旧図面格納フォルダ
	static CString m_strFolderOldDwg;

	//	新図面格納フォルダ
	static CString m_strFolderNewDwg;

	//	出力フォルダ
	static CString m_strFolderOutput;

	// サブフォルダ比較フラグ
	static BOOL m_bCompareSubFolder;

	//	処理済ファイル
	zfc::pathContainer m_conProcessed;

	//	未処理ファイル
	zfc::pathContainer m_conUnProcessed;

	//	処理済フォルダ(key=相対フォルダパス, value=フォルダパス)
	zfc::pathContainer m_conProcessedFolder;

	//	未処理フォルダ(key=相対フォルダパス, value=フォルダパス)
	zfc::pathContainer m_conUnProcessedFolder;

	//	旧図面ファイルパス
	zfc::pathContainer m_conPathOld;
	
	//	新図面ファイルパス
	//zfc::pathContainer m_conPathNew;
};

inline void zfcCmdCompareDwgInFolder::setFolderOldDwg(const CString& value )
{
	m_strFolderOldDwg = value;
}

inline const CString& zfcCmdCompareDwgInFolder::folderOldDwg() const
{
	return m_strFolderOldDwg;
}

inline void zfcCmdCompareDwgInFolder::setFolderNewDwg( const CString& value )
{
	m_strFolderNewDwg = value;
}

inline const CString& zfcCmdCompareDwgInFolder::folderNewDwg() const
{
	return m_strFolderNewDwg;
}

inline void zfcCmdCompareDwgInFolder::setfolderOutput(const CString& value)
{
	m_strFolderOutput = value;
}

inline const CString& zfcCmdCompareDwgInFolder::folderOutput() const
{
	return m_strFolderOutput;
}

inline void zfcCmdCompareDwgInFolder::addProcessed( const CString& strTitle, const CString& strPath )
{
	auto itp = m_conProcessed.insert( zfc::pathContainer::value_type( strTitle, strPath ) );
	assert( itp.second );
}

inline void zfcCmdCompareDwgInFolder::addUnProcessed( const CString& strTitle, const CString& strPath )
{
	auto itp = m_conUnProcessed.insert( zfc::pathContainer::value_type( strTitle, strPath ) );
	assert( itp.second );
}

//	処理済みフォルダを追加
inline void zfcCmdCompareDwgInFolder::addProcessedFolder( const CString& strKey, const CString& strValue )
{
	auto itp = m_conProcessedFolder.insert( zfc::pathContainer::value_type( strKey, strValue ) );
	assert( itp.second );
}

//	未処理フォルダを追加
inline void zfcCmdCompareDwgInFolder::addUnProcessedFolder( const CString& strKey, const CString& strValue )
{
	auto itp = m_conUnProcessedFolder.insert( zfc::pathContainer::value_type( strKey, strValue ) );
	assert( itp.second );
}

inline BOOL zfcCmdCompareDwgInFolder::doesCompareSubFolder() const
{
	return m_bCompareSubFolder;
}

inline void zfcCmdCompareDwgInFolder::setCompareSubFolder(BOOL value)
{
	m_bCompareSubFolder = value;
}


