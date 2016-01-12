#if !defined(AFX_JHDLGCHKRESULT_H__3E0DAFE8_8092_4691_B44A_242C1B291743__INCLUDED_)
#define AFX_JHDLGCHKRESULT_H__3E0DAFE8_8092_4691_B44A_242C1B291743__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// jhDlgChkResult.h : ヘッダー ファイル
//

#include "jhListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// jhDlgChkResult ダイアログ

class jhDlgChkResult : public CAcUiDialog
{
	friend class jhListCtrl;
// コンストラクション
public:
	jhDlgChkResult(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(jhDlgChkResult)
	enum { IDD = IDD_DLG_CHK_RESULT };
	jhListCtrl	m_lvChkResult;
	//}}AFX_DATA

	//	初期化
	BOOL Initialize();

	//	ダイアログを表示する
	BOOL Show();

	static jhDlgChkResult& Instance();

	//	ダイアログ表示フラグを返す
	BOOL GetShow() const;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(jhDlgChkResult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	virtual void OnDblclkListCtrl(NMHDR* pNMHDR, LRESULT* pResult);

private:
	//	リストビューにデータをセット
	Jhc::Result SetDataToListView();

	//	チェック内容から座標を取得する
	BOOL GetPointsFromContents( AcGePoint2dArray& points, CString& strContents ) const;

	//	チェック内容から座標を取得する
	Jhc::Result GetEntityGeom( AcGePoint2d& pntCenter, double& dW, double& dH, LPCTSTR lpszHandle ) const;

	//	ズームする
	void Zoom( const AcGePoint2dArray& points, double dW = 20.0, double dH = 20.0 );

	//	要素をハイライトする
	Jhc::Result Highlight( LPCTSTR lpszHandle );

	//	要素をハイライトする
	Jhc::Result Highlight( CString& strHandles );
	
	//	終了処理
	void EndProc();

	//	ダイアログ表示フラグをセットする
	void SetShow( BOOL value );

	//	列幅を返す
	long GetColWidth( long lCol ) const;

	AcDbObjectId GetEntIdOfHighlight() const;
	void SetEntIdOfHighlight( const AcDbObjectId& value );

	//	ハイライトを解除する
	Jhc::Result Unhighlight();

	//	ハイライトを解除する
	Jhc::Result Unhighlight( const AcDbObjectId& objId ) const;

	//	図形ハンドルをキーにエンティテイをオープン
	Jhc::Result OpenEntity( AcDbVoidPtrArray& entArray, LPCTSTR lpszHandle ) const;

	//	図面内チェック時の図面ファイルパス
	void SetFilePathInChkCurDwg( LPCTSTR value );
	LPCTSTR GetFilePathInChkCurDwg() const;

private:

	//	表示フラグ
	BOOL m_bShow;

	//	ダイアログシングルトンオブジェクト
	static jhDlgChkResult m_dlg;

	//	リストビュー列幅
	std::vector<long> m_vecColWidth;

	//	ウィンドウ位置
	RECT m_rectWnd;

	//	ハイライト要素のID
	//AcDbObjectId		m_entIdOfHighlight;
	AcDbObjectIdArray	m_highlightIdArray;

	//	図面内チェック時の図面ファイルパス
	TCHAR m_szFilePathInChkCurDwg[MAX_PATH];

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(jhDlgChkResult)
	afx_msg LONG onAcadKeepFocus( UINT, LONG );
	afx_msg void OnClose();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//	図面内チェック時の図面ファイルパス
inline void jhDlgChkResult::SetFilePathInChkCurDwg( 
	LPCTSTR value	//(i)
	)
{
	memset( m_szFilePathInChkCurDwg, 0, sizeof(m_szFilePathInChkCurDwg) );
	//--2008/1/30 Unicode 対応
	//_tcsncpy( m_szFilePathInChkCurDwg, value, sizeof(m_szFilePathInChkCurDwg)-1);
	_tcsncpy( m_szFilePathInChkCurDwg, value, sizeof(m_szFilePathInChkCurDwg)/sizeof(TCHAR)-1);

}

inline LPCTSTR jhDlgChkResult::GetFilePathInChkCurDwg() const
{
	return ( m_szFilePathInChkCurDwg );
}

//	ダイアログ表示フラグを返す
inline BOOL jhDlgChkResult::GetShow() const
{
	return ( m_bShow );
}

//	ダイアログ表示フラグをセットする
inline void jhDlgChkResult::SetShow( BOOL value )
{
	m_bShow = value;
}


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_JHDLGCHKRESULT_H__3E0DAFE8_8092_4691_B44A_242C1B291743__INCLUDED_)
