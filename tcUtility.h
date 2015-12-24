#pragma once

#include "tcTypeDef.h"

//	�C���X�g�[���������W�X�g���ɓo�^����
extern "C" long __stdcall tcRegInstallInfo( LPCTSTR lpszAppKey, long lUseTerm, bool bOverWrite = false );

//	�A�N�Z�X�����X�V����
extern "C" long __stdcall tcUpdateAccessDay( LPCTSTR lpszAppKey );

//	�g�p���Ԃ��`�F�b�N����
extern "C" long __stdcall tcCheckUseTerm( LPCTSTR lpszAppKey );


//	���W�X�g���L�[���擾����
Tcs::Result tcGetRegKey( LPTSTR lpszRegKey, LPCTSTR lpszAppKey, DWORD dwSize );

//	���t������(YYYYMMDD)���擾����
void tcGetDayString( LPTSTR lpszString, const CTime& time );