// ACCompBase.h: ACCompBase �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCOMPBASE_H__BF30EC2F_9D90_40B6_9A68_8685BFF57FE0__INCLUDED_)
#define AFX_ACCOMPBASE_H__BF30EC2F_9D90_40B6_9A68_8685BFF57FE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"

class ACCompBase  
{
public:
	ACCompBase();
	virtual ~ACCompBase();
	
	// �}�`��r���̋��e�덷���Z�b�g����B
	static void SetTol(double dTol);

	// �}�`��r���̋��e�덷�𓾂�B
	//static void GetTol(double dTol);

public: //protected:
	// �w�肵��2�̒l�����e�덷�͈͓̔��œ��������ǂ�����Ԃ��B
	bool IsEqual(const double &dVal1, const double &dVal2);
	
	// �w�肵��2��2d�|�C���g�����e�덷�͈͓̔��œ��������ǂ�����Ԃ��B
	bool IsEqual(const AcGePoint2d &pt2_1, const AcGePoint2d &pt2_2); 

	// �w�肵��2��3d�|�C���g�����e�덷�͈͓̔��œ��������ǂ�����Ԃ��B
	// ��r�͈͂̊�_�̂�����l�����Ȃ��^�C�v
	bool IsEqualNoGap(const AcGePoint3d &pt3_1, const AcGePoint3d &pt3_2);

	// �w�肵��2��3d�|�C���g�����e�덷�͈͓̔��œ��������ǂ�����Ԃ��B
	bool IsEqual(const AcGePoint3d &pt3_1, const AcGePoint3d &pt3_2); 
	
	// �w�肵��2��2d�x�N�g�������e�덷�͈͓̔��œ��������ǂ�����Ԃ��B
	bool IsEqual(const AcGeVector3d &vec3_1, const AcGeVector3d &vec3_2); 

	// �w�肵��2��3d�|�C���g�z�񂪋��e�덷�͈͓̔��œ��������ǂ�����Ԃ��B
	// �z�񐔂��قȂ�ꍇ��false��Ԃ��B
	bool IsEqual(const AcGePoint3dArray &pt3ary1, const AcGePoint3dArray &pt3ary2); 
	
	// �w�肵��2��double�z�񂪋��e�덷�͈̔͂œ��������ǂ�����Ԃ��B
	// �z�񐔂��قȂ�ꍇ��false��Ԃ��B
	bool IsEqual(const AcGeDoubleArray &dary1, const AcGeDoubleArray &dary2); 

	// �w�肵��2�̕��ʂ����e�덷�͈̔͂œ��������ǂ�����Ԃ��B	
	bool IsEqual(const AcGePlane &dpln1, const AcGePlane &dpln2); 

	// �w�肵��2��2d�x�N�g�������e�덷�͈̔͂œ��������ǂ�����Ԃ��B
	bool IsEqual(const AcGeVector2d &vec2_1, const AcGeVector2d &vec2_2); 
	
	// �w�肵��2��3d�X�P�[�������e�덷�͈̔͂œ��������ǂ�����Ԃ��B
	bool IsEqual(const AcGeScale3d &scl3_1, const AcGeScale3d &scl3_2);

	// AcGe�N���X�̔�r
	//AcGeCircArc2d �̔�r
	//bool IsEqual(const AcGeCircArc2d &cArc1, const AcGeCircArc2d &cArc2);

	//AcGeEllipArc2d �̔�r
	//bool IsEqual(const AcGeEllipArc2d &eArc1, const AcGeEllipArc2d &eArc2);	 

	//AcGeLineSeg2d �̔�r
	//bool IsEqual(const AcGeLineSeg2d &lineSeg1, const AcGeLineSeg2d &lineSeg2);
	 
	//AcGeNurbCurve2d �̔�r
	//bool IsEqual(const AcGeNurbCurve2d &nCurve1, const AcGeNurbCurve2d &nCurve2);


	


	// ---- �ȉ��͋��e�덷���l�����Ȃ���r ----

	//bool IsEqual(const bool ab1, const bool ab2);

	// Adesk::Boolean�^�̔�r
	//bool IsEqual(const Adesk::Boolean ab1, const Adesk::Boolean ab2);

	// AcCmColor�^�̔�r
	bool IsEqual(const AcCmColor &clr1, const AcCmColor &clr2);

	// int�^�̔�r
	bool IsEqual(const int &i1, const int &i2); 

	// AcDb::LineWeight�^�̔�r
	bool IsEqual(const AcDb::LineWeight lw1, const AcDb::LineWeight lw2); 
	
	// AcDb::Vertex2dType�^�̔�r
	bool IsEqual(const AcDb::Vertex2dType &vtx2Type1, const AcDb::Vertex2dType &vtx2Type2);
	
	// FlowDirection�^�̔�r
	bool IsEqual(const AcDbMText::FlowDirection &fDir1, const AcDbMText::FlowDirection &fDir2);
	
	// AcDb::LineSpacingStyle�^�̔�r
	bool IsEqual(const AcDb::LineSpacingStyle &lineSpcStl1, const AcDb::LineSpacingStyle &lineSpcStl2);

	// bool�^�̔�r
	bool IsEqual(const bool &b1, const bool &b2);
	
	// StandardScaleType�^�̔�r
	bool IsEqual(const AcDbViewport::StandardScaleType &ssclType1, const AcDbViewport::StandardScaleType &ssclType2);
	
	// Adesk::Int16�^�̔�r
	bool IsEqual(const Adesk::Int16 &aint1, const Adesk::Int16 &aint2);

	//AcGeTol �̔�r
	bool IsEqual(const AcGeTol &tol1, const AcGeTol &tol2);

public:
	void SetBasePointGap(const AcGeVector3d &vec3BasePointGap);
	// ������̔�r������B
	bool IsEqualStr(const TCHAR *pcStr1, const TCHAR *pcStr2); 

private:
	// �}�`��r���̋��e�덷
	static double  m_dTol; 
	static AcGeTol m_geTol; 

protected:
	// �}��1�̔�r��_���W����}��2�̔�r��_���W�ւ̃x�N�g���B�͈͔�r���[�h���̂ݐݒ肷��B
	AcGeVector3d m_vec3BasePointGap;      // �͈͔�r���[�h�Őݒ肳�ꂽ�M���b�v
	AcGeVector3d m_vec3BasePointGapApply; // ���ۂɔ�r���ɓK�p����M���b�v
};

#endif // !defined(AFX_ACCOMPBASE_H__BF30EC2F_9D90_40B6_9A68_8685BFF57FE0__INCLUDED_)
