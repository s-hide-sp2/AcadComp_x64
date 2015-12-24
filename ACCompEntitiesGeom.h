// ACCompEntitiesGeom.h: ACCompEntitiesGeom �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCOMPENTITIESGEOM_H__25291CD7_5C15_498A_BE88_757CCAB26930__INCLUDED_)
#define AFX_ACCOMPENTITIESGEOM_H__25291CD7_5C15_498A_BE88_757CCAB26930__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "ACCompBase.h"
#include "ACEntityInfo.h"

class ACCompEntitiesGeom : public ACCompBase
{
public:
	bool CompareEntityGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2, bool bApplyGap);
	// ��r���ʂ̃^�C�v
	enum CE_TYPE_RET {
		INVALID,
		EQUAL,
		NOTEQUAL,
		ONLY1,
		ONLY2,
		UNKNOWN
	};


	ACCompEntitiesGeom();
	virtual ~ACCompEntitiesGeom();

	

	// �}�`��r���̋��e�덷���Z�b�g����B
	//void SetTol(double dTol);

	// �w�肵��2�̐}�`�̈�ʑ��������������ǂ������ׂ�B
	bool IsEqualGeneralAttrs(AcDbEntity *pEnt1, AcDbEntity *pEnt2);

private:	 
	// �w�肳�ꂽ�G���e�B�e�B�̃W�I���g���̈Ⴂ�𒲂ׂ�B
	// pEnt1, pEnt2 ��Acad::kForRead�ŊJ����Ă��邱��
	// ���[�U�[�h���N���X���͔�r��������true��Ԃ��B
	// bApplyGap: ��r��_�̂�����l�����Ĕ�r���邩�ǂ������w�肷��B
	// ��r�͈̓��[�h�ł��A�u���b�N�Q�Ƃ�A���[�W�����̍\���v�f�̔�r�̏ꍇ�͔�r��_�̂�����l������K�v���Ȃ����߁B
	bool CompareEntityGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);


	// ---- �J�[�u�h���N���X�̔�r ---- //
	// �w�肳�ꂽ2�{�̃��C�����}�`�I�ɓ��������ǂ������ׂ�B
	bool CompareLineGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	

	// 2d�|�����C���̔�r	
	bool Compare2dPolylineGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	

	// 3d�|�����C���̔�r		
	bool Compare3dPolylineGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	

	// �~�ʂ̔�r	
	bool CompareArcGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	

	// �~�̔�r		
	bool CompareCircleGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	

	// �ȉ~�̔�r	
	bool CompareEllipseGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	

	// ���o�����̔�r	
	bool CompareLeaderGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	

	// �|�����C���̔�r	
	bool ComparePolylineGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	

	// Ray�̔�r		
	bool CompareRayGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	

	// Spline�̔�r	
	bool CompareSplineGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	

	// Xline�̔�r		
	bool CompareXlineGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	

	// Vettex
	// 2d���_���r����B
	bool Compare2dVertexGeoms(AcDb2dVertex *p2dVtx1, AcDb2dVertex *p2dVtx2);

	// 3d���_���r����B
	bool Compare3dPolylineVertexGeoms(AcDb3dPolylineVertex *p3dVtx1, AcDb3dPolylineVertex *p3dVtx2);


	//---- ���@���h���N���X�̔�r ----//
	// Dimension ���N���X�̑����̔�r���s���B
	bool CompareDimensionGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	

	// 2LineAngularDimension
	bool Compare2LineAngularDimensionGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	

	// 3PointAngularDimension
	bool Compare3PointAngularDimensionGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	

	// AlignedDimension
	bool CompareAlignedDimensionGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	

	// DiametricDimension
	bool CompareDiametricDimensionGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	

	// OrdinateDimension
	bool CompareOrdinateDimensionGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	

	// RadialDimension
	bool CompareRadialDimensionGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	

	// RotatedDimension
	bool CompareRotatedDimensionGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	


	// ---- ���̑�AcDbEntity�h���N���X�̔�r ---- //
	// �u���b�N�Q��			
	bool CompareBlockReferenceGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);

	// MInsertBlock?		
	bool CompareMInsertBlockGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
			
	// Face	��r�ΏۊO
	bool CompareFaceGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
	
	// FaceRecord ��r�ΏۊO
	bool CompareFaceRecordGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
	
	// Fcf?	
	bool CompareFcfGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
	
	// ??? Frame�͒��ۃN���X��? �� ���ۃN���X	
	//bool CompareFrameGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);

	// OleFrame ��r�ΏۊO
	bool CompareOleFrameGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);

	// Ole2Frame ��r�ΏۊO	
	bool CompareOle2FrameGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
	
	// �n�b�`���O	
	bool CompareHatchGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
			
	// ??? Image�͒��ۃN���X��? �� ���ۃN���X
	//bool CompareImageGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);

	// RasterImage		
	bool CompareRasterImageGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);

	// Mline	
	bool CompareMlineGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
		
	// MText	
	bool CompareMTextGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
	
	// Point	
	bool ComparePointGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
	
	// PolyFaceMesh	��r�ΏۊO
	bool ComparePolyFaceMeshGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
	
	// PolygonMesh ��r�ΏۊO
	bool ComparePolygonMeshGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
	
	// ProxyEntity ��r�ΏۊO
	bool CompareProxyEntityGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
	
	// Shape	
	bool CompareShapeGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
	
	// Solid	
	bool CompareSolidGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
		
	// �e�L�X�g	
	bool CompareTextGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);

	// ����	
	bool CompareAttributeGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);

	// ������`	
	bool CompareAttributeDefinitionGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
	
	// Trace	
	bool CompareTraceGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
	
	// Viewport	
	bool CompareViewportGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
	
	// 3dSolid ��r�ΏۊO
	bool Compare3dSolidGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
	
	// Body	��r�ΏۊO 
	bool CompareBodyGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
	
	// Region	
	bool CompareRegionGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);


	// AcDbVertex�h���N���X
	// AcDbVertex	
	bool CompareVertexGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
	// AcDb2dVertex	
	bool Compare2dVertexGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
	// AcDb3dPolylineVertex	
	bool Compare3dPolylineVertexGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
	// AcDbPolyFaceMeshVertex	
	bool ComparePolyFaceMeshVertexGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
	// AcDbPolygonMeshVertex	
	bool ComparePolygonMeshVertexGeoms(AcDbEntity *pEnt1, AcDbEntity *pEnt2);
	
	// AcDbBlockTableRecord�̃T�u�G���e�B�e�B���r����B
	// pBlkRec1, pBlkRec2 ��Acad::kForRead�̏�ԂŊJ����Ă��邱�ƁB
	bool CompareBlockRecSubEntityGeoms(
		AcDbBlockTableRecord *pBlkRec1, 
		AcDbBlockTableRecord *pBlkRec2); 	
	
	// �w�肳�ꂽID��BlockTableRecord���J���A���̖��O���r����B
	// �J�����Ƃ��̌��ʂ�eOk�ŁA���O�������ꍇ��bRetOpenOk=true�ŁAtrue��Ԃ��B
	// �J�����Ƃ��̌��ʂ�eOk�ȊO�̂Ƃ��́AbRetOpenOk=false�ŁA���ʂ��������ǂ�����߂�l�ŕԂ��B
	bool CompareBlockTableRecord(const AcDbObjectId &idRec1, const AcDbObjectId &idRec2, bool &bRetOpenOk);

	// ���@�X�^�C���̔�r
	// ���@�X�^�C�������݂��A���̖��O���قȂ�ꍇ��false��Ԃ��B
	bool CompareDimStyle(const AcDbObjectId &idDimStl1, const AcDbObjectId &idDimStl2);

	// �e�L�X�g�X�^�C���̔�r
	// �e�L�X�g�X�^�C�������݂��A���̖��O���قȂ�ꍇ��false��Ԃ��B
	bool CompareTextStyle(const AcDbObjectId &idTxtStl1, const AcDbObjectId &idTxtStl2);


	// �n�C�p�[�����N�̔�r	
	//// �n�C�p�[�����N�������ł���Ƃ��ƁA�����Ƃ��g���f�[�^���Ȃ��Ƃ���true��Ԃ��B
	bool CompareHyperLink(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	

	// �w�肵�����@���������������@�ł��邩���ׂ�B
	bool IsAssociatedDimension(AcDbDimension *pDim);

	// �V���{���e�[�u���̃��R�[�h�𖼑O�Ŕ�r����B
	bool CompareSymbolTableRecord(const AcDbObjectId &idRec1, const AcDbObjectId &idRec2);


	//// AcGeCurve�h���N���X ////	
	//AcGeCircArc2d �̔�r
	bool CompareCircArc2dGeom(AcGeCircArc2d *pCArc1, AcGeCircArc2d *pCArc2); 

	//AcGeEllipArc2d �̔�r
	bool CompareEllipArc2dGeom(AcGeEllipArc2d *pEArc1, AcGeEllipArc2d *pEArc2); 

	//AcGeLineSeg2d �̔�r
	bool CompareLineSeg2dGeom(AcGeLineSeg2d *pLineSeg1, AcGeLineSeg2d *pLineSeg2); 

	//AcGeNurbCurve2d �̔�r
	bool CompareNurbCurve2dGeom(AcGeNurbCurve2d *pNCurve1, AcGeNurbCurve2d *pNCurve2); 	

	

	// �n�b�`���O���E�� �|�����C���^�C�v�̔�r
	bool CompareHatchBoundary_Type_Polyline(int iIdx, AcDbHatch *pHatch1, AcDbHatch *pHatch2); 

	// �n�b�`���O���E�� �|�����C���^�C�v�ȊO�̔�r
	bool CompareHatchBoundary(int iIdx, AcDbHatch *pHatch1, AcDbHatch *pHatch2); 

	// ���[�W�����̍\���v�f���r����B
	// fullSubentPath��AcBr�����E���̌X�̐}�`��r�̕��@�́A���ꂼ���AcGe�N���X�̔�r��
	// �쐬����̂���ςȂ��߁A���[�W�����𕪉����A���ꂼ���AcDb���r������@�ɂ����B
	bool CompareRegionComponent(AcDbRegion *pRgn1, AcDbRegion *pRgn2); 
};

#endif // !defined(AFX_ACCOMPENTITIESGEOM_H__25291CD7_5C15_498A_BE88_757CCAB26930__INCLUDED_)
