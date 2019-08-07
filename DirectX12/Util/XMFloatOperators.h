#pragma once
#ifndef __MXF3OP__
#define __XMF3OP__
#include <DirectXMath.h>

DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMFLOAT3 operator+=(DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMFLOAT3 operator-=(DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& lval, float rato);

DirectX::XMFLOAT3 operator*(float rato, const DirectX::XMFLOAT3& rval);

DirectX::XMFLOAT3 operator*=(DirectX::XMFLOAT3& lval, float rato);

DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& lval, float rato);

DirectX::XMFLOAT3 operator/=(DirectX::XMFLOAT3& lval, float rato);

DirectX::XMFLOAT3 Normalize(const DirectX::XMFLOAT3& val);

float Dot(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMFLOAT3 Cross(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMFLOAT4X4 CreateQuoternion(const DirectX::XMFLOAT3& axis, float rad);

DirectX::XMVECTOR CreateQuoternionXMVEC(const DirectX::XMFLOAT3& axis, float rad);

DirectX::XMFLOAT4 CreateQuoternionXMFloat4(const DirectX::XMFLOAT3& axis, float rad);

DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& val);

bool operator==(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

bool operator!=(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMFLOAT4 operator+=(DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMFLOAT4 operator+=(DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT4& rval);

DirectX::XMFLOAT4 operator-=(DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT4& rval);

DirectX::XMFLOAT4X4 operator*(const DirectX::XMFLOAT4X4& lval, const float rval);

DirectX::XMFLOAT4X4 operator*( const float lval, const DirectX::XMFLOAT4X4& rval);

DirectX::XMFLOAT4X4 operator+(const DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval);

DirectX::XMFLOAT4X4 operator+=(DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval);

DirectX::XMFLOAT4X4 operator-(const DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval);

DirectX::XMFLOAT4X4 operator-=(DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval);

DirectX::XMFLOAT4X4 operator-(const DirectX::XMFLOAT4X4& val);

DirectX::XMFLOAT4 operator*(const DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT4X4& rval);

DirectX::XMFLOAT4 operator*=(DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT4X4& rval);

DirectX::XMFLOAT4X4 operator*(const DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval);

DirectX::XMFLOAT4X4 operator*=(DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval);

float GetLength(const DirectX::XMFLOAT3& val);

DirectX::XMFLOAT4 ConvertToXMFloat4(const DirectX::XMFLOAT3& val);

DirectX::XMFLOAT3 ConvertToXMFloat3(const DirectX::XMFLOAT4& val);

DirectX::XMFLOAT4X4 ConvertToXMFloat4x4(const DirectX::XMMATRIX& val);

DirectX::XMMATRIX ConvertToXMMatrix(const DirectX::XMFLOAT4X4& val);

DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& xmf3, const DirectX::XMFLOAT4& xmf4);

DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& xmf4, const DirectX::XMFLOAT3& xmf3);

DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT4& rval);

DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& xmf3, const DirectX::XMFLOAT4& xmf4);

DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4& xmf4, const DirectX::XMFLOAT3& xmf3);

DirectX::XMFLOAT4 operator/(const DirectX::XMFLOAT4& xmf4, const float val);

DirectX::XMFLOAT4 operator*(const DirectX::XMFLOAT4& xmf4, const float val);

DirectX::XMFLOAT4 operator*(const float val, const DirectX::XMFLOAT4& xmf4);

DirectX::XMFLOAT4 operator*=(const float val, DirectX::XMFLOAT4& xmf4);

DirectX::XMFLOAT4 operator*=(DirectX::XMFLOAT4& xmf4, const float val);

DirectX::XMFLOAT4 operator/=(DirectX::XMFLOAT4& xmf4, const float val);

DirectX::XMFLOAT4X4 operator*=(DirectX::XMFLOAT4X4& f44, const float val);

bool operator==(const DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT4& rval);

DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT4& rval);

DirectX::XMFLOAT4X4 Inverse(const DirectX::XMFLOAT4X4& matrix);

DirectX::XMFLOAT4X4 IdentityXMFloat4x4();

DirectX::XMFLOAT3 operator*=(DirectX::XMFLOAT3& xmf3, const DirectX::XMFLOAT4X4 mat);

DirectX::XMFLOAT2 Normalize(const DirectX::XMFLOAT2& val);

float GetLength(const DirectX::XMFLOAT2& val);

float Dot(const DirectX::XMFLOAT2& lval, const DirectX::XMFLOAT2 rval);

DirectX::XMFLOAT2 operator-(const DirectX::XMFLOAT2& lval, const DirectX::XMFLOAT2& rval);
DirectX::XMFLOAT2 operator+(const DirectX::XMFLOAT2& lval, const DirectX::XMFLOAT2& rval);
DirectX::XMFLOAT2 operator-=(DirectX::XMFLOAT2& lval, const DirectX::XMFLOAT2& rval);
DirectX::XMFLOAT2 operator+=(DirectX::XMFLOAT2& lval, const DirectX::XMFLOAT2& rval);
DirectX::XMFLOAT2 operator*(const DirectX::XMFLOAT2& xmf2, const float val);
DirectX::XMFLOAT2 operator/(const DirectX::XMFLOAT2& xmf2, const float val);
DirectX::XMFLOAT2 operator*=(DirectX::XMFLOAT2& xmf2, const float val);
DirectX::XMFLOAT2 operator/=(DirectX::XMFLOAT2& xmf2, const float val);
#endif