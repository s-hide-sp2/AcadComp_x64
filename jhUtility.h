#pragma once

//##ModelId=4316551103B9

//	���[�e�B���B�e�B�N���X
class jhUtility{
public:

	//	�J�����g�h�L�������g�Z�b�g
	static Jhc::Result SetCurDocument( LPCTSTR lpszDwgPath );

	//	�����v�f���͂ދ�`�̍����E�E����W���v�Z����
	static bool GetMinMaxPoints( AcGePoint3d& pntMin, AcGePoint3d& pntMax, const AcDbVoidPtrArray& entArray	);

	static void UpdateView();

	//	�}�ʂ��J��
	static Jhc::Result OpenDwg( LPCTSTR lpszDwgPath );

	//	�}�ʃI�[�v���w���p�[
	static void openSyncDocHelper( void *pData );

	//	�R�}���h�Ăяo���R�}���h���s����
	static bool DoesExecuteCommandCall( const Jhc::CheckItem& checkItem );

	//	�}�ʖ��`�F�b�N����
	static bool DoesCheckByDwg( const Jhc::CheckItem& checkItem );

	//	�g�[�N�����擾����
	//--2008/1/30 Unicode �Ή�
	//static void GetTokens( std::vector<std::string>& vecToken, CString& str, TCHAR chSep );
	static void GetTokens( std::vector<atstring>& vecToken, CString& str, TCHAR chSep );

	//	�g�[�N�����擾����
	static CString StrTok( CString& str, TCHAR chSep );
	
	//	�t�@�C���p�X����t�H���_�����擾����
	static void GetFolderName( LPTSTR lpszFolder, LPCTSTR lpszFilePath );

	//	�ݒ�t�@�C���p�X���擾����
	//##ModelId=4316551103DF
	static Jhc::Result GetIniFilePath(LPTSTR lpszFilePath);

	//	�`�F�b�N���ʃt�@�C���p�X���擾����
	//##ModelId=4316551103DD
	static Jhc::Result GetCheckResultFilePath(LPTSTR lpszFilePath);

	//	�`�F�b�N���ʃt�@�C���p�X���擾����(AutoCad�\���p)
	//##ModelId=4316551103DD
	static Jhc::Result GetCheckResultFilePath2(LPTSTR lpszFilePath);

	//	���o�t�@�C���p�X���擾����
	static Jhc::Result GetFindFilePath( LPTSTR lpszFilePath );

	//	�t�@�C���p�X���擾����
	//##ModelId=4316551103DA
	static Jhc::Result GetFilePath(LPTSTR lpszFilePath, LPCTSTR lpszFileName);
	
	//	�`�F�b�N�T�����v�Z����
	//##ModelId=4316551103D5
	static UINT CalcCheckSum( LPCTSTR lpszBuff, UINT size );

	//	�G���[���b�Z�[�W��Ԃ�
	//##ModelId=4316551103D3
	static CString GetErrorMessage( Jhc::Result result );

	//	Ini�t�@�C�����`�F�b�N����
	//##ModelId=4316551103D2
	static Jhc::Result CheckIniFile();

	//	Ini����f�[�^���擾����
	//##ModelId=4316551103CD
	static Jhc::Result GetIniData( LPTSTR lpszValue, DWORD dwSize , LPCTSTR lpszSecName, LPCTSTR lpszKey );

	//	����r
	//##ModelId=4316551103CA
	static bool LargerThan( double dA, double dB );
	
	//	������
	static bool Larger( double dA, double dB );

	//	������
	static bool Smaller( double dA, double dB );

	//	=��r
	//##ModelId=4316551103BF
	static bool IsEqual( double dA, double dB );

	//	�|�����C������
	//##ModelId=4316551103BD
	static bool IsClosed( const AcDbPolyline* pPoly );

	//	�_����
	//##ModelId=4316551103BA
	static bool FindEqualPoint( const AcGePoint3d& pntTarget, const AcGePoint3dArray points );

	//	�ŏ������v�Z
	static double CalcMinDistPoint( AcGePoint3d& pntCalc, const AcGePoint3d& pnt, const AcGePoint3dArray& points );
	
	//	��[�_���ݔ���
	static bool ExistNotEdgePoint( const AcDbCurve* pCurve, const AcGePoint3dArray& points );

	//	���W��`������
	static bool IsIn( const AcGePoint3d& pnt, const AcGePoint3d& pntMin, const AcGePoint3d& pntMax );

	//	�|�����C��������
	static bool IsIn( const AcDbPolyline* pPoly, const AcGePoint3d& pnt );

	//	���W������Ԃ�
	static CString GetCodString( const AcGePoint3d& pnt );

	//	���W������Ԃ�
	static CString GetCodString2( const AcGePoint3d& pnt );

	//	��`�T�C�Y���v�Z����
	static Jhc::Result CalcRectSize( double& dW, double& dH, const AcDbPolyline* pPoly );

	//	��`�T�C�Y������Ԃ�
	static CString GetRectString( double dW, double dH );

	//	�A�v���P�[�V�����L�[��Ԃ�
	static CString GetAppKey();

};

//	����r
inline bool jhUtility::LargerThan( double dA, double dB )
{
	 return ( (dB - dA < JHC_TOL ) ? true : false );
}

//	=��r
inline bool jhUtility::IsEqual( double dA, double dB )
{
	return ( (fabs(dB-dA) < JHC_TOL ) ? true : false );
}

//	������
inline bool jhUtility::Larger( double dA, double dB )
{
	return ( ( !IsEqual(dA,dB) && LargerThan(dA,dB) ) ? true : false );
}

//	������
inline bool jhUtility::Smaller( double dA, double dB )
{
	return ( Larger(dB, dA) );
}




