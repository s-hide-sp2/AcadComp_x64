// ACString.h: ACString �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACSTRING_H__86A415AD_82AD_484C_A8CB_98CE54704E70__INCLUDED_)
#define AFX_ACSTRING_H__86A415AD_82AD_484C_A8CB_98CE54704E70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ACString  
{
public:
	ACString();
	virtual ~ACString();

	static bool    StringToNumber(CString strSrc, double &dRet);
	static CString NumberToString(double dSrc);

	// �w�肵����������w�肵�������ŋ�؂����O��̕������Ԃ��B
	static CString GetSegmentR(CString strSrc, char cSeparator, bool bRetAfter, bool bInclude);

	// CSV�`���̃t�@�C����1�s���󂯎��A�v�f�ɕ�������CStringArray�ŕԂ��B""������ꍇ�͎�菜���B
	static void SplitLine(CString strSrc, CStringArray& aryRetElm);

};

#endif // !defined(AFX_ACSTRING_H__86A415AD_82AD_484C_A8CB_98CE54704E70__INCLUDED_)
