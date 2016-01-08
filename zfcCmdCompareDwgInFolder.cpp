#include "stdafx.h"
#include "resource.h"
#include "zfcTypeDef.h"
#include "zfcCmdCompareDwgInFolder.h"
#include "zfcUtility.h"
#include "zfcLogger.h"
#include "zfcComparetor.h"
#include "zfcDlgSelectFolder.h"

//	旧図面格納フォルダ
CString zfcCmdCompareDwgInFolder::m_strFolderOldDwg;

//	新図面格納フォルダ
CString zfcCmdCompareDwgInFolder::m_strFolderNewDwg;

//	出力フォルダ
CString zfcCmdCompareDwgInFolder::m_strFolderOutput;


zfcCmdCompareDwgInFolder::zfcCmdCompareDwgInFolder(void)
{
}

zfcCmdCompareDwgInFolder::~zfcCmdCompareDwgInFolder(void)
{
	zfcLogger::instance().close();
}

bool zfcCmdCompareDwgInFolder::execute()
{
	zfc::pathContainer conPathOld;
	zfc::pathContainer conPathNew;

	//	各種フォルダを選択する
	if( !selectFolder() )
		return true;

	CString strLogPath = zfcUtility::filePath( folderOutput(), zfcUtility::logFileName() );

	//	比較オブジェクト初期化
	zfcComparetor::instance().init( folderOutput() );

	{
		CWaitCursor wait;
		VERIFY( zfcLogger::instance().open( strLogPath ) );

		zfcUtility::writeLog1( IDS_OLD_DWG_FOLDER, folderOldDwg() );
		zfcUtility::writeLog1( IDS_NEW_DWG_FOLDER, folderNewDwg() );
		zfcLogger::instance().write( _T("\r\n") );

		//	フォルダ下のファイル情報取得
		getDwgInFoder( conPathOld, folderOldDwg() );
		getDwgInFoder( conPathNew, folderNewDwg() );

		// 比較実行
		zfc::for_each( conPathNew, [=](zfc::pathContainer::const_reference pairNew){ compare( pairNew, conPathOld ); } );

		// 旧図面フォルダにしかないファイル情報をログ出力
		writeLogOnlyExistInOldDwgFolder( conPathOld );

		// 新図面フォルダにしかないファイル情報をログ出力
		writeLogOnlyExistInNewDwgFolder();
	}

	//	比較結果をログ出力
	writeLogResult();
	zfcLogger::instance().close();

	AfxMessageBox( IDS_COMPLETE_COMPARE_DWG );
	
	//	ログファイルを表示
	::ShellExecute( AfxGetMainWnd()->GetSafeHwnd(), _T("open"), strLogPath, NULL, folderOutput(), SW_SHOWNORMAL );
	
	return true;
}

//	各種フォルダを選択する
bool zfcCmdCompareDwgInFolder::selectFolder()
{
	zfcDlgSelectFolder dlg;
	bool bSelect = false;

	dlg.setFolderOldDwg( folderOldDwg() );
	dlg.setFolderNewDwg( folderNewDwg() );
	dlg.setFolderCompoundDwg( folderOutput() );

	//	フォルダ選択ダイアログ
	if( dlg.DoModal() == IDOK ){
		setFolderOldDwg( dlg.folderOldDwg() );
		setFolderNewDwg( dlg.folderNewDwg() );
		setfolderOutput( dlg.folderCompoundDwg() );
		bSelect = true;
	}

	return bSelect;
}

//	フォルダ下の図面ファイルを取得
void zfcCmdCompareDwgInFolder::getDwgInFoder( zfc::pathContainer& conPath, const CString& strFolder ) const
{
	CFileFind ff;
	CString strFindFile;
	BOOL bFlg = TRUE;

	strFindFile.Format( _T("%s\\*.dwg"), strFolder );
	bFlg = ff.FindFile( strFindFile );

	while( bFlg ){
		bFlg = ff.FindNextFile();
		if( !ff.IsDots() && !ff.IsDirectory() ){
			auto itp = conPath.insert( zfc::pathContainer::value_type( ff.GetFileTitle(), ff.GetFilePath() ) );
			assert( itp.second );
		}
	}
}

//	ファイルパスを検出
bool zfcCmdCompareDwgInFolder::findPath( zfc::pathContainer::const_iterator& itPath, const CString& strFind, const zfc::pathContainer& conPath ) const
{
	bool bFind = false;
	auto it = conPath.find( strFind );

	if( conPath.end() == it ){
		auto strNearFile = strFind.Left( strFind.GetLength()-1 );

		it = conPath.find( strNearFile );
	}

	if( conPath.end() != it ){
		bFind = true;
		itPath = it;
	}
	
	return bFind;
}

//	図面を比較する
void zfcCmdCompareDwgInFolder::compare( zfc::pathContainer::const_reference pairNew, const zfc::pathContainer& conPathOld )
{
	const CString& strTitleNew = pairNew.first;
	zfc::pathContainer::const_iterator itOld;
	bool bFind = findPath( itOld, strTitleNew, conPathOld );

	if( bFind ){
		bool result = zfcComparetor::instance().execute( itOld->second, pairNew.second );
		assert( result );
		addProcessed( itOld->first, itOld->second );
	}
	else{
		addUnProcessed( pairNew.first, pairNew.second );
	}
}

// 旧図面フォルダにしかないファイル情報をログ出力
void zfcCmdCompareDwgInFolder::writeLogOnlyExistInOldDwgFolder(zfc::pathContainer& conPathOld) const
{
	//	旧図面フォルダに存在したファイルコンテナから、処理済のファイルを除去
	zfc::for_each( m_conProcessed, [&](zfc::pathContainer::const_reference pair){conPathOld.erase(pair.first);} );

	if( !conPathOld.empty() ){
		zfcLogger::instance().write( _T("\r\n") );
		zfcUtility::writeLog1( IDS_ONLY_EXIST_OLD_DWG_FOLDER, _T("") );
		//	旧図面フォルダのみに存在したファイルを出力
		zfcUtility::writeFileName( conPathOld );
	}
}

// 新図面フォルダにしかないファイル情報をログ出力
void zfcCmdCompareDwgInFolder::writeLogOnlyExistInNewDwgFolder() const
{
	if( !m_conUnProcessed.empty() ){
		zfcLogger::instance().write( _T("\r\n") );
		zfcUtility::writeLog1( IDS_ONLY_EXIST_NEW_DWG_FOLDER, _T("") );
		zfcUtility::writeFileName( m_conUnProcessed );
	}
}

// 比較結果をログ出力する
void zfcCmdCompareDwgInFolder::writeLogResult() const
{
	auto& comparetor = zfcComparetor::instance();
	CString strCorrespond;
	CString strDiscord;
	CString strWarning;
	CString strError;
	CString strResult;
	
	AfxFormatString1( strCorrespond, IDS_RESULT_CORRESPOND, std::to_wstring((_Longlong)comparetor.cntCorrespond()).c_str() );
	AfxFormatString1( strDiscord, IDS_RESULT_DISCORD, std::to_wstring((_Longlong)comparetor.cntDiscord()).c_str() );
	AfxFormatString1( strWarning, IDS_RESULT_WARNING, std::to_wstring((_Longlong)comparetor.cntWarning()).c_str() );
	AfxFormatString1( strError, IDS_RESULT_ERROR, std::to_wstring((_Longlong)comparetor.cntError()).c_str() );

	strResult.Format( _T("%s %s %s %s"), strCorrespond, strDiscord, strWarning, strError );

	zfcUtility::writeLog1( IDS_COMPARE_RESULT, strResult );
}
