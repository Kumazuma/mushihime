#pragma once
#include "Character.h"
#include "Transform.h"
enum class BulletType
{
	NONE,
	_01,
	_02,
	_03,
	_04,
	_05,
	_06,
	_07,
	_08,
	_09,
	_10,
	_11,

	END
};
class MetaBullet
{
public:
	static void Initialize(std::shared_ptr<GameObject>& _pObject, BulletType _type, const DirectX::XMFLOAT2 & pos, float rad, bool _isAlias);
};
class Bullet : public Character
{
public:
	Bullet();
	// GameObject을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Move();
	virtual void Render() override;
	void OnCollision(const CollisionEvent& event);

	BulletType bulletType;
	bool isAlias;
	float radian;
	float radian2 = 0.f;
private:

};
class Bullet01 : public Bullet
{
public:
	Bullet01();
	virtual void Update() override;
	virtual void Render() override;
};
class Bullet02 : public Bullet
{
public:
	Bullet02();
	virtual void Update() override;
	virtual void Render() override;
};
class Bullet03 : public Bullet
{
public:
	Bullet03();
	virtual void Update() override;
	virtual void Render() override;
};
class Bullet04 : public Bullet
{
public:
	Bullet04();
	virtual void Update() override;
	virtual void Render() override;
};
class Bullet05 : public Bullet
{
public:
	Bullet05();
	virtual void Update() override;
	virtual void Render() override;
};
class Bullet06 : public Bullet
{
public:
	Bullet06();
	virtual void Update() override;
	virtual void Render() override;
};
class Bullet07 : public Bullet
{
public:
	Bullet07();
	virtual void Update() override;
	virtual void Render() override;

private:
	float m_fStackTime;
};

class Bullet08 : public Bullet
{
public:
	Bullet08();
	virtual void Update() override;
	virtual void Render() override;

private:
	float m_fStackTime;
	int m_iCheckNum;
};

class Bullet09 : public Bullet
{
public:
	Bullet09();
	virtual void Update() override;
	virtual void Render() override;

private:
	float m_fStackTime;
	int m_iCheckNum;
};

// 일정시간 유도탄
class Bullet010 : public Bullet
{
public:
	Bullet010();
	void Initialize(const DirectX::XMFLOAT2& pos, float radAngle);
	virtual void Update() override;
	virtual void Render() override;

private:
	float m_fStackTime;
	float m_fAlphaNum; 

	float m_fScalingStackTime;
	float m_fAddValue;
};

class Bullet011 : public Bullet
{
public:
	Bullet011();
	virtual void Update() override;
	virtual void Render() override;
};