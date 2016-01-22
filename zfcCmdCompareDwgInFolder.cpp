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

// サブフォルダ比較フラグ
BOOL zfcCmdCompareDwgInFolder::m_bCompareSubFolder = FALSE;

zfcCmdCompareDwgInFolder::zfcCmdCompareDwgInFolder(void)
{
}

zfcCmdCompareDwgInFolder::~zfcCmdCompareDwgInFolder(void)
{
	zfcLogger::instance().close();
}

bool zfcCmdCompareDwgInFolder::execute()
{
	//	各種フォルダを選択する
	if( !selectFolder() )
		return true;

	CWaitCursor wait;
	CString strLogPath = zfcUtility::filePath( folderOutput(), zfcUtility::logFileName() );

	//	比較オブジェクト初期化
	zfcComparetor::instance().init( folderOutput() );
	zfcComparetor::instance().setFolderOldDwg( folderOldDwg() );
	zfcComparetor::instance().setFolderNewDwg( folderNewDwg() );
	
	VERIFY( zfcLogger::instance().open( strLogPath ) );

	zfcUtility::writeLog1( IDS_OLD_DWG_FOLDER, folderOldDwg() );
	zfcUtility::writeLog1( IDS_NEW_DWG_FOLDER, folderNewDwg() );
	zfcLogger::instance().write( _T("\r\n") );

	//	比較実行
	compare( folderOldDwg(), folderNewDwg() );

	//	サブフォルダを比較
	if( doesCompareSubFolder() )
		compareSubFolder();

	// 旧図面フォルダにしかないファイル情報をログ出力
	writeLogOnlyExistInOldDwgFolder( m_conPathOld );

	// 新図面フォルダにしかないファイル情報をログ出力
	writeLogOnlyExistInNewDwgFolder();

	//	比較結果をログ出力
	writeLogResult();
	zfcLogger::instance().close();

	AfxMessageBox( IDS_COMPLETE_COMPARE_DWG );
	
	//	ログファイルを表示
	::ShellExecute( AfxGetMainWnd()->GetSafeHwnd(), _T("open"), strLogPath, NULL, folderOutput(), SW_SHOWNORMAL );
	
	return true;
}

// 比較を実行する
void zfcCmdCompareDwgInFolder::compare(const CString& strFolderOldDwg, const CString& strFolderNewDwg)
{
	zfc::pathContainer conPathOld;
	zfc::pathContainer conPathNew;
	//	フォルダ下のファイル情報取得
	getDwgInFolder( conPathOld, strFolderOldDwg, folderOldDwg() );
	getDwgInFolder( conPathNew, strFolderNewDwg, folderNewDwg() );

	m_conPathOld.insert( conPathOld.begin(), conPathOld.end() );

	// 比較実行
	zfc::for_each( conPathNew, [=](zfc::pathContainer::const_reference pairNew){ compare( pairNew, conPathOld ); } );
}

//	各種フォルダを選択する
bool zfcCmdCompareDwgInFolder::selectFolder()
{
	zfcDlgSelectFolder dlg;
	bool bSelect = false;

	dlg.setFolderOldDwg( folderOldDwg() );
	dlg.setFolderNewDwg( folderNewDwg() );
	dlg.setFolderCompoundDwg( folderOutput() );
	dlg.setCompareSubFolder( doesCompareSubFolder() );

	//	フォルダ選択ダイアログ
	if( dlg.DoModal() == IDOK ){
		setFolderOldDwg( dlg.folderOldDwg() );
		setFolderNewDwg( dlg.folderNewDwg() );
		setfolderOutput( dlg.folderCompoundDwg() );
		setCompareSubFolder( dlg.compareSubFolder() );
		bSelect = true;
	}

	return bSelect;
}

//	フォルダ下の図面ファイルを取得
void zfcCmdCompareDwgInFolder::getDwgInFolder( zfc::pathContainer& conPath, const CString& strFolder, const CString& strParentFolder ) const
{
	CFileFind ff;
	CString strFindFile;
	BOOL bFlg = TRUE;

	strFindFile.Format( _T("%s\\*.dwg"), strFolder );
	bFlg = ff.FindFile( strFindFile );

	while( bFlg ){
		bFlg = ff.FindNextFile();
		if( !ff.IsDots() && !ff.IsDirectory() ){
			auto strRelativeRoot = zfcUtility::relativePath( ff.GetRoot(), strParentFolder );
			CString strKey;

			if( strRelativeRoot.IsEmpty() ){
				strKey = ff.GetFileTitle();
			}
			else{
				strKey.Format( _T("%s\\%s"), strRelativeRoot, ff.GetFileTitle() );
			}

			auto itp = conPath.insert( zfc::pathContainer::value_type( strKey, ff.GetFilePath() ) );
			assert( itp.second );
		}
	}
}

//	ファイルパスを検出
bool zfcCmdCompareDwgInFolder::findPath( CString& strTitle, CString& strPath, const CString& strFind, const zfc::pathContainer& conPath ) const
{
	bool bFind = false;
	auto it = conPath.find( strFind );

	if( conPath.end() == it ){
		auto strFindNearFile = strFind.Left( strFind.GetLength()-1 );

		zfc::for_each_if( conPath, [&](zfc::pathContainer::const_reference pair)->bool{
			const auto& strCurTitle = pair.first;
			const auto& strNearTitle = strCurTitle.Left( strCurTitle.GetLength()-1 );

			if( !strFindNearFile.CompareNoCase(strNearTitle) ){
				bFind = true;
				strTitle = pair.first;
				strPath = pair.second;
			}

			return bFind;
		} );
	}
	else{
		strTitle = it->first;
		strPath = it->second;
		bFind = true;
	}
	
	return bFind;
}

//	図面を比較する
void zfcCmdCompareDwgInFolder::compare( zfc::pathContainer::const_reference pairNew, const zfc::pathContainer& conPathOld )
{
	const CString& strTitleNew = pairNew.first;
	CString strOldTiltle, strOldPath;
	bool bFind = findPath( strOldTiltle, strOldPath, strTitleNew, conPathOld );

	if( bFind ){
		bool result = zfcComparetor::instance().execute( strOldPath, pairNew.second );
		assert( result );
		addProcessed( strOldTiltle, strOldPath );
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

// サブフォルダの比較を実行する
void zfcCmdCompareDwgInFolder::compareSubFolder()
{
	zfc::pathContainer conSubFolderOld;
	zfc::pathContainer conSubFolderNew;

	//	サブフォルダ取得
	getSubFolder( conSubFolderOld, folderOldDwg(), folderOldDwg() );
	getSubFolder( conSubFolderNew, folderNewDwg(), folderNewDwg() );

	//	比較実行
	zfc::for_each( conSubFolderNew, [=](zfc::pathContainer::const_reference pairFolderNew){ 
		compareSubFolder( pairFolderNew, conSubFolderOld );
	} );

	// 旧図面フォルダにしかないサブフォルダ情報をログ出力
	writeLogOnlyExistSubFolderInOld( conSubFolderOld );

	// 新図面フォルダにしかないサブフォルダ情報をログ出力
	writeLogOnlyExistSubFolderInNew();
}

// 旧図面フォルダにしかないサブフォルダ情報をログ出力
void zfcCmdCompareDwgInFolder::writeLogOnlyExistSubFolderInOld(zfc::pathContainer& conSubFolderOld) const
{
	//	旧図面フォルダに存在したファイルコンテナから、処理済のファイルを除去
	zfc::for_each( m_conProcessedFolder, [&](zfc::pathContainer::const_reference pair){conSubFolderOld.erase(pair.first);} );

	if( !conSubFolderOld.empty() ){
		zfcLogger::instance().write( _T("\r\n") );
		zfcUtility::writeLog1( IDS_ONLY_EXIST_SUB_FOLDER_IN_OLD, _T("") );
		//	旧図面フォルダのみに存在したファイルを出力
		zfcUtility::writeFolderPath( conSubFolderOld );
	}
}

// 新図面フォルダにしかないサブフォルダ情報をログ出力
void zfcCmdCompareDwgInFolder::writeLogOnlyExistSubFolderInNew() const
{
	if( !m_conUnProcessedFolder.empty() ){
		zfcLogger::instance().write( _T("\r\n") );
		zfcUtility::writeLog1( IDS_ONLY_EXIST_SUB_FOLDER_IN_NEW, _T("") );
		zfcUtility::writeFolderPath( m_conUnProcessedFolder );
	}
}

//	サブフォルダを比較する
void zfcCmdCompareDwgInFolder::compareSubFolder( zfc::pathContainer::const_reference pairFolderNew, const zfc::pathContainer& conFolderOld )
{
	auto itFolderOld = conFolderOld.find( pairFolderNew.first );

	if( conFolderOld.end() != itFolderOld ){
		const auto& folderOld = itFolderOld->second;
		const auto& folderNew = pairFolderNew.second;
		const auto& strFolderName = itFolderOld->first;
		CString strFolderOutput;

		//	出力フォルダセット
		strFolderOutput.Format( _T("%s\\%s"), (LPCTSTR)folderOutput(), strFolderName );
		zfcComparetor::instance().setFolderOutput( strFolderOutput );
		//VERIFY( zfcUtility::createFolders(strFolderOutput) );
		
		//	比較実行
		compare( folderOld, folderNew );
		addProcessedFolder( itFolderOld->first, itFolderOld->second );
	}
	else{
		addUnProcessedFolder( pairFolderNew.first, pairFolderNew.second );
	}
}

//	サブフォルダを取得
void zfcCmdCompareDwgInFolder::getSubFolder( zfc::pathContainer& conPath, const CString& strFolder, const CString& strParentFolder ) const
{
	CFileFind ff;
	CString strFindFile;
	BOOL bFlg = TRUE;

	strFindFile.Format( _T("%s\\*"), strFolder );
	bFlg = ff.FindFile( strFindFile );

	while( bFlg ){
		bFlg = ff.FindNextFile();
		if( !ff.IsDots() && ff.IsDirectory() ){
			auto strRelativePath = zfcUtility::relativePath(ff.GetFilePath(), strParentFolder );
			auto itp = conPath.insert( zfc::pathContainer::value_type( strRelativePath, ff.GetFilePath() ) );
			assert( itp.second );

			getSubFolder( conPath, ff.GetFilePath(), strParentFolder );
		}
	}
}

