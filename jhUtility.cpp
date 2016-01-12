#include "StdAfx.h"
//#include "StdArx.h"
#include "resource.h"
#include "jhTypeDef.h"
#include "jhUtility.h"
#include "jhRegistry.h"
#include "jhOption.h"
#include "jhDlgMenu.h"


//	複数要素を囲む矩形の左下・右上座標を計算する
bool jhUtility::GetMinMaxPoints( 
	AcGePoint3d& pntMin,				//(o)左下座標
	AcGePoint3d& pntMax,				//(o)右上座標
	const AcDbVoidPtrArray& entArray	//(i)エンティティ配列
	)
{
	bool bResult = false;
	double dMinX = DBL_MAX;
	double dMinY = DBL_MAX;
	double dMinZ = DBL_MAX;
	double dMaxX = -DBL_MAX;
	double dMaxY = -DBL_MAX;
	double dMaxZ = -DBL_MAX;

	for( int n = 0; n < entArray.length(); n++ ){
		AcDbEntity*	pEnt = AcDbEntity::cast((AcRxObject*)entArray[n]);
		AcDbExtents extents;
		AcGePoint3d pntMax, pntMin;

		if( pEnt == NULL )
			continue;

		pEnt->getGeomExtents(extents);

		pntMin = extents.minPoint();
		pntMax = extents.maxPoint();
		
		dMinX = __min(pntMin.x, dMinX);
		dMinY = __min(pntMin.y, dMinY);
		dMinZ = __min(pntMin.z, dMinZ);

		dMaxX = __max(pntMax.x, dMaxX);
		dMaxY = __max(pntMax.y, dMaxY);
		dMaxZ = __max(pntMax.z, dMaxZ);
		bResult = true;
	}
	
	if( bResult ){
		pntMin.set( dMinX, dMinY, dMinZ );
		pntMax.set( dMaxX, dMaxY, dMaxZ );
	}

	return ( bResult );
}

void jhUtility::UpdateView()
{
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();
}

//	図面を開く
Jhc::Result jhUtility::OpenDwg( 
	LPCTSTR lpszDwgPath //(i)図面ファイルパス
	)
{
	Jhc::Result result = Jhc::rOk;
	DWORD dwAttributes = ::GetFileAttributes( lpszDwgPath );

	if( dwAttributes == ACC_FILE_NOT_EXIST || dwAttributes == FILE_ATTRIBUTE_DIRECTORY )
		result = Jhc::rInvalidFilePath;
	else{
/*		AcApDocument* pTargetDoc = NULL;		
		AcApDocumentIterator* pItr = acDocManager->newAcApDocumentIterator();
	
		if( pItr == NULL )
			return ( Jhc::rOutOfMemory );

		//	引数の図面が既にオープン済みか確認する
		for( ; !pItr->done() && pTargetDoc == NULL; pItr->step() ){
			AcApDocument* pDoc = pItr->document();

			if( !_tcsicmp( pDoc->fileName(), lpszDwgPath ) )
				pTargetDoc = pDoc;
		}

		delete pItr;

		//	引数の図面がオープンしてない場合は図面を開く
		//	開いている場合にはその図面をカレント図面にする
		if( pTargetDoc == NULL )
			openSyncDocHelper( (void*)lpszDwgPath );
		else
			acDocManager->setCurDocument( pTargetDoc, AcAp::kNone, true );
			*/
		result = SetCurDocument( lpszDwgPath );
		if( result == Jhc::rCancel ){
			result = Jhc::rOk;
			openSyncDocHelper( (void*)lpszDwgPath );
		}
	}

	return ( result );
}

//	カレントドキュメントセット
Jhc::Result jhUtility::SetCurDocument( 
	LPCTSTR lpszDwgPath //(i)ファイルパス
	)
{
	Jhc::Result result = Jhc::rCancel;
	AcApDocument* pTargetDoc = NULL;		
	AcApDocumentIterator* pItr = acDocManager->newAcApDocumentIterator();

	if( pItr == NULL )
		return ( Jhc::rOutOfMemory );

	//	引数の図面を検出
	for( ; !pItr->done() && pTargetDoc == NULL; pItr->step() ){
		AcApDocument* pDoc = pItr->document();

		if( !_tcsicmp( pDoc->fileName(), lpszDwgPath ) )
			pTargetDoc = pDoc;
	}
	delete pItr;

	//	引数の図面を検出したらカレント図面にする
	if( pTargetDoc != NULL ){
		Acad::ErrorStatus es;

		if( curDoc() == pTargetDoc )
			return ( Jhc::rOk );		

		es = acDocManager->setCurDocument( pTargetDoc, AcAp::kNone, true );
		if( es == Acad::eOk )
			result = Jhc::rOk;
		else
			result = Jhc::rAcadError;
	}
	
	return ( result );
}

void jhUtility::openSyncDocHelper( void *pData )
{
	if( acDocManager->isApplicationContext() ){
		//--2008/1/30 Unicode 対応
		//acDocManager->appContextOpenDocument( (const char *)pData );
		acDocManager->appContextOpenDocument( (LPCTSTR)pData );
	}
}

//	コマンド呼び出しコマンド実行判定
bool jhUtility::DoesExecuteCommandCall( 
	const Jhc::CheckItem& checkItem //(i)チェック項目
	)
{
	bool bResult = false;

	if( DoesCheckByDwg(checkItem) || checkItem.bCheckFileNum )
		bResult = true;

	return ( bResult );
}

//	図面毎チェック判定
bool jhUtility::DoesCheckByDwg( 
	const Jhc::CheckItem& checkItem //(i)チェック項目
	)
{
	bool bResult = false;
	
	if(		checkItem.bCheckFileName	//	ファイル名チェック
		||	checkItem.bCheckClosed		//	閉合チェック
		||	checkItem.bCheckLayer		//	レイヤチェック
		||	checkItem.bCheckFrame		//	図枠チェック
		||	checkItem.bCheckRoadArea	//	道路区域チェック
		||	checkItem.bCheckPlane		//	平面線形チェック			
		||	checkItem.bCheckXRefPath	//	パスチェック
		||	checkItem.bCheckDwgVer	)	//	Dwgバージョン
	bResult = true;

	return ( bResult );
}

//	トークンを取得する
void jhUtility::GetTokens( 
	//--2008/1/30 Unicode 対応
	//std::vector<std::string>& vecToken,	//(o)トークン
	std::vector<atstring>& vecToken,	//(o)トークン
	CString& str,					//(i/o)トークンデータ
	TCHAR chSep						//(i)区切り文字
	)
{
	CString strToken;

	for( int n = 0; !str.IsEmpty(); n++ ){
		strToken = StrTok( str, chSep );
		vecToken.push_back( (LPCTSTR)strToken );
	}
}

//	トークンを取得する
CString jhUtility::StrTok( 
	CString& str,	//(i/o)トークンデータ
	TCHAR chSep		//(i)区切り文字	
	)
{
	CString strToken;
	int nIndex = str.Find( chSep );
	
	if( nIndex == -1 ){
		strToken = str;
		str.Empty();
	}
	else{
		strToken = str.Left(nIndex);
		str = str.Mid( nIndex+1 );
	}

	return ( strToken );
}

//	パスからフォルダ名を取得する
void jhUtility::GetFolderName(
	LPTSTR lpszFolder,	//(o)フォルダ名（大文字）
	LPCTSTR lpszPath	//(i)パス
	)
{
	TCHAR szDrive[_MAX_DRIVE];
    TCHAR szDir[_MAX_DIR];

	_tsplitpath( lpszPath, szDrive, szDir, NULL, NULL );
	_tmakepath( lpszFolder, szDrive, szDir, NULL, NULL );
	//	大文字に変換
	_tcsupr( lpszFolder );
}

//	ポリライン内判定
bool jhUtility::IsIn( 
	const AcDbPolyline* pPoly,	//(i)ポリライン
	const AcGePoint3d& pnt		//(i)判定座標
	)
{
	bool bResult = IsClosed( pPoly );
	
	if( bResult ){
		AcGePoint3dArray vertex;
		int n = 0;
		
		//	ポリライン頂点取得
		vertex.setLogicalLength( pPoly->numVerts() );
		//--2008/1/30 Unicode 対応
		//for( int n = 0; n < pPoly->numVerts(); n++ )
		for( n = 0; n < pPoly->numVerts(); n++ )
			pPoly->getPointAt( n, vertex[n] );
		
		double dGTol = AcGeContext::gTol.equalPoint();

		AcGeContext::gTol.setEqualPoint( JHC_TOL );
		
		//	判定座標-ポリライン頂点間直線とポリラインとの交点計算を行い
		//	交点がポリライン頂点と一致しなければ、ポリライン外と判定する
		for( n = 0; n < vertex.length() && bResult; n++ ){
			AcDbLine line( pnt, vertex[n] );
			AcGePoint3dArray intersects;

			line.intersectWith( pPoly, AcDb::kOnBothOperands, intersects );

			//	ポリライン頂点一致判定
			if( intersects.length() == 1 )
				bResult = vertex.contains( intersects[0] );
			else if( 1 < intersects.length() )
				bResult = false;
		}

		AcGeContext::gTol.setEqualPoint( dGTol );
	}

	return ( bResult );
}

//	矩形サイズ文字を返す
CString jhUtility::GetRectString( 
	double dW,	//(i)幅
	double dH	//(i)高さ
	)
{
	CString str;
	//--2008/1/30 Unicode 対応
#ifdef _UNICODE
	wostringstream ostr;
#else
	ostrstream ostr;
#endif

	ostr.setf( ios::fixed, ios::floatfield );	
	ostr.precision( JHC_LOG_PRECISION );

	//	端点座標文字列取得		
	ostr << _T("(W:") << dW << _T(" H:") << dH << _T(')') << ends;

	//--2008/1/30 Unicode 対応
#ifdef _UNICODE
	str = ostr.str().c_str();
#else
	str = ostr.str();
	ostr.rdbuf()->freeze(0);
#endif

	return ( str );
}

//	矩形サイズを計算する
Jhc::Result jhUtility::CalcRectSize( 
	double& dW,					//(o)幅
	double& dH,					//(o)高さ
	const AcDbPolyline* pPoly	//(i)矩形
	) 
{
	Jhc::Result result = Jhc::rOk;
	Acad::ErrorStatus es = Acad::eOk;
	AcGePoint2dArray vertex;

	//	ポリラインの３頂点を取得
	vertex.setLogicalLength(3);
	for( int n = 0; n < vertex.length() && es == Acad::eOk; n++ )
		es = pPoly->getPointAt( n, vertex[n] );

	//	頂点間距離を計算する
	if( es == Acad::eOk ){
		double dDist[2];

		dDist[0] = vertex[0].distanceTo(vertex[1]);
		dDist[1] = vertex[1].distanceTo(vertex[2]);
		
		dW = __max( dDist[0], dDist[1] );
		dH = __min( dDist[0], dDist[1] );
	}
	else
		result = Jhc::rFail;

	return ( result );
}

//	ファイルパスを取得する
Jhc::Result jhUtility::GetFilePath(
	LPTSTR lpszFilePath,	//(o)ファイルパス
	LPCTSTR lpszFileName	//(i)ファイル名
	)
{
	Jhc::Result result = Jhc::rOk;
	jhRegistry reg;
	TCHAR szInstPath[MAX_PATH];

	//--2008/1/30 Unicode 対応
	//result = reg.ReadData( szInstPath, JHC_REG_DATA_INSTALL_PATH, sizeof(szInstPath) );
	result = reg.ReadData( szInstPath, JHC_REG_DATA_INSTALL_PATH, sizeof(szInstPath)/sizeof(TCHAR) );
	if( result == Jhc::rOk )
		_tmakepath( lpszFilePath, NULL, szInstPath, lpszFileName, NULL );

	return ( result );
}

//	設定ファイルパスを取得する
Jhc::Result jhUtility::GetIniFilePath(
	LPTSTR lpszFilePath		//(o)ファイルパス
	)
{
	return ( GetFilePath( lpszFilePath, JHC_INI_FILE_NAME ) );
}

//	チェック結果ファイルパスを取得する
Jhc::Result jhUtility::GetCheckResultFilePath(
	LPTSTR lpszFilePath		//(o)ファイルパス
	)
{
	Jhc::Result result = Jhc::rOk;

	result = GetFilePath( lpszFilePath, JHC_CHECK_RESULT_FILE_NAME );

/*	if( jhDlgMenu::Instance().GetCheckCurDwg() )
		lpszFilePath[0] = _T('\0');
	else
		result = GetFilePath( lpszFilePath, JHC_CHECK_RESULT_FILE_NAME );
*/
	return ( result );
}

//	チェック結果ファイルパスを取得する(AutoCad表示用)
//##ModelId=4316551103DD
Jhc::Result jhUtility::GetCheckResultFilePath2(LPTSTR lpszFilePath)
{
	Jhc::Result result = Jhc::rOk;

	if( jhOption::Instance().GetCheckCurDwg() )
		result = GetFilePath( lpszFilePath, JHC_CHECK_IN_DWG_RESULT_FILE_NAME );
	else
		result = GetFilePath( lpszFilePath, JHC_CHECK_FOR_DISP_RESULT_FILE_NAME );

	return ( result );
}

//	チェックサムを計算する
UINT jhUtility::CalcCheckSum( 
	LPCTSTR lpszBuff,	//(i)文字列バッファ
	UINT size			//(i)バッファサイズ
	)
{
	UINT unResult = 0;

	for( UINT n = 0; n < size; n++ )
		unResult += lpszBuff[n] * 10;

	return ( unResult );
}

//	エラーメッセージを返す
CString jhUtility::GetErrorMessage( 
	Jhc::Result result //(i)実行結果
	)
{
	CString strMessage;
	UINT resourceId = 0;
	CAcModuleResourceOverride resOverride;
	
	switch( result ){
	case Jhc::rOk:
		resourceId = IDS_OK;	break;
	case Jhc::rFail:
		resourceId = IDS_FAIL;	break;
	case Jhc::rAcadError:
		resourceId = IDS_ACAD_ERROR;	break;
	case Jhc::rOutOfMemory:
		resourceId = IDS_OUT_OF_MEMORY;	break;
	case Jhc::rFailToWriteCheckLayer:
		resourceId = IDS_FAIL_TO_WRITE_LOG;	break;
	case Jhc::rFailToAccessRegistry:
		resourceId = IDS_FAIL_TO_ACCESS_REGISTRY;	break;
	case Jhc::rFailToWriteIni:
		resourceId = IDS_FAIL_TO_WRITE_INI;	break;
	case Jhc::rFailToReadIni:				
		resourceId = IDS_FAIL_TO_READ_INI;	break;
	case Jhc::rIncorrectIni:				
		resourceId = IDS_INCORRECT_INI;		break;
	case Jhc::rExecelError:				
		resourceId = IDS_EXECEL_ERROR;	break;
	case Jhc::rOverUseSpan:				
		resourceId = IDS_OVER_USE_SPAN;	break;
	case Jhc::rOpenedCheckResultExcelFile:				
		resourceId = IDS_OPENED_CHECK_RESULT_EXCEL_FILE;	break;
	case Jhc::rInvalidFilePath:
		resourceId = IDS_INVALID_FILE_PATH;	break;
	};
	
	strMessage.LoadString( resourceId );

	return ( strMessage );
}

//	Iniファイルをチェックする
Jhc::Result jhUtility::CheckIniFile()
{
	Jhc::Result result = Jhc::rOk;
	TCHAR szPath[MAX_PATH];

	result = GetIniFilePath(szPath);
	if( result == Jhc::rOk ){
		//--2008/1/30 Unicode 対応
#ifdef _UNICODE
		wifstream ifs;
#else
		ifstream ifs;
#endif
		
		ifs.open( szPath, ios::in );
				
		if( ifs.is_open() ){
			TCHAR szCheckSum[64];
	//--2008/1/30 Unicode 対応
#ifdef _UNICODE
			wostringstream ostr;
#else
			ostrstream ostr;
#endif
			UINT unCalcCheckSum;
			CString str;
			long lCheckSum;
			//--2008/1/30 Unicode 対応
			//char* pStop = NULL;
			LPTSTR pStop = NULL;

			//	チェックサム読み込み
			ifs.getline( szCheckSum, sizeof(szCheckSum) );
			
			//	Ini内容読み込み
			ifs >> ostr.rdbuf();
			ostr << ends;
	//--2008/1/30 Unicode 対応
#ifdef _UNICODE
			str = ostr.str().c_str();
#else
			str = ostr.str();
			ostr.rdbuf()->freeze(0);
#endif
			
			//	16進　→　10進変換
			lCheckSum = _tcstoul( szCheckSum, &pStop, 16 );
			
			//	Ini内容のチェックサム計算
			unCalcCheckSum = CalcCheckSum( str, str.GetLength() );
			
		//	ostr.rdbuf()->freeze( 0 );
			
			//	Iniに書き込まれているチェックサムとIni内容で計算した値が一致するかを判定
			if( lCheckSum != unCalcCheckSum )
				result = Jhc::rIncorrectIni;

			ifs.close();
		}
		else
			result = Jhc::rFailToReadIni;
	}

	return ( result );
}

//	Iniからデータを取得する
Jhc::Result jhUtility::GetIniData( 
	LPTSTR lpszValue,		//(o)値
	DWORD dwSize ,			//(i)バッファサイズ
	LPCTSTR lpszSecName,	//(i)セクション名
	LPCTSTR lpszKey			//(i)キー
	)
{
	Jhc::Result result = Jhc::rOk;
	TCHAR szIniPath[MAX_PATH];
	
	result = GetIniFilePath( szIniPath );
	if( result == Jhc::rOk ){
		if( ::GetPrivateProfileString(lpszSecName, lpszKey, _T(""), lpszValue, dwSize, szIniPath ) == 0 )
			result = Jhc::rFailToReadIni;		
	}

	return ( result );
}

//	閉ポリライン判定
bool jhUtility::IsClosed( const AcDbPolyline* pPoly )
{
	bool bResult = false;

	if( pPoly->isClosed() )
		bResult = true;
	else{
		AcGePoint3d pntS, pntE;

		pPoly->getStartPoint( pntS );
		pPoly->getEndPoint( pntE );

		if( pntS.isEqualTo( pntE, jhOption::Instance().GetTol() ) )
			bResult = true;
	}

	return ( bResult );
}

//	点検索
bool jhUtility::FindEqualPoint( 
	const AcGePoint3d& pntTarget,	//(i)検出点
	const AcGePoint3dArray points	//(i)点列
	)
{
	bool bFind = false;
	
	for( int n = 0; n < points.length() && !bFind; n++ ){
		if( pntTarget.isEqualTo( points[n], jhOption::Instance().GetTol() ) )
			bFind = true;
	}

	return ( bFind );
}

//	最小距離計算
double jhUtility::CalcMinDistPoint( 
	AcGePoint3d& pntCalc,			//(o)算出座標
	const AcGePoint3d& pnt,			//(i)基準点
	const AcGePoint3dArray& points	//(i)点列
	)
{
	double dMin = DBL_MAX;

	for( int n = 0; n < points.length(); n++ ){
		double dDist = pnt.distanceTo( points[n] );

		if( LargerThan( dMin, dDist ) ){
			dMin = dDist;
			pntCalc = points[n];
		}
	}

	return ( dMin );
}

//	非端点存在判定
bool jhUtility::ExistNotEdgePoint( 
	const AcDbCurve* pCurve,			//(i)要素
	const AcGePoint3dArray& points		//(i)点列
	)
{
	bool bExist = false;
	AcGePoint3d pntS, pntE;
	
	pCurve->getStartPoint( pntS );
	pCurve->getEndPoint( pntE );

	for( int n = 0; n < points.length() && !bExist; n++ ){
		AcGePoint3d intersect = points[n];

		if(		!pntS.isEqualTo( intersect, jhOption::Instance().GetTol() ) 
			&&	!pntE.isEqualTo( intersect, jhOption::Instance().GetTol() ) ){
#ifdef _DEBUG
			acutPrintf(_T("\n pntS(%.4lf,%.4lf)"), pntS.x, pntS.y);
			acutPrintf(_T("\n pntE(%.4lf,%.4lf)"), pntE.x, pntE.y);
			acutPrintf(_T("\n intersect(%.4lf,%.4lf)"), intersect.x, intersect.y);
#endif
			bExist = true;
		}
	}
	
	return ( bExist );
}

//	座標矩形内判定
bool jhUtility::IsIn( 
	const AcGePoint3d& pnt,		//(i)判定座標
	const AcGePoint3d& pntMin,	//(i)矩形左下座標
	const AcGePoint3d& pntMax	//(i)矩形右上座標
	)
{
	bool bResult = true;

	if(		Larger( pnt.x, pntMax.x ) || Larger( pnt.y, pntMax.y ) 
		||	Smaller( pnt.x, pntMin.x ) || Smaller( pnt.y, pntMin.y ) )
		bResult = false;

	return ( bResult );
}

//	座標文字を返す
CString jhUtility::GetCodString( 
	const AcGePoint3d& pnt	//(i)座標
	)
{
	CString strCod;
	//--2008/1/30 Unicode 対応
#ifdef _UNICODE
	wostringstream ostr;
#else
	ostrstream ostr;
#endif

	ostr.setf( ios::fixed, ios::floatfield );	
	ostr.precision( JHC_LOG_PRECISION );

	//	端点座標文字列取得		
	ostr << _T('(') << pnt.x  << JHC_LOG_COD_SEP << pnt.y << _T(')') << ends;

	//--2008/1/30 Unicode 対応
#ifdef _UNICODE
	strCod = ostr.str().c_str();
#else
	strCod = ostr.str();
	ostr.rdbuf()->freeze(0);
#endif

	return ( strCod );
}

//	座標文字を返す
CString jhUtility::GetCodString2( 
	const AcGePoint3d& pnt	//(i)座標
	)
{
	CString strCod;
	//--2008/1/30 Unicode 対応
#ifdef _UNICODE
	wostringstream ostr;
#else
	ostrstream ostr;
#endif

	ostr.setf( ios::fixed, ios::floatfield );	
	ostr.precision( JHC_LOG_PRECISION );

	//	端点座標文字列取得		
	ostr << pnt.x  << JHC_LOG_COD_SEP << pnt.y << ends;

	//--2008/1/30 Unicode 対応
#ifdef _UNICODE
	strCod = ostr.str().c_str();
#else
	strCod = ostr.str();
	ostr.rdbuf()->freeze(0);
#endif

	return ( strCod );
}

//	アプリケーションキーを返す
CString jhUtility::GetAppKey()
{
	resbuf rb;
	CString strAppKey;

	acedGetVar( _T("ACADVER"), &rb );

	if( rb.resval.rstring != NULL ){
		//--2008/1/30 Unicode 対応
		//double dVer = atof(rb.resval.rstring);
		double dVer = _tstof(rb.resval.rstring);
		atmap<double,atstring> mapVer;
		atmap<double,atstring>::const_iterator it;

		free(rb.resval.rstring);

		mapVer.insert( atpair<double,atstring>(JHC_ACAD_SYSVAR_VER2000, JHC_APP_KEY_2000) );
		mapVer.insert( atpair<double,atstring>(JHC_ACAD_SYSVAR_VER2000I, JHC_APP_KEY_2000I) );
		mapVer.insert( atpair<double,atstring>(JHC_ACAD_SYSVAR_VER2002, JHC_APP_KEY_2002) );
		mapVer.insert( atpair<double,atstring>(JHC_ACAD_SYSVAR_VER2004, JHC_APP_KEY_2004) );
		mapVer.insert( atpair<double,atstring>(JHC_ACAD_SYSVAR_VER2005, JHC_APP_KEY_2005) );
		mapVer.insert( atpair<double,atstring>(JHC_ACAD_SYSVAR_VER2006, JHC_APP_KEY_2006) );
		//*Acadバージョンアップ対応
		//-- 2008/1/31 H.Suzuki after update begin
		mapVer.insert( atpair<double,atstring>(JHC_ACAD_SYSVAR_VER2007, JHC_APP_KEY_2007) );
		mapVer.insert( atpair<double,atstring>(JHC_ACAD_SYSVAR_VER2008, JHC_APP_KEY_2008) );
		//-- 2008/1/31 H.Suzuki after update end

		it = mapVer.find( dVer );
		if( it != mapVer.end() )
			strAppKey = it->second.c_str();
	}
	
	return ( strAppKey );
}

//	検出ファイルパスを取得する
Jhc::Result jhUtility::GetFindFilePath( 
	LPTSTR lpszFilePath //(o)Iniファイルパス
	)
{
	Jhc::Result result = Jhc::rOk;
	jhRegistry reg;
	TCHAR szInstPath[MAX_PATH];

	reg.SetKey( ACC_REG_KEY );

	//	レジストリデータを取得する
	//--2008/1/30 Unicode 対応
	//result = reg.ReadData( szInstPath, ACC_REG_DATA_INSTALL_PATH, sizeof(szInstPath) );
	result = reg.ReadData( szInstPath, ACC_REG_DATA_INSTALL_PATH, sizeof(szInstPath)/sizeof(TCHAR) );
	if( result == Jhc::rOk )
		_tmakepath( lpszFilePath, NULL, szInstPath, ACC_FIND_FILE_NAME, NULL );

	return ( result );
}
