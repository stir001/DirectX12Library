#pragma once
#include "IPrimitive2D.h"

class Primitive2DLine : public IPrimitive2D
{
public:
	Primitive2DLine(const DirectX::XMFLOAT3& point1,const DirectX::XMFLOAT3& point2
		, const Microsoft::WRL::ComPtr<ID3D12Device> dev, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
	~Primitive2DLine();

	void SetCenter(const DirectX::XMFLOAT3& pos);
	void Draw();
	void SetScale(float scale);
	void SetRota(float deg);
	void SetBasePoints(const DirectX::XMFLOAT3& point1, const DirectX::XMFLOAT3& point2);
	void SetColor(const DirectX::XMFLOAT3& color);
	void SetPoints(const DirectX::XMFLOAT3& point1, const DirectX::XMFLOAT3& point2);

	void AddCenterPos(const DirectX::XMFLOAT3& val);
private:
	DirectX::XMFLOAT3 mCenter;
	DirectX::XMFLOAT3 mColor;
	/*DirectX::XMFLOAT2 mWndSize;*/
	float mBaseRad;
	float mAddRad;
	float mLength;
	float mScale;

	void UpdateVertex();
	void UpdateBuffer();
};

