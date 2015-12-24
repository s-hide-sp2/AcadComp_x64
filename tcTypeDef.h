#pragma once

#ifdef _IMPORT_TCS
	#define TCS_EXPORT_CLASS	__declspec( dllimport )
#else
	#define TCS_EXPORT_CLASS	__declspec( dllexport )
#endif

/////////////////////////////////////////////////////////////
//
//	���W�X�g��


namespace Tcs{
	enum Result{
		rOk,					//	OK
		rFail,					//	���s
		rOverUseSpan,			//	�g�p���ԃI�[�o�[
		rInvalidRegistryInfo,	//	���W�X�g����񖳌�
		rFailToAccessRegistry,	//	���W�X�g���A�N�Z�X���s
		rOutOfBuffer			//	�o�b�t�@�s��
	};
};

/////////////////////////////////////////////////////////////
//
//	���W�X�g��

#define TCS_REG_KEY_INSTALL_INFO	_T("CLSID\\{124E9F44-658E-4bf5-BE5A-C57BCAFAE8A0}")	//	�C���X�g�[�����i�[�L�[
#define TCS_REG_DATA_INSTALL_DAY	_T("InstallDay")			//	�C���X�g�[�����t
#define TCS_REG_DATA_ACCESS_DAY		_T("AccessDay")				//	�ŏI�A�N�Z�X���t
#define TCS_REG_DATA_USE_SPAN		_T("UseSpan")				//	�g�p����
