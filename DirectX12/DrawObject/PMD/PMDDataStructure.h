#pragma once
/**
*	@addtogroup PMD_Data
*	@file PMDDataStructure
*	@brief PMD�̏ڍׂȏ���ێ�����\���̂̒�`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/21
*/
#include <vector>
#include <DirectXMath.h>

/**
*	@ingroup PMD_Data
*	@struct PMDHeader
*	@brief PMD�t�@�C���̃w�b�_�����̍\����
*/
struct PMDHeader
{
	/**
	*	PMD�Ƃ��������������Ă���w�b�_����
	*/
	char fileType[3];

	/**
	*	PMD�̃o�[�W�������
	*/
	float version;

	/**
	*	PMD���f���̖��O
	*/
	char name[20];

	/**
	*	PMD���f���ɂ��ẴR�����g
	*/
	char comment[256];

	/**
	*	���_��
	*/
	unsigned int vertexcount;
};

/**
*	@ingroup PMD_Data
*	@struct PMDVertex
*	@brief PMD�̒��_���
*/
struct PMDVertex
{
	/**
	*	���_���W
	*/
	DirectX::XMFLOAT3 pos;

	/**
	*	�@�����
	*/
	DirectX::XMFLOAT3 normal;

	/**
	*	uv���
	*/
	DirectX::XMFLOAT2 uv;

	/**
	*	�e�����󂯂�{�[���ԍ�
	*/
	unsigned short boneNum[2];

	/**
	*	�{�[���ԍ���0�Ԗڂ̃{�[������e�����󂯂�E�F�C�g
	*/
	unsigned char boneWeight;
	unsigned char edgeFlag;//�֊s���t���O(1�o�C�g) 
};

/**
*	@ingroup PMD_Data
*	@struct PMDMaterial
*	@brief PMD�̃}�e���A�����
*/
struct PMDMaterial {
	/**
	*	diffuse�J���[�A�g�U����
	*/
	DirectX::XMFLOAT3 diffuse;

	/**
	*	������
	*/
	float alpha;

	/**
	*	���ʐ�
	*/
	float specularity;

	/**
	*	���ʔ���
	*/
	DirectX::XMFLOAT3 specular;

	/**
	*	����
	*/
	DirectX::XMFLOAT3 ambient;

	/**
	*	toonTexture��Index
	*/
	unsigned char toonIndex;

	/**
	*	�֊s�t���O	0:�ʏ�	1:����
	*/
	unsigned char edgeFlag;

	/**
	*	�ʒ��_���X�g�̐�
	*/
	unsigned int indexCount;

	/**
	*	�e�N�X�`���t�@�C����
	*/
	char texturePath[20];

	/**
	*	�e�N�X�`����ID(�Ǝ��t�����Ă�����)
	*/
	int texid;
};

/**
*	@ingroup PMD_Data
*	@struct PMDBoneData
*	@brief PMD�̃{�[�����
*/
struct PMDBoneData
{
	/**
	*	�{�[���̖��O
	*/
	char boneName[20];

	/**
	*	�{�[���̐e�ԍ�(�Ȃ����̂�0xffff)
	*/
	unsigned short parentIndex;

	/**
	*	�{�[���̏I�[�ԍ�(�Ȃ��ꍇ��0)
	*/
	unsigned short tailIndex;

	/**
	*	�{�[���̎��
	*	0:��] 1:��]�ƈړ� 2:IK 3:�s�� 4:IK�e���� 5:��]�e���� 6:IK�ڑ��� 7:��\�� 8:�P�� 9:��]�^��
	*/
	unsigned char type;

	/**
	*	�e�����󂯂�IK�{�[���ԍ�(�Ȃ��ꍇ��0)
	*/
	unsigned short ikParentIndex;

	/**
	*	�{�[���̃w�b�h�̍��W
	*/
	DirectX::XMFLOAT3 pos;
};

/**
*	@ingroup PMD_Data
*	@struct PMDIKData
*	@brief IKBone�̃f�[�^
*/
struct PMDIKData
{
	/**
	*	IK�{�[���ԍ�
	*/
	unsigned short ikBoneIndex;

	/**
	*	IK�^�[�Q�b�g�{�[���ԍ� IK�{�[�����ŏ��ɐڑ�����{�[��
	*/
	unsigned short ikTargetBoneIndex;

	/**
	*	IK�`�F�[���̒���(�q�̐�)
	*/
	unsigned char ikChainLength;

	/**
	*	�ċA���Z�� IK�l1
	*/
	unsigned short iterations;

	/**
	*	���Z��񓖂���̐����p�x IK�l2
	*/
	float controlWeight;

	/**
	*	IK�e�����̃{�[���ԍ� �v�f����ikChainLength��)
	*/
	std::vector<unsigned short> ikChildBoneIndex;
};

/**
*	@ingroup PMD_Data
*	@struct PMDIKList
*	@brief PMDIK�����܂Ƃ߂ĕێ�
*/
struct PMDIKList
{
	/**
	*	IK�{�[���̐�
	*/
	unsigned short ikCount;

	/**
	*	IK�̃f�[�^�Q
	*/
	std::vector<PMDIKData> ikDatas;
};

/**
*	@ingroup PMD_Data
*	@struct PMDSkinVertexData
*	@brief PMD�̕\��p�̒��_�f�[�^
*/
struct PMDSkinVertexData
{
	/**
	*	���_�C���f�b�N�X
	*/
	unsigned int index;

	/**
	*	���_���W
	*/
	DirectX::XMFLOAT3 pos;
};

/**
*	@ingroup PMD_Data
*	@struct PMDSkinData
*	@brief PMD�̕\����
*/
struct PMDSkinData
{
	/**
	*	�\��̖��O
	*/
	char skinName[20];

	/**
	*	�\��v�̒��_��
	*/
	unsigned int skinVertexCount;

	/**
	*	�\��̎�� 0:base,1:�܂�,2:��,3:���b�v,4:���̑�
	*/
	char skintype;

	/**
	*	�\��p�̒��_���Q
	*/
	std::vector<PMDSkinVertexData> vertices;
};

/**
*	@ingroup PMD_Data
*	@struct PMDSkin
*	@brief PMD�̕\��p�̏��Q
*/
struct PMDSkin
{
	unsigned short skincount;
	std::vector<PMDSkinData> datas;
};

/**
*	@ingroup PMD_Data
*	@struct PMDSkinDisp
*	@brief �\��g�p�\�����X�g
*/
struct PMDSkinDisp
{
	/**
	*	�\��g�ɕ\������\�
	*/
	unsigned char skindispcount;

	/**
	*	�\��ԍ�
	*/
	std::vector<unsigned short> skinIndices;
};

/**
*	@ingroup PMD_Data
*	@struct PMDBoneFrameDispName
*	@brief �{�[���g�p�̖��O
*/
struct PMDBoneFrameDispName
{
	/**
	*	�g�̖��O
	*/
	char name[50];
};

/**
*	@ingroup PMD_Data
*	@struct PMDBoneFrameDisp
*	@brief �{�[���g�\���p���
*/
struct PMDBoneFrameDisp
{
	/**
	*	�{�[���g�̖��O�̐�
	*/
	unsigned char boneDispNameCount;
	std::vector<PMDBoneFrameDispName> names;
};

/**
*	@ingroup PMD_Data
*	@struct PMDBoneDispIndex
*	@brief �{�[���̖��O�\���p���
*/
struct PMDBoneDispIndex
{
	/**
	*	�{�[���̔ԍ�
	*/
	unsigned short boneIndex;

	/**
	*	�{�[���̕\���p�g�̔ԍ�
	*/
	unsigned char boneDispFrameIndex;
};

/**
*	@ingroup PMD_Data
*	@struct PMDBoneDisp
*	@brief	�{�[���̖��O�\���p�̏��Q
*/
struct PMDBoneDisp
{
	/**
	*	0�ȊO�̃{�[���̐��̍��v
	*/
	unsigned int boneocunt;

	/**
	*	�{�[�����\���p�̏��Q
	*/
	std::vector<PMDBoneDispIndex> boneDispIndices;
};

/**
*	@ingroup PMD_Data
*	@struct PMDToonPath
*	@brief PMD�̃g�D�[���p�̃e�N�X�`���p�X�i�[�p
*/
struct PMDToonPath
{
	/**
	*	�e�N�X�`���p�X[100]	�ő吔[10]
	*/
	char path[100][10];
};

/**
*	@ingroup PMD_Data
*	@struct PMDRigidBodyData
*	@brief PMD�̍��̃f�[�^
*/
struct PMDRigidBodyData
{
	/**
	*	���̖̂��O
	*/
	char rigidBodyName[20];

	/**
	*	���̂Ɗ֘A����{�[���ԍ�
	*/
	unsigned short rigidbodyRelBoneIndex;

	/**
	*	���̃O���[�v�ԍ�
	*/
	unsigned char rigidbodyGroupIndex;

	/**
	*	���̃O���[�v�Ώ۔ԍ�
	*/
	unsigned short rigidbodyGroupTarget;

	/**
	*	�`��̎��	0:�� 1:�� 2:�J�v�Z��
	*/
	unsigned char shapeType;

	/**
	*	�`��̔��a(��) 
	*/
	float shapeW;

	/**
	*	�`��̍���
	*/
	float shapeH;

	/**
	*	�`��̉��s
	*/
	float shapeD;

	/**
	*	���̂̈ʒu
	*/
	DirectX::XMFLOAT3 pos;

	/**
	*	���̂̉�](radian,�ʓx�@)
	*/
	DirectX::XMFLOAT3 rot;

	/**
	*	����
	*/
	float weight;

	/**
	*	�ړ�������
	*/
	float posDim;

	/**
	*	��]������
	*/
	float rotDim;

	/**
	*	������
	*/
	float recoil;

	/**
	*	���C��
	*/
	float friction;

	/**
	*	���̂̎��	0:Bone�Ǐ] 1:�������Z 2:�������Z(Bone�ʒu����)
	*/
	unsigned char rigidbodyType;
};

/**
*	@ingroup PMD_Data
*	@struct PMDRigidBody
*	@brief ���̏��Q
*/
struct PMDRigdBody
{
	/**
	*	���̂̌�
	*/
	unsigned int rigidbodycount;

	/**
	*	���̂̏��Q
	*/
	std::vector<PMDRigidBodyData> datas;
};

/**
*	@ingroup PMD_Data
*	@struct PMDJointData
*	@brief �������Z�̃W���C���g�̏��
*/
struct PMDJointData
{
	/**
	*	�W���C���g�̖��O
	*/
	char jointName[20];

	/**
	*	����A(�̔ԍ�?)
	*/
	unsigned int jointRigidbodyA;

	/**
	*	����B(�̔ԍ�?)
	*/
	unsigned int jointRigidbodyB;

	/**
	*	�W���C���g�̍��W
	*/
	DirectX::XMFLOAT3 jointPos;

	/**
	*	�W���C���g�̉�](radian,�ʓx�@)
	*/
	DirectX::XMFLOAT3 jointRot;

	/**
	*	�W���C���g�̈ړ�����1
	*/
	DirectX::XMFLOAT3 constrainPos1;

	/**
	*	�W���C���g�̈ړ�����2
	*/
	DirectX::XMFLOAT3 constrainPos2; // �����F�ړ�2(x, y, z)

	/**
	*	�W���C���g�̉�]����(radian,�ʓx�@)
	*/
	DirectX::XMFLOAT3 constrainRot1;

	/**
	*	�W���C���g�̉�]����(radian,�ʓx�@)
	*/
	DirectX::XMFLOAT3 constrainRot2;

	/**
	*	�΂˂̈ړ�
	*/
	DirectX::XMFLOAT3 springPos;

	/**
	*	�΂˂̉�](radian,�ʓx�@)
	*/
	DirectX::XMFLOAT3 springRot; // �΂ˁF��](rad(x), rad(y), rad(z))
};

/**
*	@ingroup PMD_Data
*	@struct PMDJoint
*	@brief �W���C���g�̏��Q
*/
struct PMDJoint
{
	/**
	*	�W���C���g�̑���
	*/
	unsigned int jointcount;

	/**
	*	�W���C���g�̏��Q
	*/
	std::vector<PMDJointData> datas;
};

/**
*	@ingroup PMD_Data
*	@struct BoneTree
*	@brief �{�[���̐e�q�\���ۑ��p
*/
struct BoneTree
{
	/**
	*	�e�q�֌W��ԍ��ŊǗ� �֌W�͈ȉ��̒ʂ�
	*	n�͎q�̃{�[���̑���(node[�e�{�[���ԍ�].size())��菬�����ԍ�
	*	node[�e�{�[���ԍ�][n] = �q��boneIndex
	*/
	std::vector<std::vector<int>> node;
};