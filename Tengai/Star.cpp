#include "stdafx.h"
#include "Star.h"
#include "StarPart.h"
#include "RenderManager.h"
#include "item.h"
#include "GameWinBox.h"

Star::Star()
{
	this->type = ObjectType::MONSTER;
	currentFireState = nullptr;
	currentMoveState = nullptr;
}

std::shared_ptr<GameObject> Star::Initialize(const std::shared_ptr<GameObject>& obj, MonsterType monsterType, const DirectX::XMFLOAT2& firstPos)
{
	
	if (obj->type == ObjectType::MONSTER)
	{
		
		auto self = std::dynamic_pointer_cast<Star>(obj);
		if (self == nullptr)
			Monster::Initialize(obj, monsterType, firstPos);
		else
			self->Initialize(monsterType, firstPos);
	}
	return obj;
}

void Star::Initialize(MonsterType _monsterType, const DirectX::XMFLOAT2& firstPos)
{
	Bind(EventId::PASS_TIME, &Monster::OnShow);
	Bind(EventId::COLLISION_OBJ, &Star::OnCollision);
	position = firstPos;
	m_vecParts = {
		{ StarPart::Initialize(ObjectManager::CreateObject<StarPart>(ObjectType::MONSTER), MonsterType::MOB01, position, 0.f) },
		{ StarPart::Initialize(ObjectManager::CreateObject<StarPart>(ObjectType::MONSTER), MonsterType::MOB01, position, 72.f) },
		{ StarPart::Initialize(ObjectManager::CreateObject<StarPart>(ObjectType::MONSTER), MonsterType::MOB01, position, 144.f) },
		{ StarPart::Initialize(ObjectManager::CreateObject<StarPart>(ObjectType::MONSTER), MonsterType::MOB01, position, 216.f) },
		{ StarPart::Initialize(ObjectManager::CreateObject<StarPart>(ObjectType::MONSTER), MonsterType::MOB01, position, 288.f) },
	};

	isEnable = false;
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

	// 공격패턴
	fireStateList.push_back(
		new WaitState{ 3});
	fireStateList.push_back(
		new CrossCurvesFireState{ this, 0.2f, 3.f });
	fireStateList.push_back(
		new WaitState{ 3 });
	fireStateList.push_back(
		new UniqueFlowerFireState{ this, 0.2f, 2.f });
	fireStateList.push_back(
		new WaitState{ 3 });
	fireStateList.push_back(
		new UniqueWarmFireState{ this, 0.2f, 2.f });
	fireStateList.push_back(
		new WaitState{ 3 });
	fireStateList.push_back(
		new ZigzagWarmFireState{ this, 0.2f, 2.f });
	speed = 100;

	fireStateList[0]->pNextState = fireStateList[1];
	fireStateList[1]->pNextState = fireStateList[2];
	fireStateList[2]->pNextState = fireStateList[3];
	fireStateList[3]->pNextState = fireStateList[4];
	fireStateList[4]->pNextState = fireStateList[5];
	fireStateList[5]->pNextState = fireStateList[6];
	fireStateList[6]->pNextState = fireStateList[7];
	fireStateList[7]->pNextState = fireStateList[0];
	currentFireState = this->fireStateList.front();
	currentFireState->Reset();

	monsterRect = RECT{ -36, -36, 36, 36 };
	colliders.push_back(RECT{ -32, -32, 32, 32 });
	hp = MAX_HP;

	monsterType = _monsterType;
	simpleCollider = CreateSimpleCollider(colliders);
	currentMoveState = this->moveStateList.front();
	currentMoveState->Reset();
	m_radian = 0.f;
}

Star::~Star()
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

void Star::Update()
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
	m_radian += 2 * 3.141592f * TimeManager::DeltaTime();
	auto mat{ DirectX::XMMatrixRotationZ(m_radian) * DirectX::XMMatrixTranslation(position.x, position.y, 0.f) };
	for (auto& it : m_vecParts)
	{
		auto star = std::static_pointer_cast<StarPart>(it);
		star->SetCenter(mat);
	}
}

void Star::Render()
{
	//RenderManager::DrawSimpleCollider(monsterRect + position, RGB(255, 0, 0));
	//for (auto& iter : m_vecParts)
	//{
	//	iter->Render();
	//}
	RenderManager::DrawCircle(monsterRect + position, RGB(255, 0, 0), RGB(255, 0, 0));
}

void Star::OnShow(const Event&)
{
	isEnable = true;
	for (auto& iter : m_vecParts);
}

void Star::OnCollision(const CollisionEvent& event)
{
	if (event.other->type == ObjectType::BULLET && this->isEnable)
	{
		auto pBullet = std::static_pointer_cast<Bullet>(event.other);
		if (pBullet->isAlias == false)
		{
			return;
		}
		hp -= pBullet->isAlias == true;
		const float MAX_HP{ static_cast<float>(Star::MAX_HP) };
		int percent{static_cast<int>(hp * 100.f / MAX_HP) };
		if (percent % 10 == 0 && hp != Star::MAX_HP && m_vecParts.empty() == false)
		{
			ObjectManager::DeleteObject(m_vecParts.back());
			m_vecParts.pop_back();
		}
		if (hp <= 0)
		{
			for (auto& it : m_vecParts)
			{
				ObjectManager::DeleteObject(it);
			}
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
