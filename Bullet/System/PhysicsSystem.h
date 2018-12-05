#pragma once
/**
*	@file	PhysicsSystem.h
*	@brief	bulletのworld周辺の機能を持つクラス定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/11/27
*/
#include <memory>
#include <DirectXMath.h>
#include <map>

class BulletDebugDrawDx;
class BulletRigidBody;
class BulletCollisionShape;
class CollisionDetector;
class BulletGhostObject;
class btGhostObject;
class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btGhostPairCallback;

/**
*	@enum	BulletShapeType
*	@brief	形状のタイプ定義
*/
enum class BulletShapeType
{
	BOX,
	SPHERE,
	CYLINDER,
	CAPSULE,
	PLANE,
	CONE
};

/**
*	@class PhysicsSystem
*	@brief rigidBodyやcollision関係を管理する
*/
class PhysicsSystem : public std::enable_shared_from_this<PhysicsSystem>
{
public:
	~PhysicsSystem();

	static PhysicsSystem* mInstance;

	static PhysicsSystem& Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new PhysicsSystem();
		}
		return *mInstance;
	}

	static void Destory()
	{
		if (mInstance != nullptr)
		{
			delete mInstance;
		}
	}

	/**
	*	デバッグ描画
	*/
	void DebugDraw();

	/**
	*	デバッグ用に取得したデータを解放する　毎フレーム呼ぶ
	*/
	void ClearDebugDraw();

	/**
	*	@brief	剛体をsystemの管理下に追加する
	*	@param[in]	collider	追加する剛体
	*/
	void AddRigidBody(std::shared_ptr<BulletRigidBody> rigid);

	int GetValidityWorldID();

	/**
	*	剛体シュミレーションをする
	*/
	void Simulation();

	/**
	*	@brief 剛体シュミレーションをする
	*	@param[in]	de;taTime	進める時間
	*/
	void Simulation(float deltaTime);

	/**
	*	@brief	剛体を管理下から解放する
	*	@param[in]	collider	解放する剛体
	*/
	void RemoveRigidBody(std::shared_ptr<BulletRigidBody> rigid);

	/**
	*	@brief 剛体を管理下から解放する
	*	@param[in]	tag		解放する剛体のタグ
	*/
	void RemoveRigidBody(int tag);

	/**
	*	@brief	rigidBodyを作成する
	*	@param[in]	type	作成する形状
	*	@param[in]	data	形状により異なる (x,y,z)
			BOX			(x, y, z)				各方向の辺の長さ
			SPHERE		(radius, nan, nan)		x半径 y無視	z無視
			CYLINDER	(radius, height, nan)	x半径 y高さ z無視
			CAPSULE		(radisu, height, nan)	x半径 y高さ z無視
			PLANE		(x, y, z)				x法線 y法線 z法線 
			CONE		(radius, height, nan)	x半径 y高さ z無視
	*	@param[in]	pos		初期位置
	*/
	std::shared_ptr<BulletRigidBody> CreateRigitBody(const BulletShapeType type
		, const DirectX::XMFLOAT3& data , const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f,0.f,0.f));

	/**
	*	@brief	コリジョン形状を作成する
	*	@param[in]	type	作成する形状
	*	@param[in]	data	形状により異なる (x,y,z)
			BOX			(x, y, z)				各方向の辺の長さ
			SPHERE		(radius, nan, nan)		x半径 y無視	z無視
			CYLINDER	(radius, height, nan)	x半径 y高さ z無視
			CAPSULE		(radisu, height, nan)	x半径 y高さ z無視
			PLANE		(x, y, z)				x法線 y法線 z法線 
			CONE		(radius, height, nan)	x半径 y高さ z無視
	*/
	std::shared_ptr<BulletCollisionShape> CreateCollisionShape(const BulletShapeType type
		, const DirectX::XMFLOAT3& data);

	/**
	*	@brief	ghostを作成する
	*	@param[in]	type	作成する形状
	*	@param[in]	data	形状により異なる (x,y,z)
	BOX			(x, y, z)				各方向の辺の長さ
	SPHERE		(radius, nan, nan)		x半径 y無視	z無視
	CYLINDER	(radius, height, nan)	x半径 y高さ z無視
	CAPSULE		(radisu, height, nan)	x半径 y高さ z無視
	PLANE		(x, y, z)				x法線 y法線 z法線
	CONE		(radius, height, nan)	x半径 y高さ z無視
	*	@param[in]	pos		初期位置
	*/
	std::shared_ptr<BulletGhostObject> CreateGhostObject(const BulletShapeType type
		, const DirectX::XMFLOAT3& data, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));

	/**
	*	@brief	アクションを追加する
	*	@param[in]	action	追加するアクション
	*/
	void AddAction(std::shared_ptr<CollisionDetector> action);

	/**
	*
	*/
	void RemoveAction(std::shared_ptr<CollisionDetector> action);

	/**
	*	@brief	ゴーストを追加する
		@param[in]	ghost	追加するゴースト
	*/
	void AddGhost(std::shared_ptr<BulletGhostObject> ghost);

	/**
	*	@brief	Ghostを世界から削除する
	*	@parma[in]	index	削除するインデックス
	*/
	void RemoveGhost(int index);
private:
	PhysicsSystem();
	PhysicsSystem(const PhysicsSystem&) = delete;
	PhysicsSystem(const PhysicsSystem&&) = delete;

	/**
	*	広域位相フェーズ
	*/
	std::shared_ptr<btBroadphaseInterface>					mBroadphase;

	/**
	*	狭域位相フェーズ設定
	*/
	std::shared_ptr<btDefaultCollisionConfiguration>		mCollisionConfiguration;

	/**
	*	狭域位相フェーズ
	*/
	std::shared_ptr<btCollisionDispatcher>					mDispatcher;

	/**
	*	拘束解決
	*/
	std::shared_ptr<btSequentialImpulseConstraintSolver>	mSolver;

	/**
	*	世界
	*/
	std::shared_ptr<btDiscreteDynamicsWorld>				mWorld;

	/**
	*	デバッグ描画システム
	*/
	std::shared_ptr<BulletDebugDrawDx>						mDebugDrawer;

	/**
	*	剛体保持
	*/
	std::map<int, std::shared_ptr<BulletRigidBody>>			mRigidBodies;
	
	/**
	*	ゴースト保持
	*/
	std::map<int, std::shared_ptr<BulletGhostObject>> mGhosts;

	/**
	*	現在の時間
	*/
	long mTime;

	/**
	*	ghostをペアリングするためのコールバッククラス
	*/
	std::shared_ptr<btGhostPairCallback> mGhostCallBack;
};

