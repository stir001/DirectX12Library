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

DirectX::XMFLOAT3 NormalizeXMFloat3(const DirectX::XMFLOAT3& val);

float DotXMFloat3(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMFLOAT3 CrossXMFloat3(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMVECTOR CreateQuoternion(const DirectX::XMFLOAT3& axis, float deg);

DirectX::XMFLOAT4 CreateQuoternionXMFloat4(const DirectX::XMFLOAT3& axis, float deg);

DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& val);

bool operator==(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

bool operator!=(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMFLOAT4 operator+=(DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMFLOAT4 operator+=(DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT4& rval);

DirectX::XMFLOAT4X4 operator*(const DirectX::XMFLOAT4X4& lval, const float rval);

DirectX::XMFLOAT4X4 operator*( const float lval, const DirectX::XMFLOAT4X4& rval);

DirectX::XMFLOAT4X4 operator+(const DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval);

DirectX::XMFLOAT4X4 operator+=(DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval);

DirectX::XMFLOAT4X4 operator-(const DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval);

DirectX::XMFLOAT4X4 operator-=(DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval);

DirectX::XMFLOAT4 operator*(const DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT4X4& rval);

DirectX::XMFLOAT4 operator*=(DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT4X4& rval);

DirectX::XMFLOAT4X4 operator*(const DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval);

DirectX::XMFLOAT4X4 operator*=(DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval);

float GetLengthXMFloat3(const DirectX::XMFLOAT3& val);

DirectX::XMFLOAT4 ConvertXMFloat3ToXMFloat4(const DirectX::XMFLOAT3& val);

DirectX::XMFLOAT3 ConvertXMFloat4ToXMFloat3(const DirectX::XMFLOAT4& val);

DirectX::XMFLOAT4X4 ConvertXMMATRIXToXMFloat4x4(const DirectX::XMMATRIX& val);

DirectX::XMMATRIX ConvertXMFloat4x4ToXMMatrix(const DirectX::XMFLOAT4X4& val);

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

DirectX::XMFLOAT4X4 InverseXMFloat4x4(const DirectX::XMFLOAT4X4& matrix);

DirectX::XMFLOAT4X4 IdentityXMFloat4x4();

DirectX::XMFLOAT3 operator*=(DirectX::XMFLOAT3& xmf3, const DirectX::XMFLOAT4X4 mat);

DirectX::XMFLOAT2 NormalizeXMFloat2(const DirectX::XMFLOAT2& val);

float GetLengthXMFloat2(const DirectX::XMFLOAT2& val);

float DotXMFloat2(const DirectX::XMFLOAT2& lval, const DirectX::XMFLOAT2 rval);
#endif