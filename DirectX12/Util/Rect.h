#pragma once
#include <DirectXMath.h>
#include <memory>
#include <vector>

class Primitive2DLine;

class Rect
{
public:
	Rect();
	Rect(DirectX::XMFLOAT3& inc,float inw,float inh);
	~Rect();

	float GetLeft() const;
	float GetUp() const;
	float GetRight() const;
	float GetDown() const;
	float GetWidth() const;
	float GetHeight() const;
	const DirectX::XMFLOAT3& GetCenter() const;

	void SetCenter(const DirectX::XMFLOAT3& inc);
	void SetWidth(const float inw);
	void SetHeight(const float inh);
	void SetScale(float scale);

	void Draw();
private:
	DirectX::XMFLOAT3 mCenter;
	float mWidth;
	float mHeight;
	float mScale;

#ifdef _DEBUG
	std::shared_ptr<Primitive2DLine> mLines[4];
	void UpdateLines();
#endif
};

