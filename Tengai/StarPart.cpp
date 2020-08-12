#include "stdafx.h"
#include "StarPart.h"
#include "RenderManager.h"
#include "item.h"
#include "GameWinBox.h"

StarPart::StarPart()
{
	this->type = ObjectType::MONSTER;
	currentFireState = nullptr;
	currentMoveState = nullptr;
}

GameObject* StarPart::Initialize(GameObject* const obj, MonsterType monsterType, const DirectX::XMFLOAT2& firstPos, const float BasicDegree)
{
	if (obj->type == ObjectType::MONSTER)
	{
		Monster* self = static_cast<Monster*>(obj);
		dynamic_cast<StarPart*>(self)->Initialize(monsterType, firstPos, BasicDegree);
	}
	return obj;
}

void StarPart::Initialize(MonsterType _monsterType, const DirectX::XMFLOAT2& firstPos, const float BasicDegree)
{
	Bind(EventId::PASS_TIME, &StarPart::OnShow);
	Bind(EventId::COLLISION_OBJ, &StarPart::OnCollision);
	position = firstPos;
	isEnable = false;
	m_BasicDegree = BasicDegree;
	for (auto it : fireStateList)
	{
		delete it;
	}
	fireStateList.clear();
	for (auto it : moveStateList)
	{
		delete it;
	}
	moveStateList.clear();

	//��Ÿ ������ �κ�
	moveStateList.push_back(
		new BezierCurveMoveToState{ this, firstPos + DirectX::XMFLOAT2{0.f, 0.f} ,firstPos + DirectX::XMFLOAT2{ -1.f * WINDOW_WIDTH / 4, 700.f } ,firstPos + DirectX::XMFLOAT2{ 0.f,  400.f} }
	);
	moveStateList.push_back(
		new BezierCurveMoveToState{ this, firstPos + DirectX::XMFLOAT2{0.f, 400.f} ,firstPos + DirectX::XMFLOAT2{ 1.f * WINDOW_WIDTH / 4 , 150.f } ,firstPos + DirectX::XMFLOAT2{ 0.f,  100.f} }
	);
	moveStateList.push_back(
		new BezierCurveMoveToState{ this, firstPos + DirectX::XMFLOAT2{ 0.f, 100.f} ,firstPos + DirectX::XMFLOAT2{ 1.f * WINDOW_WIDTH / 4,  150.f} ,firstPos + DirectX::XMFLOAT2{ 0.f,  400.f} }
	);
	speed = 50;
	moveStateList[0]->pNextState = moveStateList[1];
	moveStateList[1]->pNextState = moveStateList[2];
	moveStateList[2]->pNextState = moveStateList[1];

	DirectX::XMMATRIX matStar, Scale, RotZ, Trans, RevZ, Parent;
	matStar = DirectX::XMMatrixIdentity();
	Scale = DirectX::XMMatrixScaling(1.f, 1.f, 0.f);
	RotZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(0.f));
	Trans = DirectX::XMMatrixTranslation(0.f, -50.f, 0.f);
	RevZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_BasicDegree + m_Degree));
	Parent = DirectX::XMMatrixTranslation(position.x, position.y, 0.f);
	matStar = Scale * RotZ * Trans * RevZ * Parent;

	auto s{ DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, 0.f, 0.f), Parent) };
	DirectX::XMStoreFloat2(&position, s);

	monsterRect = RECT{ -18, -18, 18, 18 };
	colliders.push_back(RECT{ -16, -16, 16, 16 });
	hp = 100;

	monsterType = _monsterType;
	simpleCollider = CreateSimpleCollider(colliders);
	currentMoveState = this->moveStateList.front();
	currentMoveState->Reset();
	isEnable = true;
}

StarPart::~StarPart()
{
	for (auto it : fireStateList)
	{
		delete it;
	}
	fireStateList.clear();
	for (auto it : moveStateList)
	{
		delete it;
	}
	moveStateList.clear();
}

void StarPart::Update()
{
	if (isEnable == false)
	{
		return;
	}
	if (currentMoveState != nullptr)
	{
		if (currentMoveState->Update())
		{
			auto pNext = currentMoveState->pNextState;
			if (pNext != nullptr)
			{
				pNext->Reset();
				currentMoveState = pNext;
			}
		}
	}
	if (currentFireState != nullptr)
	{
		if (currentFireState->Update())
		{
			auto pNext = currentFireState->pNextState;
			if (pNext != nullptr)
			{
				pNext->Reset();
				currentFireState = pNext;
			}
		}
	}
		//position.x += 1;
		m_Degree += 1.f;
	
}

void StarPart::Render()
{
	DirectX::XMVECTOR triangle[3]{
			{0.f, -20.f, 0.f },
			{ -20.f, 20.f , 0.f },
			{ 20.f, 20.f, 0.f }
	};

	auto scale{ DirectX::XMMatrixScaling(1.f, 1.f, 0.f) };
	auto rot{ DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(0.f)) };
	auto trans{ DirectX::XMMatrixTranslation(0.f, -50.f, 0.f) };
	auto RevZ{ DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_BasicDegree + m_Degree)) };
	auto Parent = DirectX::XMMatrixTranslation(position.x, position.y, 0.f);
	auto transform = scale * rot * trans * RevZ * Parent;
	for (int i = 0; i < 3; ++i)
	{
		triangle[i] = DirectX::XMVector3TransformCoord(triangle[i], transform);
	}
	RenderManager::DrawTriangle(triangle);
	RenderManager::DrawCircle(monsterRect + position);
}

void StarPart::OnShow(const Event&)
{
	isEnable = true;
}

void StarPart::OnCollision(const CollisionEvent& event)
{
	if (event.other->type == ObjectType::BULLET && this->isEnable)
	{
		const Bullet* const pBullet = (const Bullet*)event.other;
		hp -= pBullet->isAlias;
		if (hp <= 0)
		{
			Die();
			if (monsterType == MonsterType::BOSS)
			{
				GameObject::Die();
				UI* ui = (UI*)ObjectManager::CreateObject<GameWinBox>(ObjectType::UI);
				if (SceneManager::GetInstance()->pCurrentScene->ShowBox(ui) == false)
				{
					SceneManager::GetInstance()->pCurrentScene->HideBox();
					SceneManager::GetInstance()->pCurrentScene->ShowBox(ui);
				}
				ui->Show();
				MainGame::Pause();
				return;
			}
			//�������� �����Ѵ�.
			ItemType itemType = ItemType::NONE;
			switch (rand() % 2)
			{
			case 0:
				itemType = ItemType::HEAL;
				break;
			case 1:
				itemType = ItemType::SKILL;
				break;
			}
			Item::Initialize(ObjectManager::CreateObject(ObjectType::ITEM), itemType, position);
		}
	}
}
