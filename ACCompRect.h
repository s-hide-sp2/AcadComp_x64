// ACCompRect.h: ACCompRect �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCOMPRECT_H__4651332F_85EB_4C0E_896D_1884060B0BDD__INCLUDED_)
#define AFX_ACCOMPRECT_H__4651332F_85EB_4C0E_896D_1884060B0BDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ACDatabaseUtility.h"
//2006.07.20
#define  APPNAME _T("COMPDWG")
#define  OBJNAME _T("COMPRECT")


class ACCompRect  
{
public:
	// ��r�͈͂̋�`��ObjectId��Ԃ��B�������݂����ꍇ�́A�ŏ���ObjectId��Ԃ��B
	// �߂�l�Ŕ�r�͈͂̋�`�̌���Ԃ��B
	long GetCompRectId(AcDbObjectId &idCompRect);  
	void DeleteCompRect();
	void Test02();
	void Test01();
	void ShowName();
	// ���[�U�[��2�_�̓��͂����߁A��r�͈͂̋�`��`�悷��BXData��ݒ肷��B	
	bool DrawCompRect();
	ACCompRect();
	virtual ~ACCompRect();

};

#endif // !defined(AFX_ACCOMPRECT_H__4651332F_85EB_4C0E_896D_1884060B0BDD__INCLUDED_)
