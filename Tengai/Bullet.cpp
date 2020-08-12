#include "stdafx.h"
#include "Bullet.h"
#include "RenderManager.h"
#include "Transform.h"
#include <xmemory>
#include"GameObject.h"

Bullet::Bullet()
{
	type = ObjectType::BULLET;
	Bind(EventId::COLLISION_OBJ, &Bullet::OnCollision);
	simpleCollider = RECT{ -8,-8,8,8 };
}

void Bullet::Update()
{
	if (position.x <= 0 || position.y <= 0)
	{
		ObjectManager::DeleteObject(this);
	}
	if(position.x >= WINDOW_WIDTH || position.y >= WINDOW_HEIGHT)
	{ 
		ObjectManager::DeleteObject(this);
	}
}

void Bullet::Move()
{
	const float moveDistance = TimeManager::DeltaTime()* speed;
	DirectX::XMVECTOR pos{DirectX::XMLoadFloat2(&position)};
	auto mat{
		DirectX::XMMatrixTranslation(moveDistance, 0.f, 0.f) *
		DirectX::XMMatrixRotationZ(radian) * 
		DirectX::XMMatrixTranslationFromVector(pos)
	};
	pos = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f), mat);
	DirectX::XMStoreFloat2(&position, pos);
}

void Bullet::Render()
{
}

void Bullet::OnCollision(const CollisionEvent& event)
{
	if (event.other->type != ObjectType::BULLET)
	{
		if (event.other->type == ObjectType::MONSTER && this->isAlias == true)
		{
			ObjectManager::DeleteObject(this);
		}
		if (event.other->type == ObjectType::PLAYER && this->isAlias == false)
		{
			ObjectManager::DeleteObject(this);
		}
	}
}

void MetaBullet::Initialize(GameObject* _pObject, BulletType _type, const DirectX::XMFLOAT2& pos, float rad, bool _isAlias)
{
	if (_pObject == nullptr)
	{
		return;
	}
	if (_pObject->type != ObjectType::BULLET)
	{
		return;
	}
	Bullet* pBullet = (Bullet*)_pObject;
	auto uid = pBullet->uid;
	switch (_type)
	{
	case BulletType::_01:
		pBullet->~Bullet();
		new(pBullet) Bullet01{};
		break;
	case BulletType::_02:
		pBullet->~Bullet();
		new(pBullet) Bullet02{};
		break;
	case BulletType::_03:
		pBullet->~Bullet();
		new(pBullet) Bullet03{};
		break;
	case BulletType::_04:
		pBullet->~Bullet();
		new(pBullet) Bullet04{};
		break;
	case BulletType::_05:
		pBullet->~Bullet();
		new(pBullet) Bullet05{};
		break;
	case BulletType::_06:
		pBullet->~Bullet();
		new(pBullet) Bullet06{};
		break;
	case BulletType::_07:
		pBullet->~Bullet();
		new(pBullet) Bullet07{};
		break;
	case BulletType::_08:
		pBullet->~Bullet();
		new(pBullet) Bullet08{};
		break;
	case BulletType::_09:
		pBullet->~Bullet();
		new(pBullet) Bullet09{};
		break;
	case BulletType::_10:
		pBullet->~Bullet();
		new(pBullet) Bullet010{};
		static_cast<Bullet010*>(pBullet)->Initialize(pos, rad);
		break;
		
	}
	pBullet->uid = uid;
	pBullet->position = pos;
	pBullet->radian = rad;
	pBullet->bulletType = _type;
	pBullet->isAlias = _isAlias;
}

Bullet01::Bullet01()
{
	hp = 999;
	speed = 150;
	colliders.push_back(RECT{ -3,-3,3, 3 });
}

Bullet02::Bullet02()
{
	hp = 999;
	speed = 200;
	colliders.push_back(RECT{ -3,-3,3, 3 });
}

Bullet03::Bullet03()
{
	hp = 999;
	speed = 200;
	colliders.push_back(RECT{ -6,-6,6,6 });
}
void Bullet01::Update()
{
	Move();
	Bullet::Update();
}
void Bullet02::Update()
{
	radian += PI * 0.5f / 180;
	Move();
	Bullet::Update();
}
void Bullet03::Update()
{
	Move();
	Bullet::Update();
}
void Bullet01::Render()
{
	RenderManager::DrawCircle(RECT{ -6, -4, 6, 4 } +position, RGB(255, 130, 0), RGB(255, 0, 0));
}
void Bullet02::Render()
{
	RenderManager::DrawCircle(RECT{ -8, -8, 8, 8 } + position, RGB(255, 130, 0), RGB(255, 0, 0));
}

void Bullet03::Render()
{
	RenderManager::DrawCircle(RECT{ -8, -8, 8, 8 } + position, RGB(255, 130, 0), RGB(255, 0, 0));
}

Bullet04::Bullet04()
{
	hp = 999;
	speed = 800;
	colliders.push_back(RECT{ -15,-3,15,3 });
}

void Bullet04::Update()
{
	Move();
	Bullet::Update();
}

void Bullet04::Render()
{
	RenderManager::DrawCircle(RECT{ -5, -30, 5, 30 } + position, RGB(100, 200, 255), RGB(0, 0, 255));
}

Bullet05::Bullet05()
{
	hp = 999;
	speed = 500;
	colliders.push_back(RECT{ -4,-4,4,4 });
}

void Bullet05::Update()
{
	radian += PI * 1.f * 15.f / 180;
	position.x += 10.f * sinf(radian);
	position.y -= speed * TimeManager::DeltaTime();
	Bullet::Update();
}

void Bullet05::Render()
{
	RenderManager::DrawCircle(RECT{ -5, -5, 5, 5 } + position, RGB(200, 100, 200), RGB(0, 0, 200));
}

Bullet06::Bullet06()
{
	hp = 999;
	speed = 200;
	colliders.push_back(RECT{ -4,-4,4,4 });
}

void Bullet06::Update()
{
	DirectX::XMFLOAT2 direction{};
	
	radian2 += PI * 1.f * 15.f / 180;
	direction.x += cosf(radian) + cosf(radian2);
	direction.y += sinf(radian) + sinf(radian2);
	position.x += direction.x * speed * TimeManager::DeltaTime();
	position.y += direction.y * speed * TimeManager::DeltaTime();
	Bullet::Update();
}

void Bullet06::Render()
{
	RenderManager::DrawCircle(RECT{ -5, -5, 5, 5 } +position, RGB(200, 100, 200), RGB(0, 0, 200));
}

Bullet07::Bullet07()
{
	hp = 999;
	speed = 200;
	colliders.push_back(RECT{ -3,-3,3, 3 });
	m_fStackTime = 0.f;
}

void Bullet07::Update()
{
	m_fStackTime += TimeManager::GetInstance()->DeltaTime();

	if (m_fStackTime > 1.5f)
	{
		Move();
	}
	else
	{
		radian += PI * 1.f * 25.f / 180;
		DirectX::XMVECTOR pos{ DirectX::XMLoadFloat2(&position) };
		auto mat{
			DirectX::XMMatrixTranslation(30.f, 0.f, 0.f) *
			DirectX::XMMatrixRotationZ(radian) *
			DirectX::XMMatrixTranslationFromVector(pos)
		};

		pos = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f), mat);
		DirectX::XMStoreFloat2(&position, pos);
	}

	Bullet::Update();
}

void Bullet07::Render()
{
	if (m_fStackTime > 1.5f)
		RenderManager::DrawCircle(RECT{ -6, -6, 6, 6 } + position, RGB(255, 0, 0), RGB(255, 0, 0));
	else
		RenderManager::DrawCircle(RECT{ -4, -4, 4, 4 } + position, RGB(255, 130, 0), RGB(255, 0, 0));
}

Bullet08::Bullet08()
{
	hp = 999;
	speed = 200;
	colliders.push_back(RECT{ -3,-3,3, 3 });
	m_fStackTime = 0.f;
	m_iCheckNum = 0;
}

void Bullet08::Update()
{
	m_fStackTime += TimeManager::GetInstance()->DeltaTime();

	if (m_fStackTime > 1.5f)
	{
		radian += PI * 0.5f / 180;
		Move();
	}
	else
	{
		m_iCheckNum++;
		// ver 1
		radian += PI * 1.f * 15.f / 180;
		position.x += 3.f * sinf(radian);
		position.y -= speed * TimeManager::DeltaTime();
	}

	Bullet::Update();
}

void Bullet08::Render()
{
	RenderManager::DrawCircle(RECT{ -4, -4, 4, 4 } + position, RGB(255, 130, 0), RGB(255, 0, 0));
}

Bullet09::Bullet09()
{
	hp = 999;
	speed = 200;
	colliders.push_back(RECT{ -3,-3,3, 3 });
	m_fStackTime = 0.f;
	m_iCheckNum = 0;
}

void Bullet09::Update()
{
	m_fStackTime += TimeManager::GetInstance()->DeltaTime();

	if (m_fStackTime > 1.5f)
	{
		radian += PI * 0.5f / 180;
		Move();
	}
	else
	{
		m_iCheckNum++;
		if (m_iCheckNum % 2 == 0)
		{
			radian += PI * 1.f * 15.f / 180;
			position.x -= speed * TimeManager::DeltaTime() * 2.5f;
			position.y -= speed * TimeManager::DeltaTime() * 2.5f;
		}
		else
		{
			radian += PI * 1.f * 15.f / 180;
			position.x += speed * TimeManager::DeltaTime() * 2.5f;
			position.y -= speed * TimeManager::DeltaTime() * 2.5f;
		}
	}

	Bullet::Update();
}

void Bullet09::Render()
{
	if (m_fStackTime > 1.5f)
		RenderManager::DrawCircle(RECT{ -8, -8, 8, 8 } + position, RGB(255, 130, 0), RGB(255, 0, 0));
	else
		RenderManager::DrawCircle(RECT{ -4, -4, 4, 4 } + position, RGB(255, 130, 0), RGB(255, 0, 0));
}

Bullet010::Bullet010()
{
	hp = 999;
	speed = 150;
	colliders.push_back(RECT{ -3,-3,3, 3 });
	m_fAlphaNum = 0.f;
	m_fStackTime = 0.f;
}

void Bullet010::Initialize(const DirectX::XMFLOAT2& pos, float radAngle)
{
	//정보 초기화
	m_info = DirectX::XMMatrixRotationZ(radAngle) * DirectX::XMMatrixTranslation(pos.x, pos.y, 0.f);
}

void Bullet010::Update()
{
	m_fStackTime += TimeManager::GetInstance()->DeltaTime();

	if (m_fStackTime > 4.5f)
	{
		if (m_fStackTime > 6.f)
			ObjectManager::DeleteObject(this);
		Move();
	}
	else
	{
		Character const* const pPlayer = (Character*)ObjectManager::GetInstance()->pPlayer;
		if (pPlayer == nullptr)
			return;
		//이렇게 하면 행렬에서 현재 총알의 XY좌표를 알 수 있다.
		DirectX::XMVECTOR bulletPos{};
		DirectX::XMVECTOR bulletXAxis{ DirectX::XMVector3Transform(DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f), m_info) };
		DirectX::XMVECTOR bulletYAxis{ DirectX::XMVector3Transform(DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f), m_info) };
		DirectX::XMVECTOR playerPos{ DirectX::XMLoadFloat2(&pPlayer->position) };
		bulletPos = DirectX::XMVector3TransformCoord(bulletPos, m_info);
		
		DirectX::XMVECTOR dpos =
			DirectX::XMVector3Normalize(playerPos - bulletPos);
		DirectX::XMVECTOR forward{ DirectX::XMVectorSet(1.f,0.f,0.f,0.f) };
		auto tmp{ DirectX::XMVector3Dot(dpos, bulletXAxis) };
		float cosV = DirectX::XMVectorGetZ(tmp);
		float radian = acosf(cosV);
		if (DirectX::XMConvertToRadians(90.f) >= radian)
		{
			radian = DirectX::XMConvertToRadians(90.f);
		}
		if (DirectX::XMVectorGetY(DirectX::XMVector3Dot(playerPos, bulletYAxis)) < 0.f  )
			radian *= -1.f;
		m_info = DirectX::XMMatrixRotationZ(radian) * m_info;
		m_info = DirectX::XMMatrixTranslation(speed, 0.f, 0.f) * m_info;
		bulletPos = DirectX::XMVector3TransformCoord(bulletPos, m_info);
		DirectX::XMStoreFloat2(&position, bulletPos);
		this->radian = radian;
		Move();
		Bullet::Update();
	}

}

void Bullet010::Render()
{
	Character const* const pPlayer = (Character*)ObjectManager::GetInstance()->pPlayer;
	if (pPlayer == nullptr)
		return;

	DirectX::XMFLOAT2 fVertexArray[3] = 
	{
		{0.f ,  10.f},
		{15.f,  0.f },
		{0.f, -10.f }
	};

	DirectX::XMVECTOR vecVertexArray[3];
	for (int i = 0; i < 3; i++)
		vecVertexArray[i] = DirectX::XMLoadFloat2(&fVertexArray[i]);

	DirectX::XMMATRIX matWorld, matRot, matParent;

	matRot = DirectX::XMMatrixRotationZ(radian);
	matParent = DirectX::XMMatrixTranslation(position.x, position.y, 0.f);
	matWorld = matRot * matParent;

	for (int i = 0; i < 3; i++)
		vecVertexArray[i] = DirectX::XMVector2TransformCoord(vecVertexArray[i], matWorld);


	DirectX::XMFLOAT2 fResultArray[3];
	for (int i = 0; i < 3; i++)
		DirectX::XMStoreFloat2(&fResultArray[i], vecVertexArray[i]);

	if (m_fStackTime > 4.5f)
	{
		m_fAlphaNum += 10.f;
		RenderManager::DrawTriangle(fResultArray, RGB(255, m_fAlphaNum, m_fAlphaNum));
	}
	else
	{
		RenderManager::DrawTriangle(fResultArray, RGB(255, 0, 0));
	}

}
