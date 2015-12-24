// ResultRecord.h: ResultRecord �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESULTRECORD_H__C2EEBA64_DF07_49F8_811F_F3B75A03C747__INCLUDED_)
#define AFX_RESULTRECORD_H__C2EEBA64_DF07_49F8_811F_F3B75A03C747__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"

class ResultRecord  
{
public:
	ResultRecord();
	virtual ~ResultRecord();
	
public:
	// 2�̐}�`���ʂ̃n���h��(�ǂ��炩�Е��̏ꍇ������B)
	AcDbHandle m_hdlEnts;
	// ���ꂼ��̃I�u�W�F�N�gID
	AcDbObjectId m_idObj1;
	AcDbObjectId m_idObj2;	


	/*
	// �F(���O�̏ꍇ�ƃC���f�b�N�X�̏ꍇ������B)
	CString m_strColor1;
	CString m_strColor2;
	// ���C���[
	CString m_strLayer1;
	CString m_strLayer2;
	// ����
	CString m_strLinetype1;
	CString m_strLinetype2;
	// ����X�^�C��
	CString m_strPlotstyleName1;
	CString m_strPlotstyleName2;
	// ���̑���(Bylayer�Ȃǂ̏ꍇ������B)
	CString m_strLineWeight1;	
	CString m_strLineWeight2;	
	// ����ړx
	double  m_dLinetypeScale1;
	double  m_dLinetypeScale2;
	// �W�I���g���ɈႢ������ꍇtrue
	bool    m_bDiffGeom; 
    // �F�ɈႢ������ꍇtrue
	bool    m_bDiffColor;
	// ���C���[�ɈႢ������ꍇtrue
	bool    m_bDiffLayer;
	// ����ɈႢ������ꍇtrue
	bool    m_bDiffLinetype;
	// ����X�^�C���ɈႢ������ꍇtrue
	bool	m_bDiffPlotstyleName;
	// ����ړx�ɈႢ������ꍇtrue
	bool	m_bLinetypeScale;
	// ���̑����ɈႢ������ꍇtrue
	bool	m_bLineWeight;
	*/
private:
	CString m_strAttr[10];

};

#endif // !defined(AFX_RESULTRECORD_H__C2EEBA64_DF07_49F8_811F_F3B75A03C747__INCLUDED_)
