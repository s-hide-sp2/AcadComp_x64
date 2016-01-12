#pragma once

//##ModelId=4316551103B9

//	ユーティリィティクラス
class jhUtility{
public:

	//	カレントドキュメントセット
	static Jhc::Result SetCurDocument( LPCTSTR lpszDwgPath );

	//	複数要素を囲む矩形の左下・右上座標を計算する
	static bool GetMinMaxPoints( AcGePoint3d& pntMin, AcGePoint3d& pntMax, const AcDbVoidPtrArray& entArray	);

	static void UpdateView();

	//	図面を開く
	static Jhc::Result OpenDwg( LPCTSTR lpszDwgPath );

	//	図面オープンヘルパー
	static void openSyncDocHelper( void *pData );

	//	コマンド呼び出しコマンド実行判定
	static bool DoesExecuteCommandCall( const Jhc::CheckItem& checkItem );

	//	図面毎チェック判定
	static bool DoesCheckByDwg( const Jhc::CheckItem& checkItem );

	//	トークンを取得する
	//--2008/1/30 Unicode 対応
	//static void GetTokens( std::vector<std::string>& vecToken, CString& str, TCHAR chSep );
	static void GetTokens( std::vector<atstring>& vecToken, CString& str, TCHAR chSep );

	//	トークンを取得する
	static CString StrTok( CString& str, TCHAR chSep );
	
	//	ファイルパスからフォルダ名を取得する
	static void GetFolderName( LPTSTR lpszFolder, LPCTSTR lpszFilePath );

	//	設定ファイルパスを取得する
	//##ModelId=4316551103DF
	static Jhc::Result GetIniFilePath(LPTSTR lpszFilePath);

	//	チェック結果ファイルパスを取得する
	//##ModelId=4316551103DD
	static Jhc::Result GetCheckResultFilePath(LPTSTR lpszFilePath);

	//	チェック結果ファイルパスを取得する(AutoCad表示用)
	//##ModelId=4316551103DD
	static Jhc::Result GetCheckResultFilePath2(LPTSTR lpszFilePath);

	//	検出ファイルパスを取得する
	static Jhc::Result GetFindFilePath( LPTSTR lpszFilePath );

	//	ファイルパスを取得する
	//##ModelId=4316551103DA
	static Jhc::Result GetFilePath(LPTSTR lpszFilePath, LPCTSTR lpszFileName);
	
	//	チェックサムを計算する
	//##ModelId=4316551103D5
	static UINT CalcCheckSum( LPCTSTR lpszBuff, UINT size );

	//	エラーメッセージを返す
	//##ModelId=4316551103D3
	static CString GetErrorMessage( Jhc::Result result );

	//	Iniファイルをチェックする
	//##ModelId=4316551103D2
	static Jhc::Result CheckIniFile();

	//	Iniからデータを取得する
	//##ModelId=4316551103CD
	static Jhc::Result GetIniData( LPTSTR lpszValue, DWORD dwSize , LPCTSTR lpszSecName, LPCTSTR lpszKey );

	//	≧比較
	//##ModelId=4316551103CA
	static bool LargerThan( double dA, double dB );
	
	//	＞判定
	static bool Larger( double dA, double dB );

	//	＜判定
	static bool Smaller( double dA, double dB );

	//	=比較
	//##ModelId=4316551103BF
	static bool IsEqual( double dA, double dB );

	//	閉ポリライン判定
	//##ModelId=4316551103BD
	static bool IsClosed( const AcDbPolyline* pPoly );

	//	点検索
	//##ModelId=4316551103BA
	static bool FindEqualPoint( const AcGePoint3d& pntTarget, const AcGePoint3dArray points );

	//	最小距離計算
	static double CalcMinDistPoint( AcGePoint3d& pntCalc, const AcGePoint3d& pnt, const AcGePoint3dArray& points );
	
	//	非端点存在判定
	static bool ExistNotEdgePoint( const AcDbCurve* pCurve, const AcGePoint3dArray& points );

	//	座標矩形内判定
	static bool IsIn( const AcGePoint3d& pnt, const AcGePoint3d& pntMin, const AcGePoint3d& pntMax );

	//	ポリライン内判定
	static bool IsIn( const AcDbPolyline* pPoly, const AcGePoint3d& pnt );

	//	座標文字を返す
	static CString GetCodString( const AcGePoint3d& pnt );

	//	座標文字を返す
	static CString GetCodString2( const AcGePoint3d& pnt );

	//	矩形サイズを計算する
	static Jhc::Result CalcRectSize( double& dW, double& dH, const AcDbPolyline* pPoly );

	//	矩形サイズ文字を返す
	static CString GetRectString( double dW, double dH );

	//	アプリケーションキーを返す
	static CString GetAppKey();

};

//	≧比較
inline bool jhUtility::LargerThan( double dA, double dB )
{
	 return ( (dB - dA < JHC_TOL ) ? true : false );
}

//	=比較
inline bool jhUtility::IsEqual( double dA, double dB )
{
	return ( (fabs(dB-dA) < JHC_TOL ) ? true : false );
}

//	＞判定
inline bool jhUtility::Larger( double dA, double dB )
{
	return ( ( !IsEqual(dA,dB) && LargerThan(dA,dB) ) ? true : false );
}

//	＜判定
inline bool jhUtility::Smaller( double dA, double dB )
{
	return ( Larger(dB, dA) );
}




