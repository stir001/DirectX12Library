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
	float weight1;//ボーン1のウェイト(0~1.0) ボーン2は1.0 - ボーン1のウェイト
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
	float weight4;//ウェイト計1.0の保証はない
};

struct SDEF
{
	int index1;
	int index2;
	float weight1;//ボーン1のウェイト(0~1.0) ボーン2は1.0 - ボーン1のウェイト
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
	unsigned char spheremode;//0:無効 1:乗算 2:加算 3:サブテクスチャ

	unsigned char toonFlag;//0:継続値は個別Toon 1:継続値は共有Toon
	unsigned int toonindex;//フラグの状況により変化

	TextBuffer freetext;

	unsigned int adaptVertexcount;//必ず3の倍数になる
};

struct IKLINK {
	unsigned int linkboneIndex;
	unsigned char maxradflag;//制限角の有り無し

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
	/*0x0001  : 接続先(PMD子ボーン指定)表示方法 -> 0 : 座標オフセットで指定 1 : ボーンで指定

	0x0002 : 回転可能
	0x0004 : 移動可能
	0x0008 : 表示
	0x0010 : 操作可

	0x0020 : IK

	0x0080 : ローカル付与 | 付与対象 0 : ユーザー変形値／IKリンク／多重付与 1 : 親のローカル変形量
	0x0100 : 回転付与
	0x0200 : 移動付与

	0x0400 : 軸固定
	0x0800 : ローカル軸

	0x1000 : 物理後変形
	0x2000 : 外部親変形*/

	DirectX::XMFLOAT3 offsetpos;
	unsigned int conectboneIndex;

	unsigned int addparentboneIndex;
	float addparentRato;

	DirectX::XMFLOAT3 dirvec;//軸固定

	DirectX::XMFLOAT3 xAxisvec;//ローカル軸
	DirectX::XMFLOAT3 zAxisvec;//ローカル軸

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
	int materialIndex;//-1は全材質対象外
	unsigned char offsetcalType;//0:乗算 1:加算
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

	unsigned int boneIndex;//可変

	unsigned char group;
	unsigned short groupflag;

	unsigned char shape;
	DirectX::XMFLOAT3 size;

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 rotator;//ラジアン

	float weight;//質量
	float accelvel;//移動減衰
	float accelrota;//回転減衰
	float boundpw;//反発力(係数?
	float frictionpw;//摩擦力(係数?

	unsigned char rigidflag;//0:ボーン追従(static) 1:物理演算(dynamic) 2:物理演算 + Bone位置合わせ
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

	unsigned char type;//pmx2.0では0のみ

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

