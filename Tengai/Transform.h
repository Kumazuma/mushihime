#pragma once
#include<xmemory>
#include<DirectXMath.h>
#include<Windows.h>
inline RECT operator + (const RECT& rc, const DirectX::XMFLOAT2& pos)
{
	return { (int)pos.x + rc.left, (int)pos.y + rc.top, (int)pos.x + rc.right, (int)pos.y + rc.bottom };
}
inline DirectX::XMVECTOR operator + (const DirectX::XMVECTOR& v, const DirectX::XMVECTOR& u)
{
	return DirectX::XMVectorAdd(v, u);
}
inline DirectX::XMVECTOR operator -(const DirectX::XMVECTOR& v, const DirectX::XMVECTOR& u)
{
	return DirectX::XMVectorSubtract(v, u);
}
inline DirectX::XMVECTOR operator * (float val, const DirectX::XMVECTOR& v)
{
	return DirectX::XMVectorScale(v, val);
}
inline DirectX::XMFLOAT2 operator + (const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
	return DirectX::XMFLOAT2{ v1.x + v2.x, v1.y + v2.y };
}