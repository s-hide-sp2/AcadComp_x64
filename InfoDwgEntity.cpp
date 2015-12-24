// InfoDwgEntity.cpp: InfoDwgEntity �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "InfoDwgEntity.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

InfoDwgEntity::InfoDwgEntity()
{

}

InfoDwgEntity::~InfoDwgEntity()
{

}

void InfoDwgEntity::SetSize(int iSize)
{
	int i;

	for (i = 0; i < NUM_FIELD_STR; i++) {
		m_straryAttr[i].SetSize(iSize);
	}

	for (i = 0; i < NUM_FIELD_DOUBLE; i++) {
		m_daryAttr[i].SetSize(iSize);
	}

	m_idaryEnt.setLogicalLength(iSize);	
	m_hdlaryEnt.setLogicalLength(iSize);
}

CStringArray &InfoDwgEntity::GetStringArray(IDE_FIELD_STRING eFieldIdx)
{
	return m_straryAttr[eFieldIdx];
}

CArray<double, double> &InfoDwgEntity::GetDoubleArray(IDE_FIELD_DOUBLE eFieldIdx)
{
	return m_daryAttr[eFieldIdx];
}

// �z��ɒl���Z�b�g����B ������^
void InfoDwgEntity::SetValue(IDE_FIELD_STRING eFieldIdx, int iIdx, CString strValue)
{	
	m_straryAttr[eFieldIdx][iIdx] = strValue;
}

// �z��ɒl���Z�b�g����B�{���������_�^
void InfoDwgEntity::SetValue(IDE_FIELD_DOUBLE eFieldIdx, int iIdx, double dValue)
{
	m_daryAttr[eFieldIdx][iIdx] = dValue;
}

// �S�v�f
void InfoDwgEntity::SetValue(int iIdx, AcDbObjectId idEnt, AcDbHandle hdlEnt, CString strColor, CString strLayer,
	CString strLinetype, CString strPlotStyle, CString strLineWeight, double dLinetypeScale)
{
	m_idaryEnt[iIdx]  = idEnt;
	m_hdlaryEnt[iIdx] = hdlEnt;
	m_straryAttr[IDE_FIELD_STRING::COLOR][iIdx] = strColor;
	m_straryAttr[IDE_FIELD_STRING::LAYER][iIdx] = strLayer;
	m_straryAttr[IDE_FIELD_STRING::LINETYPE][iIdx] = strLinetype;
	m_straryAttr[IDE_FIELD_STRING::PLOTSTYLE][iIdx] = strPlotStyle;
	m_straryAttr[IDE_FIELD_STRING::LINEWEIGHT][iIdx] = strLineWeight;
	m_daryAttr[IDE_FIELD_DOUBLE::LINETYPESCALE][iIdx] = dLinetypeScale;
}
