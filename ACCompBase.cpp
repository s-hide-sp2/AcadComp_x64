// ACCompBase.cpp: ACCompBase �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ACCompBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

double ACCompBase::m_dTol = 0.0001;     // �}�`��r���̋��e�덷
AcGeTol ACCompBase::m_geTol; 
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

ACCompBase::ACCompBase()
{
	m_vec3BasePointGap.set(0.0, 0.0, 0.0);
	m_vec3BasePointGapApply.set(0.0, 0.0, 0.0);
}

ACCompBase::~ACCompBase()
{

}

// �}�`��r���̋��e�덷���Z�b�g����B
void ACCompBase::SetTol(double dTol)
{
	m_dTol = dTol;	
	m_geTol.setEqualPoint(dTol);
	m_geTol.setEqualVector(dTol);
}

// �w�肵��2�̒l�����e�덷�͈͓̔��œ��������ǂ�����Ԃ��B
bool ACCompBase::IsEqual(const double &dVal1, const double &dVal2)
{
	return (fabs(dVal1 - dVal2) <= m_dTol);
}

// �w�肵��2��2d�|�C���g�����e�덷�͈͓̔��œ��������ǂ�����Ԃ��B
bool ACCompBase::IsEqual(const AcGePoint2d &pt2_1, const AcGePoint2d &pt2_2)
{
	return IsEqual((
		pt2_1 + AcGeVector2d(m_vec3BasePointGapApply[X], m_vec3BasePointGapApply[Y])).distanceTo(pt2_2), 
		0.0);

	//return (IsEqual(pt2_1[0], pt2_2[0]) && 
	//		IsEqual(pt2_1[1], pt2_2[1]));
} 

// �w�肵��2��3d�|�C���g�����e�덷�͈͓̔��œ��������ǂ�����Ԃ��B 
// ��r�͈͂̊�_�̂�����l�����Ȃ��^�C�v
bool ACCompBase::IsEqualNoGap(const AcGePoint3d &pt3_1, const AcGePoint3d &pt3_2)
{	
	return IsEqual(pt3_1.distanceTo(pt3_2), 0.0);
}

// �w�肵��2��3d�|�C���g�����e�덷�͈͓̔��œ��������ǂ�����Ԃ��B 
bool ACCompBase::IsEqual(const AcGePoint3d &pt3_1, const AcGePoint3d &pt3_2)
{
	// �͈͔�r�̂Ƃ��͔�r��_�̂���̕␳���v�Z����B
	return IsEqual((pt3_1 + m_vec3BasePointGapApply).distanceTo(pt3_2), 0.0);

	//return (IsEqual(pt3_1[0], pt3_2[0]) && 
	//		IsEqual(pt3_1[1], pt3_2[1]) && 
	//		IsEqual(pt3_1[2], pt3_2[2])); 
}

// �w�肵��2��3d�x�N�g�������e�덷�͈͓̔��œ��������ǂ�����Ԃ��B
bool ACCompBase::IsEqual(const AcGeVector3d &vec3_1, const AcGeVector3d &vec3_2)
{
	return IsEqual((vec3_1 - vec3_2).length(), 0.0);

	//return (IsEqual(vec3_1[0], vec3_2[0]) && 
	//		IsEqual(vec3_1[1], vec3_2[1]) && 
	//		IsEqual(vec3_1[2], vec3_2[2])); 
} 

// �w�肵��2��3d�|�C���g�z�񂪋��e�덷�͈͓̔��œ��������ǂ�����Ԃ��B
// �z�񐔂��قȂ�ꍇ��false
bool ACCompBase::IsEqual(const AcGePoint3dArray &pt3ary1, const AcGePoint3dArray &pt3ary2) 
{
	if (pt3ary1.logicalLength() != pt3ary2.logicalLength())
		return false;

	for (int i = 0; i < pt3ary1.logicalLength(); i++) {
		if (!IsEqual(pt3ary1[i], pt3ary2[i]))
			return false;
	}
		
	return true;	
}

// �w�肵��2��double�z�񂪋��e�덷�͈̔͂œ��������ǂ�����Ԃ��B
// �z�񐔂��قȂ�ꍇ��false��Ԃ��B
bool ACCompBase::IsEqual(const AcGeDoubleArray &dary1, const AcGeDoubleArray &dary2)
{
	if (dary1.logicalLength() != dary2.logicalLength())
		return false;

	for (int i = 0; i < dary1.logicalLength(); i++) {
		if (!IsEqual(dary1[i], dary2[i]))
			return false;
	}

	return true;
} 

// �w�肵��2�̕��ʂ����e�덷�͈̔͂œ��������ǂ�����Ԃ��B	
bool ACCompBase::IsEqual(const AcGePlane &dpln1, const AcGePlane &dpln2)
{
	AcGePoint3d  pt3Org1, pt3Org2;
	AcGeVector3d vec3Axis1_1, vec3Axis1_2, vec3Axis2_1, vec3Axis2_2; 	
	
	dpln1.getCoordSystem(pt3Org1, vec3Axis1_1, vec3Axis2_1);
	dpln2.getCoordSystem(pt3Org2, vec3Axis1_2, vec3Axis2_2);
	
	return (
		IsEqual(pt3Org1,	 pt3Org2) && 
		IsEqual(vec3Axis1_1, vec3Axis1_2) &&
		IsEqual(vec3Axis2_1, vec3Axis2_2));
} 

// �w�肵��2��2d�x�N�g�������e�덷�͈̔͂œ��������ǂ�����Ԃ��B
bool ACCompBase::IsEqual(const AcGeVector2d &vec2_1, const AcGeVector2d &vec2_2)
{
	return IsEqual((vec2_1 - vec2_2).length(), 0.0);

	//return (IsEqual(vec2_1[0], vec2_2[0]) && 
	//		IsEqual(vec2_1[1], vec2_2[1]));
} 

// �w�肵��2��3d�X�P�[�������e�덷�͈̔͂œ��������ǂ�����Ԃ��B
bool ACCompBase::IsEqual(const AcGeScale3d &scl3_1, const AcGeScale3d &scl3_2)
{
	AcGePoint3d pt3_1, pt3_2;
	
	pt3_1.set(scl3_1[0], scl3_1[1], scl3_1[2]);
	pt3_2.set(scl3_2[0], scl3_2[1], scl3_2[2]);

	return IsEqual(pt3_1.distanceTo(pt3_2), 0.0);

	//return (IsEqual(scl3_1[0], scl3_2[0]) &&
	//		IsEqual(scl3_1[1], scl3_2[1]) &&
	//		IsEqual(scl3_1[2], scl3_2[2]));
}

//AcGeTol �̔�r
bool ACCompBase::IsEqual(const AcGeTol &tol1, const AcGeTol &tol2)
{
	return (
		IsEqual(tol1.equalPoint(),  tol2.equalPoint()) &&
		IsEqual(tol1.equalVector(), tol2.equalVector())); 
} 

/*
//AcGeCircArc2d �̔�r
bool ACCompBase::IsEqual(const AcGeCircArc2d &cArc1, const AcGeCircArc2d &cArc2)
{
	return (cArc1.isEqualTo(cArc2, m_geTol) == Adesk::kTrue);		
}

//AcGeEllipArc2d �̔�r
bool ACCompBase::IsEqual(const AcGeEllipArc2d &eArc1, const AcGeEllipArc2d &eArc2)
{
	return (eArc1.isEqualTo(eArc2, m_geTol) == Adesk::kTrue);		
} 

//AcGeLineSeg2d �̔�r
bool ACCompBase::IsEqual(const AcGeLineSeg2d &lineSeg1, const AcGeLineSeg2d &lineSeg2)
{
	return (lineSeg1.isEqualTo(lineSeg2, m_geTol) == Adesk::kTrue);		
} 

//AcGeNurbCurve2d �̔�r
bool ACCompBase::IsEqual(const AcGeNurbCurve2d &nCurve1, const AcGeNurbCurve2d &nCurve2)
{
	return (nCurve1.isEqualTo(nCurve2, m_geTol) == Adesk::kTrue);		
} 
*/

// ---- �ȉ��͋��e�덷���l�����Ȃ���r ----
// Adesk::Boolean�^�̔�r
/*
bool ACCompBase::IsEqual(const bool ab1, const bool ab2)
{
	return (ab1 == ab2);
}
*/

// AcCmColor�^�̔�r
bool ACCompBase::IsEqual(const AcCmColor &clr1, const AcCmColor &clr2)
{
	return (clr1 == clr2);
}

// int�^�̔�r
bool ACCompBase::IsEqual(const int &i1, const int &i2)
{
	return (i1 == i2);
} 

// AcDb::LineWeight�^�̔�r
bool ACCompBase::IsEqual(const AcDb::LineWeight lw1, const AcDb::LineWeight lw2)
{
	return (lw1 == lw2);
} 

// AcDb::Vertex2dType�^�̔�r
bool ACCompBase::IsEqual(const AcDb::Vertex2dType &vtx2Type1, const AcDb::Vertex2dType &vtx2Type2)
{
	return (vtx2Type1 == vtx2Type2);
}

// FlowDirection�^�̔�r
bool ACCompBase::IsEqual(const AcDbMText::FlowDirection &fDir1, const AcDbMText::FlowDirection &fDir2)
{
	return (fDir1 == fDir2);
}

// AcDb::LineSpacingStyle�^�̔�r
bool ACCompBase::IsEqual(const AcDb::LineSpacingStyle &lineSpcStl1, const AcDb::LineSpacingStyle &lineSpcStl2)
{
	return (lineSpcStl1 == lineSpcStl2);
}

// bool�^�̔�r
bool ACCompBase::IsEqual(const bool &b1, const bool &b2)
{
	return (b1 == b2);
}

// StandardScaleType�^�̔�r
bool ACCompBase::IsEqual(const AcDbViewport::StandardScaleType &ssclType1, const AcDbViewport::StandardScaleType &ssclType2)
{
	return (ssclType1 == ssclType2);
}

// Adesk::Int16�^�̔�r
bool ACCompBase::IsEqual(const Adesk::Int16 &aint1, const Adesk::Int16 &aint2)
{
	return (aint1 == aint2);
}

// ������̔�r������B
// �����񂪈قȂ�ꍇ�ƁA�ǂ��炩����݂̂�NULL�ł���Ƃ���false��Ԃ��B
bool ACCompBase::IsEqualStr(const TCHAR *pcStr1, const TCHAR *pcStr2) //2006.07.20
{
	bool bRet = true;

	if (pcStr1 != NULL && pcStr2 != NULL) {
		if (_tcscmp(pcStr1, pcStr2) != 0)
			bRet = false;
	}
	else if ( !(pcStr1 == NULL && pcStr2 == NULL) )
			bRet = false;	

	return bRet;		
} 

// �͈͔�r���[�h�̂Ƃ��A��r��_�̂�����Z�b�g����B
void ACCompBase::SetBasePointGap(const AcGeVector3d &vec3BasePointGap)
{
	m_vec3BasePointGap.set(vec3BasePointGap.x, vec3BasePointGap.y, vec3BasePointGap.z);
}
