// ACDbSysVar.h: ACDbSysVar �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACDBSYSVAR_H__9764701F_BDE8_4FD7_BA60_27DD7B14D5FF__INCLUDED_)
#define AFX_ACDBSYSVAR_H__9764701F_BDE8_4FD7_BA60_27DD7B14D5FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

class ACDbSysVar  
{
public:
	ACDbSysVar();
	virtual ~ACDbSysVar();
	
	// �V�X�e���ϐ����擾�^�ݒ肷��f�[�^�x�[�X���w�肷��B
	void SetDatabase(AcDbDatabase *pDb);

	// limmax�𓾂�B
	bool Getlimmax(AcGePoint2d &pt2Ret);

	// limmin�𓾂�B
	bool Getlimmin(AcGePoint2d &pt2Ret);

	
private: 
	AcDbDatabase *m_pDb;
};

#endif // !defined(AFX_ACDBSYSVAR_H__9764701F_BDE8_4FD7_BA60_27DD7B14D5FF__INCLUDED_)
