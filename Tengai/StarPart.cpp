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

std::shared_ptr<GameObject> StarPart::Initialize(const std::shared_ptr<GameObject>& obj, MonsterType monsterType, const DirectX::XMFLOAT2& firstPos, const float BasicDegree)
{
	if (obj->type == ObjectType::MONSTER)
	{
		std::static_pointer_cast<StarPart>(obj)->Initialize(monsterType, firstPos, BasicDegree);
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

	//스타 움직임 부분
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


	
	monsterRect = RECT{ -18, -18, 18, 18 };
	colliders.push_back(RECT{ -16, -16, 16, 16 });
	hp = 1;

	

	DirectX::XMMATRIX matStar, Scale, RotZ, Trans, RevZ, Parent;
	
	Scale = DirectX::XMMatrixScaling(1.f, 1.f, 0.f);
	RotZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(0.f));
	Trans = DirectX::XMMatrixTranslation(0.f, -50.f, 0.f);
	RevZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_BasicDegree + m_Degree));

	matStar = Scale * RotZ * Trans * RevZ * m_parent;

	auto s{ DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, 0.f, 0.f), matStar) };
	DirectX::XMStoreFloat2(&position, s);

	monsterRect = RECT{ -18, -18, 18, 18 };
	colliders.push_back(RECT{ -16, -16, 16, 16 });
	hp = 100;

	monsterType = _monsterType;
	simpleCollider = CreateSimpleCollider(colliders);
	currentMoveState = this->moveStateList.front();
	currentMoveState->Reset();
	isEnable = false;
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
	//if (isEnable == false)
	//{
	//	return;
	//}
	//if (currentMoveState != nullptr)
	//{
	//	if (currentMoveState->Update())
	//	{
	//		auto pNext = currentMoveState->pNextState;
	//		if (pNext != nullptr)
	//		{
	//			pNext->Reset();
	//			currentMoveState = pNext;
	//		}
	//	}
	//}
	//if (currentFireState != nullptr)
	//{
	//	if (currentFireState->Update())
	//	{
	//		auto pNext = currentFireState->pNextState;
	//		if (pNext != nullptr)
	//		{
	//			pNext->Reset();
	//			currentFireState = pNext;
	//		}
	//	}
	//}
		//position.x += 1;
		
	
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
	auto RevZ{ DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_BasicDegree)) };
	auto transform = scale * rot * trans * RevZ * m_parent;
	for (int i = 0; i < 3; ++i)
	{
		triangle[i] = DirectX::XMVector3TransformCoord(triangle[i], transform);
	}
	RenderManager::DrawTriangle(triangle);
	//RenderManager::DrawCircle(monsterRect + position);
}

void StarPart::OnShow(const Event&)
{
	isEnable = true;
}

void StarPart::OnCollision(const CollisionEvent& event)
{
	if (event.other->type == ObjectType::BULLET && this->isEnable)
	{
		auto pBullet = std::static_pointer_cast<Bullet>(event.other);
		hp -= pBullet->isAlias;
		if (hp <= 0)
		{
			Die();
			if (monsterType == MonsterType::BOSS)
			{
				GameObject::Die();
				auto ui = std::static_pointer_cast<UI>(ObjectManager::CreateObject<GameWinBox>(ObjectType::UI));
				if (SceneManager::GetInstance()->pCurrentScene->ShowBox(ui) == false)
				{
					SceneManager::GetInstance()->pCurrentScene->HideBox();
					SceneManager::GetInstance()->pCurrentScene->ShowBox(ui);
				}
				ui->Show();
				MainGame::Pause();
				return;
			}
			//아이템을 생성한다.
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

void StarPart::SetCenter(const DirectX::XMMATRIX& parent)
{
	m_parent = parent;
	DirectX::XMMATRIX matStar, Scale, RotZ, Trans, RevZ, Parent;

	Scale = DirectX::XMMatrixScaling(1.f, 1.f, 0.f);
	RotZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(0.f));
	Trans = DirectX::XMMatrixTranslation(0.f, -50.f, 0.f);
	RevZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_BasicDegree + m_Degree));
	auto position{ DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.f,0.f,0.f,0.f), Scale * Trans * RevZ * m_parent) };
	DirectX::XMStoreFloat2(&this->position, position);
}
