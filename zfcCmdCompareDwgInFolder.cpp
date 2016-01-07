#include "stdafx.h"
#include "resource.h"
#include "zfcTypeDef.h"
#include "zfcCmdCompareDwgInFolder.h"
#include "zfcUtility.h"
#include "zfcLogger.h"
#include "zfcComparetor.h"
#include "zfcDlgSelectFolder.h"

//	���}�ʊi�[�t�H���_
CString zfcCmdCompareDwgInFolder::m_strFolderOldDwg;

//	�V�}�ʊi�[�t�H���_
CString zfcCmdCompareDwgInFolder::m_strFolderNewDwg;

//	�o�̓t�H���_
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

	//	�e��t�H���_��I������
	if( !selectFolder() )
		return true;

	CString strLogPath = zfcUtility::filePath( folderOutput(), zfcUtility::logFileName() );

	//	��r�I�u�W�F�N�g������
	zfcComparetor::instance().init( folderOutput() );

	{
		CWaitCursor wait;
		VERIFY( zfcLogger::instance().open( strLogPath ) );

		zfcUtility::writeLog1( IDS_OLD_DWG_FOLDER, folderOldDwg() );
		zfcUtility::writeLog1( IDS_NEW_DWG_FOLDER, folderNewDwg() );

		//	�t�H���_���̃t�@�C�����擾
		getDwgInFoder( conPathOld, folderOldDwg() );
		getDwgInFoder( conPathNew, folderNewDwg() );

		// ��r���s
		zfc::for_each( conPathNew, [=](zfc::pathContainer::const_reference pairNew){ compare( pairNew, conPathOld ); } );

		// ���}�ʃt�H���_�ɂ����Ȃ��t�@�C���������O�o��
		writeLogOnlyExistInOldDwgFolder( conPathOld );

		// �V�}�ʃt�H���_�ɂ����Ȃ��t�@�C���������O�o��
		writeLogOnlyExistInNewDwgFolder();
	}

	//	��r���ʂ����O�o��
	writeLogResult();
	zfcLogger::instance().close();

	AfxMessageBox( IDS_COMPLETE_COMPARE_DWG );
	
	//	���O�t�@�C����\��
	::ShellExecute( AfxGetMainWnd()->GetSafeHwnd(), _T("open"), strLogPath, NULL, folderOutput(), SW_SHOWNORMAL );
	
	return true;
}

//	�e��t�H���_��I������
bool zfcCmdCompareDwgInFolder::selectFolder()
{
	zfcDlgSelectFolder dlg;
	bool bSelect = false;

	dlg.setFolderOldDwg( folderOldDwg() );
	dlg.setFolderNewDwg( folderNewDwg() );
	dlg.setFolderCompoundDwg( folderOutput() );

	//	�t�H���_�I���_�C�A���O
	if( dlg.DoModal() == IDOK ){
		setFolderOldDwg( dlg.folderOldDwg() );
		setFolderNewDwg( dlg.folderNewDwg() );
		setfolderOutput( dlg.folderCompoundDwg() );
		bSelect = true;
	}

	return bSelect;
}

//	�t�H���_���̐}�ʃt�@�C�����擾
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

//	�t�@�C���p�X�����o
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

//	�}�ʂ��r����
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

// ���}�ʃt�H���_�ɂ����Ȃ��t�@�C���������O�o��
void zfcCmdCompareDwgInFolder::writeLogOnlyExistInOldDwgFolder(zfc::pathContainer& conPathOld) const
{
	//	���}�ʃt�H���_�ɑ��݂����t�@�C���R���e�i����A�����ς̃t�@�C��������
	zfc::for_each( m_conProcessed, [&](zfc::pathContainer::const_reference pair){conPathOld.erase(pair.first);} );

	if( !conPathOld.empty() ){
		zfcUtility::writeLog1( IDS_ONLY_EXIST_OLD_DWG_FOLDER, _T("") );
		//	���}�ʃt�H���_�݂̂ɑ��݂����t�@�C�����o��
		zfcUtility::writeFileName( conPathOld );
	}
}

// �V�}�ʃt�H���_�ɂ����Ȃ��t�@�C���������O�o��
void zfcCmdCompareDwgInFolder::writeLogOnlyExistInNewDwgFolder() const
{
	if( !m_conUnProcessed.empty() ){
		zfcUtility::writeLog1( IDS_ONLY_EXIST_NEW_DWG_FOLDER, _T("") );
		zfcUtility::writeFileName( m_conUnProcessed );
	}
}

// ��r���ʂ����O�o�͂���
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
