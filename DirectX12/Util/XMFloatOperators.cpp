#include "stdafx.h"
#include "XMFloatOperators.h"

DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval)
{
	return DirectX::XMFLOAT3(lval.x + rval.x, lval.y + rval.y, lval.z + rval.z);
}

DirectX::XMFLOAT3 operator+=(DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval)
{
	lval = lval + rval;
	return lval;
}

DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval)
{
	return DirectX::XMFLOAT3(lval.x - rval.x, lval.y - rval.y, lval.z - rval.z);
}

DirectX::XMFLOAT3 operator-=(DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval)
{
	lval = lval - rval;
	return lval;
}

DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& lval, float rato)
{
	return DirectX::XMFLOAT3(lval.x * rato, lval.y * rato, lval.z * rato);
}

DirectX::XMFLOAT3 operator*(float rato, const DirectX::XMFLOAT3& rval)
{
	return rval * rato;
}

DirectX::XMFLOAT3 operator*=(DirectX::XMFLOAT3& lval, float rato)
{
	lval = lval * rato;
	return lval;
}

DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& lval, float rato)
{
	return DirectX::XMFLOAT3(lval.x / rato, lval.y / rato, lval.z / rato);
}

DirectX::XMFLOAT3 operator/=(DirectX::XMFLOAT3& lval, float rato)
{
	lval = lval / rato;
	return lval;
}

DirectX::XMFLOAT3 NormalizeXMFloat3(const DirectX::XMFLOAT3& val)
{
	float len = sqrtf(val.x * val.x + val.y * val.y + val.z * val.z);
	return DirectX::XMFLOAT3(val.x / len, val.y / len, val.z / len);
}

float DotXMFloat3(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval)
{
	return lval.x * rval.x + lval.y * rval.y + lval.z * rval.z;
}

DirectX::XMFLOAT3 CrossXMFloat3(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval)
{
	return  NormalizeXMFloat3(DirectX::XMFLOAT3(lval.y * rval.z - lval.z * rval.y, lval.z * rval.x - lval.x * rval.z, lval.x * rval.y - lval.y * rval.x));
}

DirectX::XMVECTOR CreateQuoternion(const DirectX::XMFLOAT3& axis, float deg)
{
	return DirectX::XMLoadFloat4(&CreateQuoternionXMFloat4(axis,deg));
}

DirectX::XMFLOAT4 CreateQuoternionXMFloat4(const DirectX::XMFLOAT3& axis, float deg)
{
	DirectX::XMFLOAT3 normAxis = NormalizeXMFloat3(axis);
	return DirectX::XMFLOAT4(
		normAxis.x * sinf(DirectX::XMConvertToRadians(deg / 2.0f)),
		normAxis.y * sinf(DirectX::XMConvertToRadians(deg / 2.0f)),
		normAxis.z * sinf(DirectX::XMConvertToRadians(deg / 2.0f)),
		cosf(DirectX::XMConvertToRadians(deg / 2.0f)));
}

DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& val)
{
	return -1.0f * val;
}

bool operator==(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval)
{
	return (lval.x == rval.x && lval.y == rval.y && lval.z == rval.z);
}

bool operator!=(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval)
{
	return !(lval == rval);
}

DirectX::XMFLOAT4 operator+=(DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT3& rval)
{
	lval.x += rval.x; lval.y += rval.y; lval.z += rval.z;
	return lval;
}

DirectX::XMFLOAT4 operator+=(DirectX::XMFLOAT4 & lval, const DirectX::XMFLOAT4 & rval)
{
	lval.x += rval.x;
	lval.y += rval.y;
	lval.z += rval.z;
	lval.w = 1;
	return lval;
}

DirectX::XMFLOAT4X4 operator*(const DirectX::XMFLOAT4X4& lval, const float rval)
{
	return DirectX::XMFLOAT4X4(
		lval._11 * rval, lval._12 * rval, lval._13 * rval, lval._14 * rval,
		lval._21 * rval, lval._22 * rval, lval._23 * rval, lval._24 * rval,
		lval._31 * rval, lval._32 * rval, lval._33 * rval, lval._34 * rval,
		lval._41 * rval, lval._42 * rval, lval._43 * rval, lval._44 * rval);
}

DirectX::XMFLOAT4X4 operator*(const float lval, const DirectX::XMFLOAT4X4& rval)
{
	return rval * lval;
}

DirectX::XMFLOAT4X4 operator+(const DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval)
{
	return DirectX::XMFLOAT4X4(
		rval._11 + lval._11, rval._12 + lval._12, rval._13 + lval._13, rval._14 + lval._14,
		rval._21 + lval._21, rval._22 + lval._22, rval._23 + lval._23, rval._24 + lval._24,
		rval._31 + lval._31, rval._32 + lval._32, rval._33 + lval._33, rval._34 + lval._34,
		rval._41 + lval._41, rval._42 + lval._42, rval._43 + lval._43, rval._44 + lval._44);
}

DirectX::XMFLOAT4X4 operator+=(DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval)
{
	lval = lval + rval;
	return lval;
}

DirectX::XMFLOAT4X4 operator-(const DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval)
{
	return DirectX::XMFLOAT4X4(
		rval._11 - lval._11, rval._12 - lval._12, rval._13 - lval._13, rval._14 - lval._14,
		rval._21 - lval._21, rval._22 - lval._22, rval._23 - lval._23, rval._24 - lval._24,
		rval._31 - lval._31, rval._32 - lval._32, rval._33 - lval._33, rval._34 - lval._34,
		rval._41 - lval._41, rval._42 - lval._42, rval._43 - lval._43, rval._44 - lval._44);
}

DirectX::XMFLOAT4X4 operator-=(DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval)
{
	lval = lval - rval;
	return lval;
}

DirectX::XMFLOAT4 operator*(const DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT4X4& rval)
{
	return DirectX::XMFLOAT4(
		lval.x * rval._11 + lval.y * rval._21 + lval.z * rval._31 + lval.w * rval._41,
		lval.x * rval._12 + lval.y * rval._22 + lval.z * rval._32 + lval.w * rval._42,
		lval.x * rval._13 + lval.y * rval._23 + lval.z * rval._33 + lval.w * rval._43,
		lval.x * rval._14 + lval.y * rval._24 + lval.z * rval._34 + lval.w * rval._44);
}

DirectX::XMFLOAT4 operator*=(DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT4X4& rval)
{
	lval = lval * rval;
	return lval;
}

DirectX::XMFLOAT4X4 operator*(const DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval)
{
	return DirectX::XMFLOAT4X4(
		(lval._11 * rval._11) + (lval._12 * rval._21) + (lval._13 * rval._31) + (lval._14 * rval._41),
		(lval._11 * rval._12) + (lval._12 * rval._22) + (lval._13 * rval._32) + (lval._14 * rval._42),
		(lval._11 * rval._13) + (lval._12 * rval._23) + (lval._13 * rval._33) + (lval._14 * rval._43),
		(lval._11 * rval._14) + (lval._12 * rval._24) + (lval._13 * rval._34) + (lval._14 * rval._44),

		(lval._21 * rval._11) + (lval._22 * rval._21) + (lval._23 * rval._31) + (lval._24 * rval._41),
		(lval._21 * rval._12) + (lval._22 * rval._22) + (lval._23 * rval._32) + (lval._24 * rval._42),
		(lval._21 * rval._13) + (lval._22 * rval._23) + (lval._23 * rval._33) + (lval._24 * rval._43),
		(lval._21 * rval._14) + (lval._22 * rval._24) + (lval._23 * rval._34) + (lval._24 * rval._44),

		(lval._31 * rval._11) + (lval._32 * rval._21) + (lval._33 * rval._31) + (lval._34 * rval._41),
		(lval._31 * rval._12) + (lval._32 * rval._22) + (lval._33 * rval._32) + (lval._34 * rval._42),
		(lval._31 * rval._13) + (lval._32 * rval._23) + (lval._33 * rval._33) + (lval._34 * rval._43),
		(lval._31 * rval._14) + (lval._32 * rval._24) + (lval._33 * rval._34) + (lval._34 * rval._44),

		(lval._41 * rval._11) + (lval._42 * rval._21) + (lval._43 * rval._31) + (lval._44 * rval._41),
		(lval._41 * rval._12) + (lval._42 * rval._22) + (lval._43 * rval._32) + (lval._44 * rval._42),
		(lval._41 * rval._13) + (lval._42 * rval._23) + (lval._43 * rval._33) + (lval._44 * rval._43),
		(lval._41 * rval._14) + (lval._42 * rval._24) + (lval._43 * rval._34) + (lval._44 * rval._44)
		);
}

DirectX::XMFLOAT4X4 operator*=(DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval)
{
	lval = lval * rval;
	return lval;
}

float GetLengthXMFloat3(const DirectX::XMFLOAT3& val)
{
	return sqrtf(DotXMFloat3(val, val));
}

DirectX::XMFLOAT4 ConvertXMFloat3ToXMFloat4(const DirectX::XMFLOAT3& val)
{
	return DirectX::XMFLOAT4(val.x, val.y, val.z, 1);
}

DirectX::XMFLOAT3 ConvertXMFloat4ToXMFloat3(const DirectX::XMFLOAT4 & val)
{
	return DirectX::XMFLOAT3(val.x, val.y, val.z);
}

DirectX::XMFLOAT4X4 ConvertXMMATRIXToXMFloat4x4(const DirectX::XMMATRIX& val)
{
	DirectX::XMFLOAT4X4 rtn;
	rtn._11 = val.r[0].m128_f32[0]; rtn._12 = val.r[0].m128_f32[1]; rtn._13 = val.r[0].m128_f32[2]; rtn._14 = val.r[0].m128_f32[3];
	rtn._21 = val.r[1].m128_f32[0]; rtn._22 = val.r[1].m128_f32[1]; rtn._23 = val.r[1].m128_f32[2]; rtn._24 = val.r[1].m128_f32[3];
	rtn._31 = val.r[2].m128_f32[0]; rtn._32 = val.r[2].m128_f32[1]; rtn._33 = val.r[2].m128_f32[2]; rtn._34 = val.r[2].m128_f32[3];
	rtn._41 = val.r[3].m128_f32[0]; rtn._42 = val.r[3].m128_f32[1]; rtn._43 = val.r[3].m128_f32[2]; rtn._44 = val.r[3].m128_f32[3];
	return rtn;
}

DirectX::XMMATRIX ConvertXMFloat4x4ToXMMatrix(const DirectX::XMFLOAT4X4& val)
{
	DirectX::XMMATRIX rtn;
	rtn.r[0].m128_f32[0] = val._11; rtn.r[0].m128_f32[1] = val._12; rtn.r[0].m128_f32[2] = val._13; rtn.r[0].m128_f32[3] = val._14;
	rtn.r[1].m128_f32[0] = val._21; rtn.r[1].m128_f32[1] = val._22; rtn.r[1].m128_f32[2] = val._23; rtn.r[1].m128_f32[3] = val._24;
	rtn.r[2].m128_f32[0] = val._31; rtn.r[2].m128_f32[1] = val._32; rtn.r[2].m128_f32[2] = val._33; rtn.r[2].m128_f32[3] = val._34;
	rtn.r[3].m128_f32[0] = val._41; rtn.r[3].m128_f32[1] = val._42; rtn.r[3].m128_f32[2] = val._43; rtn.r[3].m128_f32[3] = val._44;
	return rtn;
}

DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& xmf3, const DirectX::XMFLOAT4& xmf4)
{
	return DirectX::XMFLOAT3(xmf3.x - xmf4.x,  xmf3.y - xmf4.y, xmf3.z - xmf4.z);
}

DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& xmf4, const DirectX::XMFLOAT3& xmf3)
{
	return DirectX::XMFLOAT4(xmf4.x - xmf3.x, xmf4.y - xmf3.y, xmf4.z - xmf3.z, 1.0f);
}

DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4 & lval, const DirectX::XMFLOAT4 & rval)
{
	return DirectX::XMFLOAT4(lval.x - rval.x, lval.y - rval.y, lval.z - rval.z, 1.0f);
}

DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& xmf3, const DirectX::XMFLOAT4& xmf4)
{
	return DirectX::XMFLOAT3(xmf3.x + xmf4.x, xmf3.y + xmf4.y, xmf3.z + xmf4.z);
}

DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4& xmf4, const DirectX::XMFLOAT3& xmf3)
{
	return DirectX::XMFLOAT4(xmf4.x + xmf3.x, xmf4.y + xmf3.y, xmf4.z + xmf3.z, 1.0f);
}

DirectX::XMFLOAT4 operator/(const DirectX::XMFLOAT4 & xmf4, const float val)
{
	return DirectX::XMFLOAT4(xmf4.x / val, xmf4.y / val, xmf4.z / val, 1.0f);
}

DirectX::XMFLOAT4 operator*(const DirectX::XMFLOAT4 & xmf4, const float val)
{
	return DirectX::XMFLOAT4(xmf4.x * val, xmf4.y * val, xmf4.z * val, 1.0f);
}

DirectX::XMFLOAT4 operator*(const float val, const DirectX::XMFLOAT4 & xmf4)
{
	return xmf4 * val;
}

DirectX::XMFLOAT4 operator*=(const float val, DirectX::XMFLOAT4 & xmf4)
{
	return xmf4 = xmf4 * val;
}

DirectX::XMFLOAT4 operator*=(DirectX::XMFLOAT4 & xmf4, const float val)
{
	return xmf4 = xmf4 * val;
}

DirectX::XMFLOAT4 operator/=(DirectX::XMFLOAT4 & xmf4, const float val)
{
	return xmf4 = xmf4 / val;
}

DirectX::XMFLOAT4X4 operator*=(DirectX::XMFLOAT4X4 & f44, const float scaler)
{
	return f44 = f44 * scaler;
}

bool operator==(const DirectX::XMFLOAT4 & lval, const DirectX::XMFLOAT4 & rval)
{
	return lval.x == rval.x && lval.y == rval.y && lval.z == rval.z && lval.w == rval.w;
}

DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4 & lval, const DirectX::XMFLOAT4 & rval)
{
	return DirectX::XMFLOAT4(lval.x + rval.x, lval.y + rval.y, lval.z + rval.z , 1.0f);
}

DirectX::XMFLOAT4X4 InverseXMFloat4x4(const DirectX::XMFLOAT4X4 & matrix)
{
	return ConvertXMMATRIXToXMFloat4x4(DirectX::XMMatrixInverse(nullptr, ConvertXMFloat4x4ToXMMatrix(matrix)));
}

DirectX::XMFLOAT4X4 IdentityXMFloat4x4()
{
	return DirectX::XMFLOAT4X4(1.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 1.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 1.0f, 0.0f,
							   0.0f, 0.0f, 0.0f, 1.0f);
}

DirectX::XMFLOAT3 operator*=(DirectX::XMFLOAT3& xmf3, const DirectX::XMFLOAT4X4 mat)
{
	auto xmf4 = DirectX::XMFLOAT4(xmf3.x, xmf3.y, xmf3.z, 1.0f);
	xmf4 *= mat;
	xmf3 = { xmf4.x, xmf4.y, xmf4.z };
	return xmf3;
}

DirectX::XMFLOAT2 NormalizeXMFloat2(const DirectX::XMFLOAT2 & val)
{
	float length = GetLengthXMFloat2(val);
	return DirectX::XMFLOAT2(val.x / length, val.y /length);
}

float GetLengthXMFloat2(const DirectX::XMFLOAT2 & val)
{
	return sqrt(val.x * val.x + val.y * val.y);
}

float DotXMFloat2(const DirectX::XMFLOAT2 & lval, const DirectX::XMFLOAT2 rval)
{
	return lval.x * rval.x + lval.y * rval.y;
}

