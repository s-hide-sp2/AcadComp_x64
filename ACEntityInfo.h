// ACEntityInfo.h: ACEntityInfo �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACENTITYINFO_H__07D9B97F_8F82_45C2_9FF8_83AC34F68573__INCLUDED_)
#define AFX_ACENTITYINFO_H__07D9B97F_8F82_45C2_9FF8_83AC34F68573__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define ACE_DXF_HYPERLINK 1000  // �n�C�p�[�����N�����߂��Ă���DXF�R�[�h
#define ACE_DXF_HYPERLINK 1002  //kDxfXdControlString

class ACEntityInfo  
{
public:
	ACEntityInfo();
	virtual ~ACEntityInfo();
	
	// �w�肵���G���e�B�e�B�̃n�C�p�[�����N�̒l�𓾂�B
	// �n�C�p�[�����N���ݒ肳��Ă��Ȃ��ꍇ��NULL��Ԃ��B
	static CString GetHyperLink(AcDbEntity *pEnt);
};

#endif // !defined(AFX_ACENTITYINFO_H__07D9B97F_8F82_45C2_9FF8_83AC34F68573__INCLUDED_)
