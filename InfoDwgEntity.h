// InfoDwgEntity.h: InfoDwgEntity �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFODWGENTITY_H__54CE6E21_08BB_47EA_A0C2_5075093BFC59__INCLUDED_)
#define AFX_INFODWGENTITY_H__54CE6E21_08BB_47EA_A0C2_5075093BFC59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"

#define NUM_FIELD_STR    6
#define NUM_FIELD_DOUBLE 1

class InfoDwgEntity  
{
public:
	InfoDwgEntity();
	virtual ~InfoDwgEntity();
	
// ������^�̃t�B�[���h�z��̃C���f�b�N�X
	enum IDE_FIELD_STRING {
		ENTTYPE,
		COLOR,		
		LAYER,		
		LINETYPE,		
		PLOTSTYLE,		
		LINEWEIGHT,		
	};

	// �{���������_�^�̃t�B�[���h�z��̃C���f�b�N�X
	enum IDE_FIELD_DOUBLE {
		LINETYPESCALE,		
	};

	// �z��̃T�C�Y���Z�b�g����B
	void SetSize(int iSize);

	// �z��ɒl���Z�b�g����B
	// ������^
	void SetValue(IDE_FIELD_STRING eFieldIdx, int iIdx, CString strValue);
	// �{���������_�^
	void SetValue(IDE_FIELD_DOUBLE eFieldIdx, int iIdx, double dValue);

	// �S�v�f
	void SetValue(int iIdx, AcDbObjectId idEnt, AcDbHandle hdlEnt, CString strColor, CString strLayer,
		CString strLinetype, CString strPlotStyle, CString strLineWeight, double dLinetypeScale);

	// �z��𓾂�B
	CStringArray		   &GetStringArray(IDE_FIELD_STRING eFieldIdx);
	CArray<double, double> &GetDoubleArray(IDE_FIELD_DOUBLE eFieldIdx);	

	// �}�`ID�̗�
	AcDbObjectIdArray      m_idaryEnt;
	// �}�`�n���h���̗�
	AcArray<AcDbHandle>    m_hdlaryEnt;
private:	
				
	CStringArray		   m_straryAttr[NUM_FIELD_STR];
	CArray<double, double> m_daryAttr[NUM_FIELD_DOUBLE];	

};

#endif // !defined(AFX_INFODWGENTITY_H__54CE6E21_08BB_47EA_A0C2_5075093BFC59__INCLUDED_)
