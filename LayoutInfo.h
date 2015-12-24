// LayoutInfo.h: LayoutInfo �N���X�̃C���^�[�t�F�C�X
// ���C�A�E�g�Ɗ֘A�t�����u���b�N�e�[�u�����R�[�h�̏���ێ�����B
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYOUTINFO_H__A30FA471_E9B2_4C80_85AB_441776D5AC49__INCLUDED_)
#define AFX_LAYOUTINFO_H__A30FA471_E9B2_4C80_85AB_441776D5AC49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class LayoutInfo  
{
public:
	LayoutInfo();
	virtual ~LayoutInfo();
	
	void SetInfo(LPCTSTR strName, AcDbObjectId idBlkRec);

	LPCTSTR GetLayoutName();
	AcDbObjectId &GetBlockTableRecordId();

private:
	AcDb m_strLayoutName;
	AcDbObjectId m_idBlkRec;
};

#endif // !defined(AFX_LAYOUTINFO_H__A30FA471_E9B2_4C80_85AB_441776D5AC49__INCLUDED_)
