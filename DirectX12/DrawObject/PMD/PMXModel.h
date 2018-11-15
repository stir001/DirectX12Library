#pragma once
#include <string>
#include <vector>
#include <d3d12.h>
#include <DirectXMath.h>

struct TextBuffer
{
	std::vector<unsigned char> buffer;
};

struct PMXHeader {
	unsigned char type[4];
	float ver;
	unsigned char count;
	std::vector<unsigned char> info;
};

enum PMXHeaderInfo {
	ENCODE,
	ADDUV,
	VERTEXINDEXSIZE,
	TEXINDEXSIZE,
	MATERIALINDEXSIZE,
	BONEINDEXSIZE,
	MORPHINDEXSIZE,
	RIGIDINDEXSIZE
};

struct PMXModelInfo {
	TextBuffer nameJP;
	TextBuffer nameUS;
	TextBuffer commentJP;
	TextBuffer commentUS;
};

struct BDEF1
{
	int index;
};

struct BDEF2
{
	int index1;
	int index2;
	float weight1;//�{�[��1�̃E�F�C�g(0~1.0) �{�[��2��1.0 - �{�[��1�̃E�F�C�g
};

struct BDEF4
{
	int index1;
	int index2;
	int index3;
	int index4;
	float weight1;
	float weight2;
	float weight3;
	float weight4;//�E�F�C�g�v1.0�̕ۏ؂͂Ȃ�
};

struct SDEF
{
	int index1;
	int index2;
	float weight1;//�{�[��1�̃E�F�C�g(0~1.0) �{�[��2��1.0 - �{�[��1�̃E�F�C�g
	DirectX::XMFLOAT3 c;
	DirectX::XMFLOAT3 r0;
	DirectX::XMFLOAT3 r1;
};

union BONETYPE
{
	BDEF1 bdef1;
	BDEF2 bdef2;
	BDEF4 bdef4;
	SDEF sdef;
};

struct PMXBone {
	unsigned char type;
	BONETYPE data;
};

struct PMXVertex {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 norm;
	DirectX::XMFLOAT2 uv;
	std::vector<DirectX::XMFLOAT4> addVertex;
	unsigned char type;
	PMXBone bone;
	float edgscale;
};

struct PMXPlane {
	std::vector<DirectX::XMINT3> index;
};

struct PMXMatarial
{
	TextBuffer nameJP;
	TextBuffer nameUS;

	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT3 specular;
	float specularrato;
	DirectX::XMFLOAT3 ambient;

	unsigned char drawFlag;//

	DirectX::XMFLOAT4 edgecolor;
	float edgesize;

	unsigned int tableindex;
	unsigned int spheretableindex;
	unsigned char spheremode;//0:���� 1:��Z 2:���Z 3:�T�u�e�N�X�`��

	unsigned char toonFlag;//0:�p���l�͌�Toon 1:�p���l�͋��LToon
	unsigned int toonindex;//�t���O�̏󋵂ɂ��ω�

	TextBuffer freetext;

	unsigned int adaptVertexcount;//�K��3�̔{���ɂȂ�
};

struct IKLINK {
	unsigned int linkboneIndex;
	unsigned char maxradflag;//�����p�̗L�薳��

	DirectX::XMFLOAT3 minrad;
	DirectX::XMFLOAT3 maxrad;
};

struct IKDATA {
	unsigned int targetboneIndex;
	int roopcount;
	float maxrad;

	unsigned int linkcount;
	std::vector<IKLINK> links;
};

struct PMXBoneData {
	TextBuffer boneNameJP;
	TextBuffer boneNameUS;

	DirectX::XMFLOAT3 pos;
	int parentIndex;
	unsigned int formlevel;

	unsigned short boneflag;
	/*0x0001  : �ڑ���(PMD�q�{�[���w��)�\�����@ -> 0 : ���W�I�t�Z�b�g�Ŏw�� 1 : �{�[���Ŏw��

	0x0002 : ��]�\
	0x0004 : �ړ��\
	0x0008 : �\��
	0x0010 : �����

	0x0020 : IK

	0x0080 : ���[�J���t�^ | �t�^�Ώ� 0 : ���[�U�[�ό`�l�^IK�����N�^���d�t�^ 1 : �e�̃��[�J���ό`��
	0x0100 : ��]�t�^
	0x0200 : �ړ��t�^

	0x0400 : ���Œ�
	0x0800 : ���[�J����

	0x1000 : ������ό`
	0x2000 : �O���e�ό`*/

	DirectX::XMFLOAT3 offsetpos;
	unsigned int conectboneIndex;

	unsigned int addparentboneIndex;
	float addparentRato;

	DirectX::XMFLOAT3 dirvec;//���Œ�

	DirectX::XMFLOAT3 xAxisvec;//���[�J����
	DirectX::XMFLOAT3 zAxisvec;//���[�J����

	unsigned int key;

	IKDATA ikdata;
};

enum MorphType
{
	mt_group,
	mt_vertex,
	mt_bone,
	mt_uv,
	mt_uvadd1,
	mt_uvadd2,
	mt_material,

};

struct VertexMorph
{
	unsigned int vertexIndex;
	DirectX::XMFLOAT3 offsetpos;
};

struct UVMorph
{
	unsigned int vertexIndex;
	DirectX::XMFLOAT4 uvoffset;
};

struct BoneMorph
{
	unsigned int boneIndex;
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT4 rotator;
};

struct MaterialMorph
{
	int materialIndex;//-1�͑S�ގ��ΏۊO
	unsigned char offsetcalType;//0:��Z 1:���Z
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT3 specular;
	float specularrato;
	DirectX::XMFLOAT3 ambient;
	DirectX::XMFLOAT4 edgecolor;
	float edgesize;
	DirectX::XMFLOAT4 texrato;
	DirectX::XMFLOAT4 spheretexrato;
	DirectX::XMFLOAT4 toontexrato;
};

struct GroupMorph
{
	unsigned int index;
	float mophrato;
};

union Morphtype
{
	VertexMorph vMorph;
	UVMorph uMorph;
	BoneMorph bMorph;
	MaterialMorph mMorph;
	GroupMorph gMorph;
};

struct PMXMorph {
	TextBuffer nameJP;
	TextBuffer nameUS;
	unsigned char controllpanel;
	unsigned char morphtype;
	unsigned int morphcount;
	std::vector<Morphtype> morphs;

};

struct OutLineData
{
	unsigned char type;
	unsigned int index;
};

struct PMXOutLine
{
	TextBuffer nameJP;
	TextBuffer nameUS;
	unsigned char flag;
	unsigned int count;
	std::vector<OutLineData> datas;
};

struct PMXRigidBody
{
	TextBuffer nameJP;
	TextBuffer nameUS;

	unsigned int boneIndex;//��

	unsigned char group;
	unsigned short groupflag;

	unsigned char shape;
	DirectX::XMFLOAT3 size;

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 rotator;//���W�A��

	float weight;//����
	float accelvel;//�ړ�����
	float accelrota;//��]����
	float boundpw;//������(�W��?
	float frictionpw;//���C��(�W��?

	unsigned char rigidflag;//0:�{�[���Ǐ](static) 1:�������Z(dynamic) 2:�������Z + Bone�ʒu���킹
};

struct DOF6
{
	unsigned int rigidAIndex;
	unsigned int rigidBIndex;

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 rotator;

	DirectX::XMFLOAT3 minVel;
	DirectX::XMFLOAT3 maxVel;
	DirectX::XMFLOAT3 minRotator;
	DirectX::XMFLOAT3 maxRotator;

	DirectX::XMFLOAT3 springCVel;
	DirectX::XMFLOAT3 springCRota;
};

struct PMXJoint
{
	TextBuffer nameJP;
	TextBuffer nameUS;

	unsigned char type;//pmx2.0�ł�0�̂�

	DOF6 spring;
};

class PMXModel
{
public:
	PMXModel();
	~PMXModel();

	PMXHeader mHeader;
	PMXModelInfo mModelinfo;
	std::vector<PMXVertex>		mVertex;
	PMXPlane					mPlanes;
	std::vector<TextBuffer>		mTexPath;
	std::vector<PMXMatarial>	 mMaterials;
	std::vector<PMXBoneData>	 mBonedata;
	std::vector<PMXMorph>		mMorphDatas;
	std::vector<PMXOutLine>		mOutline;
	std::vector<PMXRigidBody>	mRigid;
	std::vector<PMXJoint>		mJoints;
private:
};

