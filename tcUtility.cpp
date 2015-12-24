#include "StdAfx.h"
#include "tcUtility.h"
#include "tcRegistry.h"

//	�C���X�g�[���������W�X�g���ɓo�^����
extern "C" long __stdcall tcRegInstallInfo( 
	LPCTSTR lpszAppKey,		//(i)�A�v���P�[�V�����L�[
	long lUseTerm,			//(i)�g�p����
	bool bOverWrite			//(i)�㏑���t���O
	)
{
	Tcs::Result result = Tcs::rOk;
	TCHAR szKey[1024];

	//	���W�X�g���L�[������擾
	result = tcGetRegKey( szKey, lpszAppKey, sizeof(szKey) );
	
	if( result == Tcs::rOk ){
		tcRegistry reg;
		TCHAR szInstallDay[8+1];
		TCHAR szUseTerm[128];
		bool bReg = false;
	
		reg.SetKey( szKey );

		//	�g�p���Ԃ��擾���C���X�g�[�����̗L���𔻒肷��
		result = reg.ReadData( szUseTerm, TCS_REG_DATA_USE_SPAN, sizeof(szUseTerm) );
		
		//	�C���X�g�[������񂪂Ȃ��ꍇ�͓o�^
		if(	result == Tcs::rFailToAccessRegistry )
			bReg = true;
		//	�C���X�g�[������񂪂���ꍇ�͏㏑��
		else if	( result == Tcs::rOk && bOverWrite )
			bReg = true;
		
		if( bReg ){
			//	�C���X�g�[�����t�o�^
			tcGetDayString( szInstallDay, CTime::GetCurrentTime() );
			result = reg.WriteData( szInstallDay, TCS_REG_DATA_INSTALL_DAY );
			
			//	�g�p���ԓo�^
			if( result == Tcs::rOk )
				//2006.07.01
				result = reg.WriteData( (LPCTSTR)_itoa(lUseTerm,(char *)szUseTerm,10), TCS_REG_DATA_USE_SPAN );

			//	�A�N�Z�X�����X�V����
			if( result == Tcs::rOk )
				result = (Tcs::Result)tcUpdateAccessDay( lpszAppKey );
		}
	}

	return ( result );
}

//	�A�N�Z�X�����X�V����
extern "C" long __stdcall tcUpdateAccessDay( 
	LPCTSTR lpszAppKey //(i)�A�v���P�[�V�����L�[
	)
{
	Tcs::Result result = Tcs::rOk;
	TCHAR szKey[1024];

	//	���W�X�g���L�[������擾
	result = tcGetRegKey( szKey, lpszAppKey, sizeof(szKey) );
	
	if( result == Tcs::rOk ){
		tcRegistry reg;
		TCHAR szAccessDay[8+1];
	
		reg.SetKey( szKey );

		//	�A�N�Z�X����o�^����
		tcGetDayString( szAccessDay, CTime::GetCurrentTime() );
		result = reg.WriteData( szAccessDay, TCS_REG_DATA_ACCESS_DAY );
	}

	return ( result );
}

//	�g�p���Ԃ��`�F�b�N����
extern "C" long __stdcall tcCheckUseTerm( 
	LPCTSTR lpszAppKey //(i)�A�v���P�[�V�����L�[
	)
{
	Tcs::Result result = Tcs::rOk;
	TCHAR szKey[1024];
	TCHAR szInstallDay[8+1];
	TCHAR szAccessDay[8+1];
	TCHAR szUseTerm[128];



	//	���W�X�g���L�[������擾
	result = tcGetRegKey( szKey, lpszAppKey, sizeof(szKey) );
	
	if( result == Tcs::rOk ){
		tcRegistry reg;
		
		reg.SetKey(szKey);

		//	�C���X�g�[�����t�擾
		result = reg.ReadData( szInstallDay, TCS_REG_DATA_INSTALL_DAY, sizeof(szInstallDay) );

		//	�A�N�Z�X���t�擾
		if( result == Tcs::rOk )
			result = reg.ReadData( szAccessDay, TCS_REG_DATA_ACCESS_DAY, sizeof(szAccessDay) );

		//	�g�p���Ԏ擾
		if( result == Tcs::rOk )
			result = reg.ReadData( szUseTerm, TCS_REG_DATA_USE_SPAN, sizeof(szUseTerm) );
	}

	//	�g�p���Ԃ��`�F�b�N
	if( result == Tcs::rOk ){
		CString strInstallDay( szInstallDay );
		CString strAccessDay( szAccessDay );
		CTime timeCur = CTime::GetCurrentTime();
		CTime timeInst( _ttoi(strInstallDay.Left(4)), _ttoi(strInstallDay.Mid(4,2)), _ttoi(strInstallDay.Mid(6,2)), 0,0,0  );
		CTime timeAccess( _ttoi(strAccessDay.Left(4)), _ttoi(strAccessDay.Mid(4,2)), _ttoi(strAccessDay.Mid(6,2)), 0,0,0  );
		
		//	�C���X�g�[�����t <= �X�V���t <= ���ݓ��t �`�F�b�N
		if( timeInst <= timeAccess && timeAccess <= timeCur ){
			CTimeSpan span = timeCur - timeInst;
			//2006.07.01
			LONG lUseSpan = (LONG)span.GetDays();
			
			//�X�V���t���X�V���� suda
			tcUpdateAccessDay(lpszAppKey);

			//	�g�p���ԃ`�F�b�N
			if( _ttol(szUseTerm) <= lUseSpan )
				result = Tcs::rOverUseSpan;
		}
		else
			result = Tcs::rInvalidRegistryInfo;
	}

	return ( result );
}

//	���W�X�g���L�[���擾����
Tcs::Result tcGetRegKey( 
	LPTSTR lpszRegKey,	//(o)���W�X�g���L�[
	LPCTSTR lpszAppKey,	//(i)�A�v���P�[�V�����L�[
	DWORD dwSize		//(i)�o�b�t�@�T�C�Y
	)
{
	Tcs::Result result = Tcs::rOk;
	TCHAR szBuff[1024];

	//	���W�X�g���L�[�Z�b�g
	_stprintf( szBuff, _T("%s\\%s"), TCS_REG_KEY_INSTALL_INFO, lpszAppKey );
	if( _tcslen( szBuff ) < dwSize )
		_tcscpy( lpszRegKey, szBuff );
	else 
		result = Tcs::rOutOfBuffer;

	return ( result );
}

//	���t������(YYYYMMDD)���擾����
void tcGetDayString( 
	LPTSTR lpszString,	//(o)���t������
	const CTime& time	//(i)����
	)
{
	_stprintf( lpszString, _T("%04d%02d%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );
}
