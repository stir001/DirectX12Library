#include "stdafx.h"
#include "Rect.h"
#include "DrawObject/Primitive2D/Primitive2DManager.h"

Rect::Rect() :mCenter(0, 0, 0), mWidth(10), mHeight(10), mScale(1.0f)
{
#ifdef _DEBUG
	/*mLines[0] = Primitive2DManager::Instance().CreatePrimitive2DLine(
		DirectX::XMFLOAT3(GetLeft(), GetUp(), 0),
		DirectX::XMFLOAT3(GetRight(), GetUp(), 0));

	mLines[1] = Primitive2DManager::Instance().CreatePrimitive2DLine(
		DirectX::XMFLOAT3(GetRight(), GetUp(), 0),
		DirectX::XMFLOAT3(GetRight(), GetDown(), 0));

	mLines[2] = Primitive2DManager::Instance().CreatePrimitive2DLine(
		DirectX::XMFLOAT3(GetRight(), GetDown(), 0),
		DirectX::XMFLOAT3(GetLeft(), GetDown(), 0));

	mLines[3] = Primitive2DManager::Instance().CreatePrimitive2DLine(
		DirectX::XMFLOAT3(GetLeft(), GetDown(), 0),
		DirectX::XMFLOAT3(GetLeft(), GetUp(), 0));*/
#endif
}

Rect::Rect(DirectX::XMFLOAT3& inc, float inw, float inh): mCenter(inc), mWidth(inw), mHeight(inh), mScale(1.0f)
{
#ifdef _DEBUG
	/*mLines[0] = Primitive2DManager::Instance().CreatePrimitive2DLine(
		DirectX::XMFLOAT3(GetLeft(), GetUp(), 0),
		DirectX::XMFLOAT3(GetRight(), GetUp(), 0));

	mLines[1] = Primitive2DManager::Instance().CreatePrimitive2DLine(
		DirectX::XMFLOAT3(GetRight(), GetUp(), 0),
		DirectX::XMFLOAT3(GetRight(), GetDown(), 0));

	mLines[2] = Primitive2DManager::Instance().CreatePrimitive2DLine(
		DirectX::XMFLOAT3(GetRight(), GetDown(), 0),
		DirectX::XMFLOAT3(GetLeft(), GetDown(), 0));

	mLines[3] = Primitive2DManager::Instance().CreatePrimitive2DLine(
		DirectX::XMFLOAT3(GetLeft(), GetDown(), 0),
		DirectX::XMFLOAT3(GetLeft(), GetUp(), 0));*/
#endif
}

Rect::~Rect()
{
}

float Rect::GetLeft() const
{
	return mCenter.x - (mWidth * mScale * 0.5f);
}

float Rect::GetUp() const
{
	return mCenter.y + (mHeight * mScale * 0.5f);
}

float Rect::GetRight() const
{
	return mCenter.x + (mWidth * mScale * 0.5f);
}

float Rect::GetDown() const
{
	return mCenter.y - (mHeight * mScale * 0.5f);
}

float Rect::GetWidth() const
{
	return mWidth * mScale;
}

float Rect::GetHeight() const
{
	return mHeight * mScale;
}

const DirectX::XMFLOAT3& Rect::GetCenter() const
{
	return mCenter;
}

void Rect::SetCenter(const DirectX::XMFLOAT3& inc)
{
	mCenter = inc;
#ifdef _DEBUG
	UpdateLines();
#endif
}

void Rect::SetWidth(const float inw)
{
	mWidth = inw;
#ifdef _DEBUG
	UpdateLines();
#endif
}

void Rect::SetHeight(const float inh)
{
	mHeight = inh;
#ifdef _DEBUG
	UpdateLines();
#endif
}

void Rect::SetScale(float scale)
{
	mScale = scale;
}

void Rect::Draw()
{
#ifdef _DEBUG
	//for (auto& line : mLines)
	//{
	//	line->Draw();
	//}
#endif
}

#ifdef _DEBUG
void Rect::UpdateLines()
{
	//mLines[0]->SetPoints(
	//	DirectX::XMFLOAT3(GetLeft(), GetUp(), 0),
	//	DirectX::XMFLOAT3(GetRight(), GetUp(), 0));

	//mLines[1]->SetPoints(
	//	DirectX::XMFLOAT3(GetRight(), GetUp(), 0),
	//	DirectX::XMFLOAT3(GetRight(), GetDown(), 0));

	//mLines[2]->SetPoints(
	//	DirectX::XMFLOAT3(GetRight(), GetDown(), 0),
	//	DirectX::XMFLOAT3(GetLeft(), GetDown(), 0));

	//mLines[3]->SetPoints(
	//	DirectX::XMFLOAT3(GetLeft(), GetDown(), 0),
	//	DirectX::XMFLOAT3(GetLeft(), GetUp(), 0));
}
#endif