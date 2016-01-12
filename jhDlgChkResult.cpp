// jhDlgChkResult.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "resource.h"
#include "jhTypeDef.h"
#include "jhDlgChkResult.h"
#include "jhUtility.h"
#include "jhOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
//--2008/1/30 Unicode 対応
//static char THIS_FILE[] = __FILE__;
static TCHAR THIS_FILE[] = __FILE__;
#endif

#define JHC_COL_NO				(0)
#define JHC_COL_FILE_PATH		(1)
#define JHC_COL_CHECK_ITEM		(2)
#define JHC_COL_CHECK_RESULT	(3)
#define JHC_COL_CHECK_CONTENTS	(4)
#define JHC_COL_FIGURE_HANDLE	(5)
#define JHC_COL_ZOOM_WAY		(6)
#define JHC_NUM_OF_COL_CHK_RESULT JHC_COL_ZOOM_WAY+1

jhDlgChkResult jhDlgChkResult::m_dlg;

/////////////////////////////////////////////////////////////////////////////
// jhDlgChkResult ダイアログ


jhDlgChkResult::jhDlgChkResult(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(jhDlgChkResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(jhDlgChkResult)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	SetShow( FALSE );
	memset( &m_rectWnd, 0, sizeof(m_rectWnd) );
	memset( &m_szFilePathInChkCurDwg, 0, sizeof(m_szFilePathInChkCurDwg) );

	m_vecColWidth.resize(JHC_NUM_OF_COL_CHK_RESULT);
	m_vecColWidth[JHC_COL_NO] = 30;
	m_vecColWidth[JHC_COL_FILE_PATH] = 135;
	m_vecColWidth[JHC_COL_CHECK_ITEM] = 135;
	m_vecColWidth[JHC_COL_CHECK_RESULT] = 40;
	m_vecColWidth[JHC_COL_CHECK_CONTENTS] = 260;
#ifdef _DEBUG
	m_vecColWidth[JHC_COL_FIGURE_HANDLE] = 70;
	m_vecColWidth[JHC_COL_ZOOM_WAY] = 10;
#else
	m_vecColWidth[JHC_COL_FIGURE_HANDLE] = 0;
	m_vecColWidth[JHC_COL_ZOOM_WAY] = 0;
#endif

}

void jhDlgChkResult::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(jhDlgChkResult)
	DDX_Control(pDX, IDC_LSV_CHK_RESULT, m_lvChkResult);
	//}}AFX_DATA_MAP
}

jhDlgChkResult& jhDlgChkResult::Instance()
{
	return ( m_dlg );
}

//	初期化
BOOL jhDlgChkResult::Initialize()
{
	BOOL bResult = TRUE;

	bResult = m_lvChkResult.Initialize();

	if( bResult )
		bResult = m_lvChkResult.AddColumn( JHC_COL_NO, _T("No"), GetColWidth(JHC_COL_NO) );
	if( bResult )
		bResult = m_lvChkResult.AddColumn( JHC_COL_FILE_PATH, _T("ファイル名"), GetColWidth(JHC_COL_FILE_PATH) );
	if( bResult )
		bResult = m_lvChkResult.AddColumn( JHC_COL_CHECK_ITEM, _T("チェック項目"), GetColWidth(JHC_COL_CHECK_ITEM) );
	if( bResult )
		bResult = m_lvChkResult.AddColumn( JHC_COL_CHECK_RESULT, _T("結果"), GetColWidth(JHC_COL_CHECK_RESULT) );
	if( bResult )
		bResult = m_lvChkResult.AddColumn( JHC_COL_CHECK_CONTENTS, _T("内容"), GetColWidth(JHC_COL_CHECK_CONTENTS) );
	if( bResult )
		bResult = m_lvChkResult.AddColumn( JHC_COL_FIGURE_HANDLE, _T("図形ハンドル"), GetColWidth(JHC_COL_FIGURE_HANDLE) );
	if( bResult )
		bResult = m_lvChkResult.AddColumn( JHC_COL_ZOOM_WAY, _T("ズーム方法"), GetColWidth(JHC_COL_ZOOM_WAY) );

	return ( bResult );
}

//	リストビューにデータをセット
Jhc::Result jhDlgChkResult::SetDataToListView()
{
	Jhc::Result result = Jhc::rOk;
	TCHAR szFilePath[MAX_PATH];

	if( !m_lvChkResult.DeleteAllItems() )
		return ( Jhc::rFail );

	//	図面内チェックの場合チェック時の図面ファイルパスをセット
	if( jhOption::Instance().GetCheckCurDwg() )
		SetFilePathInChkCurDwg( curDoc()->fileName() );

	m_lvChkResult.ShowWindow( SW_HIDE );

	//	図面内チェック用ＣＳＶ
	if( jhOption::Instance().GetCheckCurDwg() )
		result = jhUtility::GetFilePath( szFilePath, JHC_CHECK_IN_DWG_RESULT_FILE_NAME );
	else
		result = jhUtility::GetFilePath( szFilePath, JHC_CHECK_FOR_DISP_RESULT_FILE_NAME );
	
	if( result == Jhc::rOk ){
		CStdioFile cf( szFilePath, CFile::modeRead | CFile::typeText );
		CString str;

		for( int nRow = 0; cf.ReadString( str ); nRow++ ){
			//--2008/1/30 Unicode 対応
			//std::vector<std::string> vecToken;
			std::vector<atstring> vecToken;
			TCHAR szBuff[64];
			
			jhUtility::GetTokens( vecToken, str, _T(',') );

			//	No セット
			m_lvChkResult.SetCellText( 0, nRow, _itot(nRow+1, szBuff, 10) );

			//	CSV内容セット
			for( int nCol = 0; nCol < vecToken.size(); nCol++ )
				m_lvChkResult.SetCellText( nCol+1, nRow, vecToken[nCol].c_str() );
		}
	}

	m_lvChkResult.ShowWindow( SW_SHOWNA );

	return ( result );
}

//	ダイアログを表示する
BOOL jhDlgChkResult::Show()
{
	BOOL bResult = TRUE;
	
	if( !GetShow() ){	
		bResult = Create( IDD_DLG_CHK_RESULT, acedGetAcadFrame());
		if( bResult )
			ShowWindow( SW_SHOWNORMAL );

		SetShow( bResult );
		if( bResult ){
			int width = m_rectWnd.right - m_rectWnd.left;
			int hight = m_rectWnd.bottom - m_rectWnd.top;
		
			if( 0 < width && 0 < hight )
				MoveWindow( m_rectWnd.left, m_rectWnd.top, width, hight );
		}
	}
	else
		bResult = SetDataToListView();

	return ( bResult );
}

//	図形ハンドルをキーにエンティテイをオープン
Jhc::Result jhDlgChkResult::OpenEntity( 
	AcDbVoidPtrArray& entArray, //(o)エンティティ
	LPCTSTR lpszHandle			//(i)図形ハンドル
	) const
{
	Jhc::Result result = Jhc::rOk;
	//--2008/1/30 Unicode 対応
	//std::vector<std::string> vecHandle;
	std::vector<atstring> vecHandle;
	CString strHandle( lpszHandle );
	
	//	図形ハンドル文字列から図形ハンドル取得
	jhUtility::GetTokens( vecHandle, strHandle, JHC_LOG_COD_TO_COD_SEP );
	
	//	図形ハンドルに対応するエンティティをオープン
	for( int n = 0; n < vecHandle.size() && result == Jhc::rOk; n++ ){
		AcDbObjectId objId;
		AcDbHandle handle( vecHandle[n].c_str() );
		AcDbEntity* pEnt = NULL;
		Acad::ErrorStatus es;

		//	図形ハンドルに対するオブジェクトID取得
		es = acdbCurDwg()->getAcDbObjectId(objId, Adesk::kFalse, handle);
		//	既に削除されている要素や図面が閉じられている可能性があるのでエラーは無視
		if( es == Acad::eOk && objId.isValid() )
			es = acdbOpenAcDbEntity( pEnt, objId, AcDb::kForRead );
		if( pEnt != NULL )
			entArray.append( pEnt );
	}
	
	return ( result );
}

void jhDlgChkResult::OnDblclkListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	Jhc::Result result = Jhc::rOk;
	HD_NOTIFY* phdn = (HD_NOTIFY*)pNMHDR;
	int nRow = phdn->iItem;
	CString strFilePath = m_lvChkResult.GetItemText( nRow, JHC_COL_FILE_PATH );
	CString strCheckResult = m_lvChkResult.GetItemText( nRow, JHC_COL_CHECK_RESULT );
	
	//	チェックＯＫの場合は何もしない
	if( strCheckResult == _T("OK") )
		return;

	//	図面内チェック時はチェックした図面をカレント図面に設定する
	if( jhOption::Instance().GetCheckCurDwg() ){
		if( _tcsicmp( curDoc()->fileName(), GetFilePathInChkCurDwg() ) )
			result = jhUtility::SetCurDocument( GetFilePathInChkCurDwg() );
	}
	//	チェック結果表示の場合は図面オープン
	else
		result = jhUtility::OpenDwg( strFilePath );

	if( result == Jhc::rOk ){
		CString strItem = m_lvChkResult.GetItemText( nRow, JHC_COL_CHECK_ITEM );

		if(		strItem != _T("ファイル名チェック") 
			&&	strItem != _T("レイヤチェック") ){
			CString strZoomWay = m_lvChkResult.GetItemText( nRow, JHC_COL_ZOOM_WAY );
			CString strFigureHandle = m_lvChkResult.GetItemText( nRow, JHC_COL_FIGURE_HANDLE );
			Jhc::ZoomWay zoomWay = (Jhc::ZoomWay)_ttoi(strZoomWay);
			AcGePoint2dArray points;
			
			//	エンティティの中心にズームをかける
			if( zoomWay == Jhc::zEntCenter ){
				AcDbVoidPtrArray entArray;
				AcGePoint3d pntMin, pntMax, pntCenter;

				//	要素オープン
				result = OpenEntity( entArray, strFigureHandle );
				if( result == Jhc::rOk ){	
					//	全要素の矩形サイズ取得
					jhUtility::GetMinMaxPoints( pntMin, pntMax, entArray );
					//	エンティティを閉じる
					for( int n = 0;  n < entArray.length(); n++ )
						AcDbEntity::cast((AcRxObject*)entArray[n])->close();

					pntCenter = pntMin + 0.5*( pntMax - pntMin );
					points.append( AcGePoint2d( pntCenter.x, pntCenter.y ) );
					Zoom( points, (pntMax.x - pntMin.x)+20.0, (pntMax.y - pntMin.y)+20.0 );
					Highlight( strFigureHandle );
				}
			}
			//	座標ズーム
			else{
				CString strContents = m_lvChkResult.GetItemText( nRow, JHC_COL_CHECK_CONTENTS );
				
				//	内容列から座標取得
				GetPointsFromContents( points, strContents );
				Zoom( points, 30.0, 30.0 );
				Highlight( strFigureHandle );
			}		
		}
	}
	else if( result != Jhc::rCancel )
		AfxMessageBox( jhUtility::GetErrorMessage(result) );
}

//	ハイライトを解除する
Jhc::Result jhDlgChkResult::Unhighlight()
{
	Jhc::Result result = Jhc::rOk;

	//	図面を閉じられている要素のハイライトは失敗する為エラーは無視
	for( int n = 0; n < m_highlightIdArray.length(); n++ ){
		result = Unhighlight( m_highlightIdArray[n] );
		assert( result == Jhc::rOk );
	}
	
	m_highlightIdArray.setLogicalLength(0);

	return ( result );
}

//	要素をハイライトする
Jhc::Result jhDlgChkResult::Highlight( 
	CString& strHandles //(i)図形ハンドル文字列
	)
{
	Jhc::Result result = Jhc::rOk;
	//--2008/1/30 Unicode 対応
/*	std::vector<std::string> vecHandle;
	std::vector<std::string>::const_iterator it;*/
	std::vector<atstring> vecHandle;
	std::vector<atstring>::const_iterator it;

	//	前回ハイライトした要素を非ハイライト表示にする
	Unhighlight();

	//	トークンを取得する
	jhUtility::GetTokens( vecHandle, strHandles, JHC_LOG_COD_TO_COD_SEP );

	for( it = vecHandle.begin(); it != vecHandle.end() && result == Jhc::rOk; it++ )
		result = Highlight( it->c_str() );
			
	//	ビューの表示状態を更新
	jhUtility::UpdateView();

	return ( result );
}

//	要素をハイライトする
Jhc::Result jhDlgChkResult::Highlight( 
	LPCTSTR lpszHandle	//(i)図形ハンドル
	)
{
	Jhc::Result result = Jhc::rOk;
	AcDbHandle handle( lpszHandle );
	AcDbObjectId objId;
	Acad::ErrorStatus es;
	AcDbEntity* pEnt = NULL;
	bool bLockDoc = false;
	
	//	図形ハンドルに対するオブジェクトID取得
	es = acdbCurDwg()->getAcDbObjectId(objId, Adesk::kFalse, handle);
	if( es != Acad::eOk )
		return ( Jhc::rCancel );
	
	//	ドキュメントをロックしオブジェクトの更新権限を得る
	es = acDocManager->lockDocument(curDoc(), AcAp::kXWrite, NULL, NULL, false);

	if( es == Acad::eOk ){
		es = acdbOpenAcDbEntity( pEnt, objId, AcDb::kForRead );
		bLockDoc = true;
	}

	//	要素をハイライト
	if( es == Acad::eOk ){
		AcDbLayerTableRecord* pLayer = NULL;
		
		//	要素のレイヤが非表示なら表示状態にする
		es = acdbOpenAcDbObject( (AcDbObject*&)pLayer, pEnt->layerId(), AcDb::kForRead );
		
		if( es == Acad::eOk ){
			if( pLayer->isOff() ){
				es = pLayer->upgradeOpen();
				if( es == Acad::eOk )
					pLayer->setIsOff( false );
			}
			pLayer->close();
		}
		
		pEnt->highlight();
		pEnt->close();
	}
	
	if( es != Acad::eOk )
		result = Jhc::rAcadError;
	
	//	ハイライト要素ＩＤ追加
	if( result == Jhc::rOk )
		m_highlightIdArray.append( objId );

	//	ビューの表示状態を更新
//	jhUtility::UpdateView();

	//	ドキュメントロック解除
	if( bLockDoc ){
		es = acDocManager->unlockDocument(curDoc());
		assert( es == Acad::eOk );
	}

	return ( result );
}

//	ハイライトを解除する
Jhc::Result jhDlgChkResult::Unhighlight( 
	const AcDbObjectId& objId //(i)オブジェクトID
	) const
{
	Jhc::Result result = Jhc::rOk;
	Acad::ErrorStatus es;
	AcDbEntity* pEnt = NULL;

	if( objId.isNull() || !objId.isValid() )
		return ( Jhc::rOk );

	es = acdbOpenAcDbEntity( pEnt, objId, AcDb::kForRead );
	if( es == Acad::eOk ){
		pEnt->unhighlight();
		pEnt->close();
	}
	else
		result = Jhc::rAcadError;

	return ( result );
}

//	チェック内容から座標を取得する
Jhc::Result jhDlgChkResult::GetEntityGeom( 
	AcGePoint2d& pntCenter, 
	double& dW, 
	double& dH, 
	LPCTSTR lpszHandle 
	) const
{
	Jhc::Result result = Jhc::rOk;
	AcDbHandle handle( lpszHandle );
	AcDbObjectId objId;
	Acad::ErrorStatus es;
	AcDbEntity* pEnt = NULL;
	
	//	図形ハンドルに対するオブジェクトID取得
	es = acdbCurDwg()->getAcDbObjectId(objId, Adesk::kFalse, handle);
	if( es != Acad::eOk )
		return ( Jhc::rCancel );
	
	es = acdbOpenAcDbEntity( pEnt, objId, AcDb::kForRead );
	if( es == Acad::eOk ){
		AcDbExtents extents;
		AcGePoint3d pntMin, pntMax, pntCenter3d;
		
		pEnt->getGeomExtents(extents);
		pEnt->close();

		pntMin = extents.minPoint();
		pntMax = extents.maxPoint();
		
		pntCenter3d = pntMin + 0.5*(pntMax-pntMin);
		pntCenter.set( pntCenter3d.x, pntCenter3d.y );
		dW = pntMax.x - pntMin.x;
		dH = pntMax.y - pntMin.y;
	}

	return ( result );
}

BOOL jhDlgChkResult::GetPointsFromContents(
	AcGePoint2dArray& points,	//(o)座標
	CString& strContents		//(i/o)内容
	) const
{
	BOOL bResult = TRUE;
	
	if( strContents.Find(JHC_LOG_COD_SEP) == -1 )
		bResult = FALSE;
	else{
		//std::vector<std::string> vecTokens;
		std::vector<atstring> vecTokens;

		jhUtility::GetTokens( vecTokens, strContents, JHC_LOG_COD_TO_COD_SEP );
		
		for( int n = 0; n < vecTokens.size(); n++ ){
			CString str( vecTokens[n].c_str() );
			double dX, dY;
			
			//--2008/1/30 Unicode 対応
		/*	dX = atof( jhUtility::StrTok( str, JHC_LOG_COD_SEP ) );
			dY = atof( jhUtility::StrTok( str, JHC_LOG_COD_TO_COD_SEP ) );*/
			dX = _tstof( jhUtility::StrTok( str, JHC_LOG_COD_SEP ) );
			dY = _tstof( jhUtility::StrTok( str, JHC_LOG_COD_TO_COD_SEP ) );

			points.append( AcGePoint2d(dX, dY) );
		}
	}

	return ( bResult );
}

//	ズームする
void jhDlgChkResult::Zoom( 
	const AcGePoint2dArray& points, //(i)ズーム座標
	double dW, 
	double dH
	)
{	
    AcDbViewTableRecord view;
	AcGePoint2d pntCenter;

	if( points.isEmpty() )
		return;

	//	ズーム座標の中心を求める
	for( int n = 0; n < points.length(); n++ )
		 pntCenter += points[n].asVector();
	
	pntCenter /= (double)points.length();

	//	ズームを行なう
    view.setCenterPoint( pntCenter );
    view.setHeight( dW );
    view.setWidth( dH );
	acedSetCurrentView( &view, NULL );
}

//	列幅を返す
long jhDlgChkResult::GetColWidth( 
	long lCol	//(i)列幅
	) const
{
	long lWidth = 0;
	
	if( lCol < m_vecColWidth.size() )
		lWidth = m_vecColWidth[lCol];

	if( jhOption::Instance().GetCheckCurDwg() ){
		if( lCol == JHC_COL_NO || lCol == JHC_COL_FILE_PATH )
			lWidth = 0;
	}

	return ( lWidth );
}

//	終了処理
void jhDlgChkResult::EndProc()
{
	GetWindowRect(&m_rectWnd);
	for( int n = 0; n < JHC_NUM_OF_COL_CHK_RESULT; n++ ){
		//	図面内チェック時のNO・ファイルパス　の列幅は保持しない
		if(		jhOption::Instance().GetCheckCurDwg() 
			&&  (n == JHC_COL_NO || n == JHC_COL_FILE_PATH) )
			continue;
		
		m_vecColWidth[n] = m_lvChkResult.GetColumnWidth( n );
	}
	
	if( DestroyWindow() )
		SetShow(FALSE);

	//	前回ハイライトした要素を非ハイライト表示にする
	Unhighlight();
	
	//	ビューの表示状態を更新
	jhUtility::UpdateView();
}

BEGIN_MESSAGE_MAP(jhDlgChkResult, CAcUiDialog)
	//{{AFX_MSG_MAP(jhDlgChkResult)
	ON_MESSAGE( WM_ACAD_KEEPFOCUS, onAcadKeepFocus )
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// jhDlgChkResult メッセージ ハンドラ

LONG jhDlgChkResult::onAcadKeepFocus( UINT, LONG )
{
    return TRUE;
}

void jhDlgChkResult::OnClose() 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	EndProc();

	CAcUiDialog::OnClose();
}

void jhDlgChkResult::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	EndProc();

	CAcUiDialog::OnCancel();
}

BOOL jhDlgChkResult::OnInitDialog() 
{
	CAcUiDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	Initialize();

	SetDataToListView();
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void jhDlgChkResult::OnSize(UINT nType, int cx, int cy) 
{
	CAcUiDialog::OnSize(nType, cx, cy);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	WINDOWPLACEMENT place;
		
	if( m_lvChkResult.GetWindowPlacement( &place ) ){
		int width, hight;
	
		width = cx - 2 * place.rcNormalPosition.left;
		hight = cy - 2 * place.rcNormalPosition.top;
		if( width <= 0 )
			width = 1;
		if( hight <= 0 )
			hight = 1;

		m_lvChkResult.MoveWindow(	place.rcNormalPosition.left, 
									place.rcNormalPosition.top, 
									width, 
									hight );
	}	
}
