#pragma once

//	�}�ʔ�rAPI�p
namespace acd{
	typedef CMap<int, int, AcDbObjectId, AcDbObjectId> objectIdContainer;

};

namespace zfc{

	// �G���e�B�e�B�N���[�Y�֐��I�u�W�F�N�g
	struct closeEntity {
		void operator ()(AcDbEntity* pEnt) {
			auto es = pEnt->close();
			assert( Acad::eOk == es );
		}
	};

	//�啶���Ə���������ʂ�����2�̕�������r����֐��I�u�W�F�N�g
	struct lessNocase {
		bool operator()(const CString& x, const CString& y) const {
			return ( x.CompareNoCase(y) < 0 );
		}
	};

	//	�G���e�B�e�B�|�C���^�^�C�v��`
	typedef std::unique_ptr<AcDbEntity, closeEntity> entityPtr;

	//	�G���e�B�e�B�R���e�i�^�C�v��`(key=�}�`�n���h��, value=�G���e�B�e�B)
	typedef std::map<CString, entityPtr> entityContainer;

	//	�p�X�R���e�i�^�C�v��`(key=�t�@�C���^�C�g��, value=�t�@�C���p�X)
	typedef std::map<CString, CString, lessNocase> pathContainer;
	
	// for_each �̈������ȗ�������e���v���[�g�֐�
	template <typename T_container, typename T_function>
	T_function for_each(T_container& rcontainer, T_function function) {
		return std::for_each(rcontainer.begin(), rcontainer.end(), function);
	}
};