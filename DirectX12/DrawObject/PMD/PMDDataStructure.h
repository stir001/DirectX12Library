#pragma once
/**
*	@addtogroup PMD_Data
*	@file PMDDataStructure
*	@brief PMDの詳細な情報を保持する構造体の定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/21
*/
#include <vector>
#include <DirectXMath.h>

/**
*	@ingroup PMD_Data
*	@struct PMDHeader
*	@brief PMDファイルのヘッダ部分の構造体
*/
struct PMDHeader
{
	/**
	*	PMDという文字が入っているヘッダ部分
	*/
	char fileType[3];

	/**
	*	PMDのバージョン情報
	*/
	float version;

	/**
	*	PMDモデルの名前
	*/
	char name[20];

	/**
	*	PMDモデルについてのコメント
	*/
	char comment[256];

	/**
	*	頂点数
	*/
	unsigned int vertexcount;
};

/**
*	@ingroup PMD_Data
*	@struct PMDVertex
*	@brief PMDの頂点情報
*/
struct PMDVertex
{
	/**
	*	頂点座標
	*/
	DirectX::XMFLOAT3 pos;

	/**
	*	法線情報
	*/
	DirectX::XMFLOAT3 normal;

	/**
	*	uv情報
	*/
	DirectX::XMFLOAT2 uv;

	/**
	*	影響を受けるボーン番号
	*/
	unsigned short boneNum[2];

	/**
	*	ボーン番号の0番目のボーンから影響を受けるウェイト
	*/
	unsigned char boneWeight;
	unsigned char edgeFlag;//輪郭線フラグ(1バイト) 
};

/**
*	@ingroup PMD_Data
*	@struct PMDMaterial
*	@brief PMDのマテリアル情報
*/
struct PMDMaterial {
	/**
	*	diffuseカラー、拡散反射
	*/
	DirectX::XMFLOAT3 diffuse;

	/**
	*	α成分
	*/
	float alpha;

	/**
	*	鏡面性
	*/
	float specularity;

	/**
	*	鏡面反射
	*/
	DirectX::XMFLOAT3 specular;

	/**
	*	環境光
	*/
	DirectX::XMFLOAT3 ambient;

	/**
	*	toonTextureのIndex
	*/
	unsigned char toonIndex;

	/**
	*	輪郭フラグ	0:通常	1:無効
	*/
	unsigned char edgeFlag;

	/**
	*	面頂点リストの数
	*/
	unsigned int indexCount;

	/**
	*	テクスチャファイル名
	*/
	char texturePath[20];

	/**
	*	テクスチャのID(独自付加している情報)
	*/
	int texid;
};

/**
*	@ingroup PMD_Data
*	@struct PMDBoneData
*	@brief PMDのボーン情報
*/
struct PMDBoneData
{
	/**
	*	ボーンの名前
	*/
	char boneName[20];

	/**
	*	ボーンの親番号(ないものは0xffff)
	*/
	unsigned short parentIndex;

	/**
	*	ボーンの終端番号(ない場合は0)
	*/
	unsigned short tailIndex;

	/**
	*	ボーンの種類
	*	0:回転 1:回転と移動 2:IK 3:不明 4:IK影響下 5:回転影響下 6:IK接続先 7:非表示 8:捻り 9:回転運動
	*/
	unsigned char type;

	/**
	*	影響を受けるIKボーン番号(ない場合は0)
	*/
	unsigned short ikParentIndex;

	/**
	*	ボーンのヘッドの座標
	*/
	DirectX::XMFLOAT3 pos;
};

/**
*	@ingroup PMD_Data
*	@struct PMDIKData
*	@brief IKBoneのデータ
*/
struct PMDIKData
{
	/**
	*	IKボーン番号
	*/
	unsigned short ikBoneIndex;

	/**
	*	IKターゲットボーン番号 IKボーンが最初に接続するボーン
	*/
	unsigned short ikTargetBoneIndex;

	/**
	*	IKチェーンの長さ(子の数)
	*/
	unsigned char ikChainLength;

	/**
	*	再帰演算回数 IK値1
	*/
	unsigned short iterations;

	/**
	*	演算一回当たりの制限角度 IK値2
	*/
	float controlWeight;

	/**
	*	IK影響下のボーン番号 要素数はikChainLength数)
	*/
	std::vector<unsigned short> ikChildBoneIndex;
};

/**
*	@ingroup PMD_Data
*	@struct PMDIKList
*	@brief PMDIK情報をまとめて保持
*/
struct PMDIKList
{
	/**
	*	IKボーンの数
	*/
	unsigned short ikCount;

	/**
	*	IKのデータ群
	*/
	std::vector<PMDIKData> ikDatas;
};

/**
*	@ingroup PMD_Data
*	@struct PMDSkinVertexData
*	@brief PMDの表情用の頂点データ
*/
struct PMDSkinVertexData
{
	/**
	*	頂点インデックス
	*/
	unsigned int index;

	/**
	*	頂点座標
	*/
	DirectX::XMFLOAT3 pos;
};

/**
*	@ingroup PMD_Data
*	@struct PMDSkinData
*	@brief PMDの表情情報
*/
struct PMDSkinData
{
	/**
	*	表情の名前
	*/
	char skinName[20];

	/**
	*	表情要の頂点数
	*/
	unsigned int skinVertexCount;

	/**
	*	表情の種類 0:base,1:まゆ,2:目,3:リップ,4:その他
	*/
	char skintype;

	/**
	*	表情用の頂点情報群
	*/
	std::vector<PMDSkinVertexData> vertices;
};

/**
*	@ingroup PMD_Data
*	@struct PMDSkin
*	@brief PMDの表情用の情報群
*/
struct PMDSkin
{
	unsigned short skincount;
	std::vector<PMDSkinData> datas;
};

/**
*	@ingroup PMD_Data
*	@struct PMDSkinDisp
*	@brief 表情枠用表示リスト
*/
struct PMDSkinDisp
{
	/**
	*	表情枠に表示する表情数
	*/
	unsigned char skindispcount;

	/**
	*	表情番号
	*/
	std::vector<unsigned short> skinIndices;
};

/**
*	@ingroup PMD_Data
*	@struct PMDBoneFrameDispName
*	@brief ボーン枠用の名前
*/
struct PMDBoneFrameDispName
{
	/**
	*	枠の名前
	*/
	char name[50];
};

/**
*	@ingroup PMD_Data
*	@struct PMDBoneFrameDisp
*	@brief ボーン枠表示用情報
*/
struct PMDBoneFrameDisp
{
	/**
	*	ボーン枠の名前の数
	*/
	unsigned char boneDispNameCount;
	std::vector<PMDBoneFrameDispName> names;
};

/**
*	@ingroup PMD_Data
*	@struct PMDBoneDispIndex
*	@brief ボーンの名前表示用情報
*/
struct PMDBoneDispIndex
{
	/**
	*	ボーンの番号
	*/
	unsigned short boneIndex;

	/**
	*	ボーンの表示用枠の番号
	*/
	unsigned char boneDispFrameIndex;
};

/**
*	@ingroup PMD_Data
*	@struct PMDBoneDisp
*	@brief	ボーンの名前表示用の情報群
*/
struct PMDBoneDisp
{
	/**
	*	0以外のボーンの数の合計
	*/
	unsigned int boneocunt;

	/**
	*	ボーン名表示用の情報群
	*/
	std::vector<PMDBoneDispIndex> boneDispIndices;
};

/**
*	@ingroup PMD_Data
*	@struct PMDToonPath
*	@brief PMDのトゥーン用のテクスチャパス格納用
*/
struct PMDToonPath
{
	/**
	*	テクスチャパス[100]	最大数[10]
	*/
	char path[100][10];
};

/**
*	@ingroup PMD_Data
*	@struct PMDRigidBodyData
*	@brief PMDの剛体データ
*/
struct PMDRigidBodyData
{
	/**
	*	剛体の名前
	*/
	char rigidBodyName[20];

	/**
	*	剛体と関連するボーン番号
	*/
	unsigned short rigidbodyRelBoneIndex;

	/**
	*	剛体グループ番号
	*/
	unsigned char rigidbodyGroupIndex;

	/**
	*	剛体グループ対象番号
	*/
	unsigned short rigidbodyGroupTarget;

	/**
	*	形状の種類	0:球 1:箱 2:カプセル
	*/
	unsigned char shapeType;

	/**
	*	形状の半径(幅) 
	*/
	float shapeW;

	/**
	*	形状の高さ
	*/
	float shapeH;

	/**
	*	形状の奥行
	*/
	float shapeD;

	/**
	*	剛体の位置
	*/
	DirectX::XMFLOAT3 pos;

	/**
	*	剛体の回転(radian,弧度法)
	*/
	DirectX::XMFLOAT3 rot;

	/**
	*	質量
	*/
	float weight;

	/**
	*	移動減衰量
	*/
	float posDim;

	/**
	*	回転減衰量
	*/
	float rotDim;

	/**
	*	反発力
	*/
	float recoil;

	/**
	*	摩擦力
	*/
	float friction;

	/**
	*	剛体の種類	0:Bone追従 1:物理演算 2:物理演算(Bone位置合せ)
	*/
	unsigned char rigidbodyType;
};

/**
*	@ingroup PMD_Data
*	@struct PMDRigidBody
*	@brief 剛体情報群
*/
struct PMDRigdBody
{
	/**
	*	剛体の個数
	*/
	unsigned int rigidbodycount;

	/**
	*	剛体の情報群
	*/
	std::vector<PMDRigidBodyData> datas;
};

/**
*	@ingroup PMD_Data
*	@struct PMDJointData
*	@brief 物理演算のジョイントの情報
*/
struct PMDJointData
{
	/**
	*	ジョイントの名前
	*/
	char jointName[20];

	/**
	*	剛体A(の番号?)
	*/
	unsigned int jointRigidbodyA;

	/**
	*	剛体B(の番号?)
	*/
	unsigned int jointRigidbodyB;

	/**
	*	ジョイントの座標
	*/
	DirectX::XMFLOAT3 jointPos;

	/**
	*	ジョイントの回転(radian,弧度法)
	*/
	DirectX::XMFLOAT3 jointRot;

	/**
	*	ジョイントの移動制限1
	*/
	DirectX::XMFLOAT3 constrainPos1;

	/**
	*	ジョイントの移動制限2
	*/
	DirectX::XMFLOAT3 constrainPos2; // 制限：移動2(x, y, z)

	/**
	*	ジョイントの回転制限(radian,弧度法)
	*/
	DirectX::XMFLOAT3 constrainRot1;

	/**
	*	ジョイントの回転制限(radian,弧度法)
	*/
	DirectX::XMFLOAT3 constrainRot2;

	/**
	*	ばねの移動
	*/
	DirectX::XMFLOAT3 springPos;

	/**
	*	ばねの回転(radian,弧度法)
	*/
	DirectX::XMFLOAT3 springRot; // ばね：回転(rad(x), rad(y), rad(z))
};

/**
*	@ingroup PMD_Data
*	@struct PMDJoint
*	@brief ジョイントの情報群
*/
struct PMDJoint
{
	/**
	*	ジョイントの総数
	*/
	unsigned int jointcount;

	/**
	*	ジョイントの情報群
	*/
	std::vector<PMDJointData> datas;
};

/**
*	@ingroup PMD_Data
*	@struct BoneTree
*	@brief ボーンの親子構造保存用
*/
struct BoneTree
{
	/**
	*	親子関係を番号で管理 関係は以下の通り
	*	nは子のボーンの総数(node[親ボーン番号].size())より小さい番号
	*	node[親ボーン番号][n] = 子のboneIndex
	*/
	std::vector<std::vector<int>> node;
};

class BulletRigidBody;

struct PMDRigid
{
	std::shared_ptr<BulletRigidBody> rigidBody;
	int refBoneIndex;
};