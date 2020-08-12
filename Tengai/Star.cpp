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
	Bind(EventId::COLLISION_OBJ, &Monster::OnCollision);
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

	monsterRect = RECT{ -36, -36, 36, 36 };
	colliders.push_back(RECT{ -32, -32, 32, 32 });
	hp = 10;

	monsterType = _monsterType;
	simpleCollider = CreateSimpleCollider(colliders);
	currentMoveState = this->moveStateList.front();
	currentMoveState->Reset();

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
	if (monsterType == MonsterType::STAR)
	{
		position.x += 1;
	}
	
}

void Star::Render()
{
	//RenderManager::DrawSimpleCollider(monsterRect + position, RGB(255, 0, 0));
	//for (auto& iter : m_vecParts)
	//{
	//	iter->Render();
	//}
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
